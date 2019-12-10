#include "qmlsalecartmodel.h"
#include <QSqlField>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "models/salepaymentmodel.h"
#include "queryexecutors/sales.h"
#include "queryexecutors/stock.h"
#include "utility/saleutils.h"
#include "utility/stockutils.h"

const int CASH_PAYMENT_LIMIT = 1;
const int CARD_PAYMENT_LIMIT = 2;

QMLSaleCartModel::QMLSaleCartModel(QObject *parent) :
    QMLSaleCartModel(DatabaseThread::instance(), parent)
{}

QMLSaleCartModel::QMLSaleCartModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent),
    m_transactionId(-1),
    m_clientId(-1),
    m_totalCost(0.0),
    m_amountPaid(0.0),
    m_balance(0.0),
    m_canAcceptCash(true),
    m_canAcceptCard(false) // NOTE: Toggle to disable, genius
{
    m_paymentModel = new SalePaymentModel(this);

    connect (m_paymentModel, &SalePaymentModel::cashPaymentCountChanged, this, &QMLSaleCartModel::updateCanAcceptCash);
    connect (m_paymentModel, &SalePaymentModel::cardPaymentCountChanged, this, &QMLSaleCartModel::updateCanAcceptCard);

    connect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);
}

int QMLSaleCartModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLSaleCartModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ItemIdRole:
        return m_records.at(index.row()).toMap().value("item_id").toInt();
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
    case AvailableQuantityRole:
        return m_records.at(index.row()).toMap().value("available_quantity").toDouble();
    case QuantityRole:
        return m_records.at(index.row()).toMap().value("quantity").toDouble();
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
    case UnitIdRole:
        return m_records.at(index.row()).toMap().value("unit_id").toInt();
    case CostPriceRole:
        return m_records.at(index.row()).toMap().value("cost_price").toDouble();
    case RetailPriceRole:
        return m_records.at(index.row()).toMap().value("retail_price").toDouble();
    case UnitPriceRole:
        return m_records.at(index.row()).toMap().value("unit_price").toDouble();
    case CostRole:
        return m_records.at(index.row()).toMap().value("cost").toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleCartModel::roleNames() const
{
    return {
        { CategoryIdRole, "category_id" },
        { CategoryRole, "category" },
        { ItemIdRole, "item_id" },
        { ItemRole, "item" },
        { AvailableQuantityRole, "available_quantity" },
        { QuantityRole, "quantity" },
        { UnitRole, "unit" },
        { UnitIdRole, "unit_id" },
        { CostPriceRole, "cost_price" },
        { RetailPriceRole, "retail_price" },
        { UnitPriceRole, "unit_price" },
        { CostRole, "cost" }
    };
}

bool QMLSaleCartModel::isExistingTransaction() const
{
    return m_transactionId > -1;
}

qint64 QMLSaleCartModel::transactionId() const
{
    return m_transactionId;
}

void QMLSaleCartModel::setTransactionId(qint64 transactionId)
{
    if (m_transactionId == transactionId)
        return;

    m_transactionId = transactionId;
    emit transactionIdChanged();
}

QString QMLSaleCartModel::customerName() const
{
    return m_customerName;
}

void QMLSaleCartModel::setCustomerName(const QString &customerName)
{
    if (m_customerName == customerName)
        return;

    m_customerName = customerName;
    emit customerNameChanged();
}

QString QMLSaleCartModel::customerPhoneNumber() const
{
    return m_customerPhoneNumber;
}

void QMLSaleCartModel::setCustomerPhoneNumber(const QString &customerPhoneNumber)
{
    if (m_customerPhoneNumber == customerPhoneNumber)
        return;

    m_customerPhoneNumber = customerPhoneNumber;
    emit customerPhoneNumberChanged();
}

int QMLSaleCartModel::clientId() const
{
    return m_clientId;
}

QString QMLSaleCartModel::note() const
{
    return m_note;
}

void QMLSaleCartModel::setNote(const QString &note)
{
    if (m_note == note)
        return;

    m_note = note;
    emit noteChanged();
}

