#include "qmldebttransactionmodel.h"
#include "models/debtpaymentmodel.h"

#include <QDateTime>

QMLDebtTransactionModel::QMLDebtTransactionModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_debtorId(-1)
{
    connect(this, &QMLDebtTransactionModel::debtorIdChanged, this, &QMLDebtTransactionModel::tryQuery);
}

int QMLDebtTransactionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLDebtTransactionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TransactionIdRole:
        return m_records.at(index.row()).toMap().value("transaction_id").toInt();
        break;
    case RelatedTransactionRole: {
        const QString &relatedTransaction = m_records.at(index.row()).toMap().value("related_transaction_table").toString();
        if (relatedTransaction == "sale_transaction")
            return tr("Sales");
        else
            return relatedTransaction;
    }
        break;
    case RelatedTransactionIdRole:
        return m_records.at(index.row()).toMap().value("related_transaction_id").toInt();
        break;
    case NoteRole:
        return m_records.at(index.row()).toMap().value("note_id").toString();
        break;
    case DueDateRole:
        return m_records.at(index.row()).toMap().value("due_date").toString();
        break;
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDateTime();
        break;
    case PaymentModelRole:
        return QVariant::fromValue<QObject *>(m_debtPaymentModels.at(index.row()));
        break;
    case CurrentBalanceRole: {
        DebtPaymentModel *model = m_debtPaymentModels.at(index.row());
        return model->data(model->index(model->rowCount() - 1), DebtPaymentModel::BalanceRole).toDouble();
    }
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtTransactionModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(TransactionIdRole, "transaction_id");
    roles.insert(RelatedTransactionRole, "related_transaction");
    roles.insert(RelatedTransactionIdRole, "related_transaction_id");
    roles.insert(NoteRole, "note");
    roles.insert(DueDateRole, "due_date");
    roles.insert(CreatedRole, "created");
    roles.insert(PaymentModelRole, "payment_model");
    roles.insert(CurrentBalanceRole, "current_balance");

    return roles;
}

int QMLDebtTransactionModel::debtorId() const
{
    return m_debtorId;
}

void QMLDebtTransactionModel::setDebtorId(int debtorId)
{
    if (m_debtorId == debtorId)
        return;

    m_debtorId = debtorId;
    emit debtorIdChanged();
}

void QMLDebtTransactionModel::clearPayments()
{
    if (!m_debtPaymentModels.isEmpty()) {
        qDeleteAll(m_debtPaymentModels);
        m_debtPaymentModels.clear();
    }
}

void QMLDebtTransactionModel::tryQuery()
{
    if (m_debtorId <= 0)
        return;

    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_debt_transactions", { { "debtor_id", m_debtorId } }, QueryRequest::Debtor);
    emit executeRequest(request);
}

void QMLDebtTransactionModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_debt_transactions") {
            beginResetModel();

            clearPayments();

            m_records = result.outcome().toMap().value("transactions").toList();
            const QVariantList paymentGroups = result.outcome().toMap().value("payment_groups").toList();

            for (const QVariant &paymentGroup : paymentGroups) {
                const QVariantList &payments = paymentGroup.toList();

                DebtPaymentModel *model = new DebtPaymentModel(this);

                model->setTransactionId(payments.first().toMap().value("transaction_id").toInt());
                model->setPayments(payments);
                m_debtPaymentModels.append(model);
            }

            endResetModel();
        }
        emit success(UnknownSuccess);
    } else {
        emit error(UnknownError);
    }
}