double QMLSaleCartModel::totalCost() const
{
    return m_totalCost;
}

void QMLSaleCartModel::setTotalCost(double totalCost)
{
    if (m_totalCost == totalCost)
        return;

    m_totalCost = totalCost;
    emit totalCostChanged();
}

double QMLSaleCartModel::amountPaid() const
{
    return m_amountPaid;
}

double QMLSaleCartModel::balance() const
{
    return m_balance;
}

bool QMLSaleCartModel::canAcceptCash() const
{
    return m_canAcceptCash;
}

bool QMLSaleCartModel::canAcceptCard() const
{
    return m_canAcceptCard;
}

SalePaymentModel *QMLSaleCartModel::paymentModel() const
{
    return m_paymentModel;
}

void QMLSaleCartModel::setAmountPaid(double amountPaid)
{
    if (m_amountPaid == amountPaid)
        return;

    m_amountPaid = amountPaid;
    emit amountPaidChanged();
}

void QMLSaleCartModel::setBalance(double balance)
{
    if (m_balance == balance)
        return;

    m_balance = balance;
    emit balanceChanged();
}

QList<SalePayment> QMLSaleCartModel::payments() const
{
    return m_salePayments;
}

void QMLSaleCartModel::addPayment(double amount, QMLSaleCartModel::PaymentMethod method, const QString &note)
{
    if (amount <= 0.0)
        return;

    SalePayment payment{ amount, static_cast<SalePayment::PaymentMethod>(method), note, QStringLiteral("NGN") };
    m_paymentModel->addPayment(payment);
    m_salePayments.append(payment);

    calculateAmountPaid();
}

void QMLSaleCartModel::removePayment(int index)
{
    if (index <= -1 || index >= m_salePayments.count())
        return;

    m_paymentModel->removePayment(index);
    m_salePayments.removeAt(index);

    calculateAmountPaid();
}

void QMLSaleCartModel::submitTransaction(const QVariantMap &transactionInfo)
{
    if (m_balance != 0.0 && transactionInfo.value("due_date").isNull())
        emit error(NoDueDateSetError);
    else
        addTransaction(transactionInfo);
}

void QMLSaleCartModel::suspendTransaction(const QVariantMap &params)
{
    if (m_transactionId == -1)
        addTransaction({ { "suspended", true },
                         { "action", "suspend" },
                         { "note", params.value("note") }
                       });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
}

void QMLSaleCartModel::clearPayments()
{
    if (!m_salePayments.isEmpty()) {
        m_paymentModel->clearPayments();
        m_salePayments.clear();
        calculateAmountPaid();
    }
}

void QMLSaleCartModel::clearAll()
{
    beginResetModel();
    setTransactionId(-1);
    setCustomerName(QString());
    setCustomerPhoneNumber(QString());
    m_records.clear();
    endResetModel();
}

QString QMLSaleCartModel::toPrintableFormat() const
{
    QJsonObject rootObject;
    rootObject.insert("name", m_customerName);
    rootObject.insert("phone_number", m_customerPhoneNumber);
    rootObject.insert("query_group", "sales");
    rootObject.insert("records", QJsonArray::fromVariantList(m_records));

    return QJsonDocument(rootObject).toJson();
}

void QMLSaleCartModel::addTransaction(const QVariantMap &transactionInfo)
{
    if (!m_records.isEmpty()) {
        StockItemList items;
        for (const QVariant &record : m_records) {
            items.append(StockItem{
                             record.toMap().value("category_id").toInt(),
                             record.toMap().value("item_id").toInt(),
                             record.toMap().value("quantity").toDouble(),
                             record.toMap().value("unit_id").toInt(),
                             record.toMap().value("retail_price").toDouble(),
                             record.toMap().value("unit_price").toDouble(),
                             record.toMap().value("cost").toDouble(),
                             record.toMap().value("amount_paid").toDouble(),
                             record.toMap().value("note").toString()
                         });
        }

        setBusy(true);
        emit execute(new SaleQuery::AddSaleTransaction(m_transactionId,
                                                       m_customerName,
                                                       transactionInfo.value("client_id").toInt(),
                                                       m_customerPhoneNumber,
                                                       m_totalCost,
                                                       m_amountPaid,
                                                       m_balance,
                                                       transactionInfo.value("suspended", false).toBool(),
                                                       transactionInfo.value("due_date", QDateTime()).toDateTime(),
                                                       transactionInfo.value("action").toString(),
                                                       transactionInfo.value("note").toString(),
                                                       m_salePayments,
                                                       items,
                                                       this));
    } else {
        emit error(EmptyCartError);
    }
}

void QMLSaleCartModel::updateSuspendedTransaction(const QVariantMap &transactionInfo)
{
    if (!m_records.isEmpty()) {
        StockItemList items;
        for (const QVariant &record : m_records) {
            items.append(StockItem{
                             record.toMap().value("category_id").toInt(),
                             record.toMap().value("item_id").toInt(),
                             record.toMap().value("quantity").toDouble(),
                             record.toMap().value("unit_id").toInt(),
                             record.toMap().value("retail_price").toDouble(),
                             record.toMap().value("unit_price").toDouble(),
                             record.toMap().value("cost").toDouble(),
                             record.toMap().value("amount_paid").toDouble(),
                             record.toMap().value("note").toString()
                         });
        }

        setBusy(true);
        emit execute(new SaleQuery::UpdateSuspendedSaleTransaction(
                         m_transactionId,
                         m_customerName,
                         m_clientId,
                         m_customerPhoneNumber,
                         m_totalCost,
                         m_amountPaid,
                         m_balance,
                         transactionInfo.value("note", QVariant(QVariant::String)).toString(),
                         true,
                         this));
    } else {
        emit error(EmptyCartError);
    }
}

void QMLSaleCartModel::tryQuery()
{
    if (isExistingTransaction()) {
        setBusy(true);
        emit execute(new SaleQuery::ViewSaleCart(m_transactionId, this));
    } else {
        beginResetModel();
        setCustomerName(QString());
        setCustomerPhoneNumber(QString());
        m_records.clear();
        calculateTotal();
        endResetModel();
    }
}

void QMLSaleCartModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    // Do not re-query if the transaction ID is changed in this method
    disconnect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);

    if (result.isSuccessful()) {
        beginResetModel();

        clearPayments();
        m_records = result.outcome().toMap().value("items").toList();
        calculateTotal();

        endResetModel();

        if (result.request().command() == SaleQuery::AddSaleTransaction::COMMAND) {
            if (result.request().params().value("suspended").toBool()) {
                setTransactionId(-1);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setClientId(result.outcome().toMap().value("client_id", -1).toInt());
                emit success(SuspendTransactionSuccess);
            } else {
                setTransactionId(-1);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setClientId(result.outcome().toMap().value("client_id", -1).toInt());
                emit success(SubmitTransactionSuccess);
            }
        } else if (result.request().command() == SaleQuery::AddSaleTransaction::UNDO_COMMAND) {
            setTransactionId(-1);
            setCustomerName(QString());
            setCustomerPhoneNumber(QString());
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            emit success(UndoSubmitTransactionSuccess);
        } else if (result.request().command() == SaleQuery::ViewSaleCart::COMMAND) {
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(RetrieveTransactionSuccess);
        } else if (result.request().command() == SaleQuery::UpdateSuspendedSaleTransaction::COMMAND) {
            setTransactionId(-1);
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(SuspendTransactionSuccess);
        } else {
            emit success(UnknownSuccess);
        }
    } else {
        if (result.request().command() == SaleQuery::AddSaleTransaction::COMMAND) {
            if (result.request().params().value("suspended").toBool())
                emit error(SuspendTransactionError);
            else
                emit error(SubmitTransactionError);
        } else if (result.request().command() == SaleQuery::AddSaleTransaction::UNDO_COMMAND) {
            emit error(UndoSubmitTransactionError);
        } else if (result.request().command() == SaleQuery::ViewSaleCart::COMMAND) {
            emit error(RetrieveTransactionError);
        } else if (result.request().command() == SaleQuery::UpdateSuspendedSaleTransaction::COMMAND) {
            emit error(SuspendTransactionError);
        } else {
            emit error(UnknownError);
        }
    }

    connect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);
}

void QMLSaleCartModel::undoLastCommit()
{
    QueryRequest request{ lastSuccessfulRequest() };
    QVariantMap params{ lastSuccessfulRequest().params() };
    if (lastSuccessfulRequest().command() == SaleQuery::AddSaleTransaction::COMMAND) {
        params.insert("transaction_id", params.value("outcome").toMap().value("transaction_id").toInt());
        request.setParams(params);

        auto addSaleTransaction = new SaleQuery::AddSaleTransaction(request, this);
        addSaleTransaction->undoOnNextExecution(true);
        emit execute(addSaleTransaction);
    }
}

void QMLSaleCartModel::addItem(const QVariantMap &itemInfo)
{
    const int categoryId = itemInfo.value("category_id").toInt();
    const QString &category = itemInfo.value("category").toString();
    const int itemId = itemInfo.value("item_id").toInt();
    const QString &item = itemInfo.value("item").toString();
    const double availableQuantity = itemInfo.value("available_quantity",
                                                    itemInfo.value("quantity").toDouble()).toDouble(); // TODO: Simplify
    const int unitId = itemInfo.value("unit_id").toInt();
    const QString &unit = itemInfo.value("unit").toString();
    const double costPrice = itemInfo.value("cost_price").toDouble();
    const double retailPrice = itemInfo.value("retail_price").toDouble();
    const double unitPrice = itemInfo.value("unit_price", retailPrice).toDouble();

    if (availableQuantity == 0.0)
        return;

    if (!containsItem(itemId)) {
        beginInsertRows(QModelIndex(), m_records.count(), m_records.count());

        QVariantMap record;
        record.insert("category_id", categoryId);
        record.insert("category", category);
        record.insert("item_id", itemId);
        record.insert("item", item);
        record.insert("available_quantity", availableQuantity);
        record.insert("quantity", qMin(1.0, availableQuantity));
        record.insert("unit_id", unitId);
        record.insert("unit", unit);
        record.insert("cost_price", costPrice);
        record.insert("retail_price", retailPrice);
        record.insert("unit_price", unitPrice);
        record.insert("cost", record.value("quantity").toDouble() * unitPrice);
        m_records.append(record);

        endInsertRows();
    } else {
        const int row = indexOfItem(itemId);
        QVariantMap record(m_records.at(row).toMap());
        const double oldQuantity = record.value("quantity").toDouble();
        const double newQuantity = oldQuantity + 1;

        record.insert("quantity", qMin(newQuantity, availableQuantity));
        record.insert("cost", record.value("quantity").toDouble() * unitPrice);
        m_records.replace(row, record);

        emit dataChanged(index(row), index(row));
    }

    calculateTotal();
}

void QMLSaleCartModel::updateItem(int itemId, const QVariantMap &itemInfo)
{
    if (itemId <= 0 || itemInfo.isEmpty())
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record(m_records[row].toMap());
    const double oldQuantity = record.value("quantity").toDouble();
    const double availableQuantity = record.value("available_quantity").toDouble();
    const double quantity = itemInfo.value("quantity").toDouble();
    const double oldUnitPrice = record.value("unit_price").toDouble();
    const double oldCost = record.value("cost").toDouble();
    const double newQuantity = qMin(quantity, availableQuantity);
    const double newUnitPrice = itemInfo.value("unit_price").toDouble();
    const double newCost = itemInfo.value("cost").toDouble();

    if (itemInfo.contains("quantity"))
        record.insert("quantity", newQuantity);
    if (itemInfo.contains("cost"))
        record.insert("cost", newCost);
    if (itemInfo.contains("unit_price"))
        record.insert("unit_price", newUnitPrice);
    m_records.replace(row, record);

    if (oldQuantity != newQuantity || oldUnitPrice != newUnitPrice || oldCost != newCost) {
        emit dataChanged(index(row), index(row));
        calculateTotal();
    }
}

void QMLSaleCartModel::setItemQuantity(int itemId, double quantity)
{
    if (itemId <= 0 || quantity <= 0.0)
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record(m_records[row].toMap());
    const double oldQuantity = record.value("quantity").toDouble();
    const double availableQuantity = record.value("available_quantity").toDouble();
    const double newQuantity = qMin(quantity, availableQuantity);
    const double unitPrice = record.value("unit_price").toDouble();

    record.insert("quantity", newQuantity);
    record.insert("cost", newQuantity * unitPrice);
    m_records.replace(row, record);

    if (oldQuantity != newQuantity) {
        emit dataChanged(index(row), index(row));
        calculateTotal();
    }
}

void QMLSaleCartModel::incrementItemQuantity(int itemId, double quantity)
{
    if (quantity <= 0.0)
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record(m_records[row].toMap());
    const double oldQuantity = record.value("quantity").toDouble();
    const double availableQuantity = record.value("available_quantity").toDouble();
    const double newQuantity = qMin(oldQuantity + quantity, availableQuantity);
    const double unitPrice = record.value("unit_price").toDouble();

    record.insert("quantity", newQuantity);
    record.insert("cost", newQuantity * unitPrice);
    m_records.replace(row, record);

    emit dataChanged(index(row), index(row));

    calculateTotal();
}

void QMLSaleCartModel::decrementItemQuantity(int itemId, double quantity)
{
    if (quantity <= 0.0)
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record(m_records[row].toMap());
    const double oldQuantity = record.value("quantity").toDouble();
    const double newQuantity = qMax(oldQuantity - quantity, 0.0);
    const double unitPrice = record.value("unit_price").toDouble();

    record.insert("quantity", newQuantity);
    record.insert("cost", newQuantity * unitPrice);
    m_records.replace(row, record);

    emit dataChanged(index(row), index(row));

    calculateTotal();
}

void QMLSaleCartModel::updateCanAcceptCash()
{
    const bool canAcceptCash = m_paymentModel->cashPaymentCount() < CASH_PAYMENT_LIMIT;
    if (m_canAcceptCash != canAcceptCash) {
        m_canAcceptCash = canAcceptCash;
        emit canAcceptCashChanged();
    }
}

void QMLSaleCartModel::updateCanAcceptCard()
{
    const bool canAcceptCard = m_paymentModel->cardPaymentCount() < CARD_PAYMENT_LIMIT;
    if (m_canAcceptCard != canAcceptCard) {
        m_canAcceptCard = canAcceptCard;
        emit canAcceptCardChanged();
    }
}

void QMLSaleCartModel::removeItem(int itemId)
{
    const int row = indexOfItem(itemId);
    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();

    calculateTotal();
}

bool QMLSaleCartModel::containsItem(int itemId)
{
    for (const QVariant &record : m_records)
        if (record.toMap().value("item_id").toInt() == itemId)
            return true;

    return false;
}

int QMLSaleCartModel::indexOfItem(int itemId)
{
    if (itemId <= 0)
        return -1;

    for (int i = 0; i < m_records.count(); ++i)
        if (m_records.at(i).toMap().value("item_id").toInt() == itemId)
            return i;

    return -1;
}

void QMLSaleCartModel::calculateTotal()
{
    double totalCost = 0.0;
    for (const QVariant &record : m_records)
        totalCost += record.toMap().value("cost").toDouble();

    setTotalCost(totalCost);
    setBalance(m_totalCost - m_amountPaid);
}

void QMLSaleCartModel::calculateAmountPaid()
{
    double amountPaid = 0.0;
    for (const SalePayment &salePayment : m_salePayments)
        amountPaid += salePayment.amount;

    setAmountPaid(amountPaid);
    setBalance(m_totalCost - m_amountPaid);
}

void QMLSaleCartModel::setClientId(int clientId)
{
    if (m_clientId == clientId)
        return;

    m_clientId = clientId;
    emit clientIdChanged();
}
