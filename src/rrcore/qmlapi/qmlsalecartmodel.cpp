#include "qmlsalecartmodel.h"
#include "database/databasethread.h"
#include "models/salepaymentmodel.h"
#include "queryexecutors/sales.h"
#include "queryexecutors/stock.h"
#include "utility/saleutils.h"
#include "singletons/settings.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

const int CASH_PAYMENT_LIMIT = 1;
const int CARD_PAYMENT_LIMIT = 2;

QMLSaleCartModel::QMLSaleCartModel(QObject *parent) :
    QMLSaleCartModel(DatabaseThread::instance(), parent)
{}

QMLSaleCartModel::QMLSaleCartModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    m_paymentModel = new SalePaymentModel(this);

    connect (m_paymentModel, &SalePaymentModel::cashPaymentCountChanged,
             this, &QMLSaleCartModel::updateCanAcceptCash);
    connect (m_paymentModel, &SalePaymentModel::cardPaymentCountChanged,
             this, &QMLSaleCartModel::updateCanAcceptCard);

    connect(this, &QMLSaleCartModel::transactionIdChanged,
            this, &QMLSaleCartModel::tryQuery);
}

int QMLSaleCartModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_transaction.products.count();
}

QVariant QMLSaleCartModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_transaction.products.at(index.row()).category.id;
    case CategoryRole:
        return m_transaction.products.at(index.row()).category.category;
    case ProductIdRole:
        return m_transaction.products.at(index.row()).id;
    case ProductRole:
        return m_transaction.products.at(index.row()).product;
    case AvailableQuantityRole:
        return m_transaction.products.at(index.row()).availableQuantity;
    case QuantityRole:
        return m_transaction.products.at(index.row()).quantity;
    case UnitRole:
        return m_transaction.products.at(index.row()).unit.unit;
    case UnitIdRole:
        return m_transaction.products.at(index.row()).unit.id;
    case CostPriceRole:
        return m_transaction.products.at(index.row()).costPrice;
    case RetailPriceRole:
        return m_transaction.products.at(index.row()).retailPrice;
    case UnitPriceRole:
        return m_transaction.products.at(index.row()).unitPrice;
    case CostRole:
        return m_transaction.products.at(index.row()).cost;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleCartModel::roleNames() const
{
    return {
        { CategoryIdRole, "category_id" },
        { CategoryRole, "category" },
        { ProductIdRole, "product_id" },
        { ProductRole, "product" },
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
    return m_transaction.id > -1;
}

qint64 QMLSaleCartModel::transactionId() const
{
    return m_transaction.id;
}

void QMLSaleCartModel::setTransactionId(qint64 transactionId)
{
    if (m_transaction.id == transactionId)
        return;

    m_transaction.id = transactionId;
    emit transactionIdChanged();
}

QString QMLSaleCartModel::customerName() const
{
    return m_transaction.customer.client.preferredName;
}

void QMLSaleCartModel::setCustomerName(const QString &customerName)
{
    if (m_transaction.customer.client.preferredName == customerName)
        return;

    m_transaction.customer.client.preferredName = customerName;
    emit customerNameChanged();
}

QString QMLSaleCartModel::customerPhoneNumber() const
{
    return m_transaction.customer.client.phoneNumber;
}

void QMLSaleCartModel::setCustomerPhoneNumber(const QString &customerPhoneNumber)
{
    if (m_transaction.customer.client.phoneNumber == customerPhoneNumber)
        return;

    m_transaction.customer.client.phoneNumber = customerPhoneNumber;
    emit customerPhoneNumberChanged();
}

int QMLSaleCartModel::clientId() const
{
    return m_transaction.customer.client.id;
}

QString QMLSaleCartModel::note() const
{
    return m_transaction.note.note;
}

void QMLSaleCartModel::setNote(const QString &note)
{
    if (m_transaction.note.note == note)
        return;

    m_transaction.note.note = note;
    emit noteChanged();
}

double QMLSaleCartModel::totalCost() const
{
    return m_transaction.totalCost;
}

void QMLSaleCartModel::setTotalCost(double totalCost)
{
    if (m_transaction.totalCost == totalCost)
        return;

    m_transaction.totalCost = totalCost;
    emit totalCostChanged();
}

double QMLSaleCartModel::amountPaid() const
{
    return m_transaction.amountPaid;
}

double QMLSaleCartModel::balance() const
{
    return m_transaction.balance;
}

bool QMLSaleCartModel::canAcceptCash() const
{
    return m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCash);
}

bool QMLSaleCartModel::canAcceptCard() const
{
    return m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCard);
}

SalePaymentModel *QMLSaleCartModel::paymentModel() const
{
    return m_paymentModel;
}

void QMLSaleCartModel::setAmountPaid(double amountPaid)
{
    if (m_transaction.amountPaid == amountPaid)
        return;

    m_transaction.amountPaid = amountPaid;
    emit amountPaidChanged();
}

void QMLSaleCartModel::setBalance(double balance)
{
    if (m_transaction.balance == balance)
        return;

    m_transaction.balance = balance;
    emit balanceChanged();
}

Utility::SalePaymentList QMLSaleCartModel::payments() const
{
    return m_transaction.payments;
}

void QMLSaleCartModel::addPayment(double amount, QMLSaleCartModel::PaymentMethod method, const QString &note)
{
    if (amount <= 0.0)
        return;

    Utility::SalePayment payment {
        amount,
                Utility::PaymentMethod{ method },
                Utility::Note{ note },
                Settings::DEFAULT_CURRENCY
    };
    m_paymentModel->addPayment(payment);
    m_transaction.payments.append(payment);

    calculateAmountPaid();
}

void QMLSaleCartModel::removePayment(int index)
{
    if (index <= -1 || index >= m_transaction.payments.count())
        return;

    m_paymentModel->removePayment(index);
    m_transaction.payments.removeAt(index);

    calculateAmountPaid();
}

void QMLSaleCartModel::submitTransaction(const QVariantMap &transactionInfo)
{
    if (m_transaction.balance != 0.0 && transactionInfo.value("due_date").isNull())
        emit error(NoDueDateSetError);
    else
        addTransaction(transactionInfo);
}

void QMLSaleCartModel::suspendTransaction(const QVariantMap &params)
{
    if (m_transaction.id == -1)
        addTransaction({ { "suspended", true },
                         { "action", "suspend" },
                         { "note", params.value("note") }
                       });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
}

void QMLSaleCartModel::clearPayments()
{
    if (!m_transaction.payments.isEmpty()) {
        m_paymentModel->clearPayments();
        m_transaction.payments.clear();
        calculateAmountPaid();
    }
}

void QMLSaleCartModel::clearAll()
{
    beginResetModel();
    setTransactionId(-1);
    setCustomerName(QString());
    setCustomerPhoneNumber(QString());
    m_transaction.products.clear();
    endResetModel();
}

QString QMLSaleCartModel::toPrintableFormat() const
{
    QJsonObject rootObject;
    rootObject.insert("name", m_transaction.customer.client.preferredName);
    rootObject.insert("phone_number", m_transaction.customer.client.phoneNumber);
    rootObject.insert("query_group", "sales");
    rootObject.insert("records", QJsonArray::fromVariantList(m_transaction.products.toVariantList()));

    return QJsonDocument(rootObject).toJson();
}

void QMLSaleCartModel::addTransaction(const QVariantMap &transaction)
{
    if (!m_transaction.products.isEmpty()) {
        setBusy(true);
        m_transaction.customer.client.id = transaction.value("client_id").toInt();
        m_transaction.flags.setFlag(Utility::RecordGroup::Suspended,
                                    transaction.value("suspended").toBool());
        m_transaction.dueDateTime = transaction.value("due_date_time").toDateTime();
        m_transaction.action = transaction.value("action").toString();
        m_transaction.note.note = transaction.value("note").toString();
        emit execute(new SaleQuery::AddSaleTransaction(m_transaction, this));
    } else {
        emit error(EmptyCartError);
    }
}

void QMLSaleCartModel::updateSuspendedTransaction(const QVariantMap &addOns)
{
    if (!m_transaction.products.isEmpty()) {
        setBusy(true);
        m_transaction.note.note = addOns.value("note").toString();
        emit execute(new SaleQuery::UpdateSuspendedSaleTransaction(
                         m_transaction,
                         this));
    } else {
        emit error(EmptyCartError);
    }
}

void QMLSaleCartModel::tryQuery()
{
    if (isExistingTransaction()) {
        setBusy(true);
        emit execute(new SaleQuery::ViewSaleCart(m_transaction.id, this));
    } else {
        beginResetModel();
        setCustomerName(QString());
        setCustomerPhoneNumber(QString());
        m_transaction.products.clear();
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
        m_transaction.products = Utility::SaleCartProductList{ result.outcome().toMap().value("products").toList() };
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
        params.insert("sale_transaction_id",
                      params.value("outcome").toMap().value("sale_transaction_id").toInt());
        request.setParams(params);

        auto addSaleTransaction = new SaleQuery::AddSaleTransaction(request, this);
        addSaleTransaction->undoOnNextExecution(true);
        emit execute(addSaleTransaction);
    }
}

void QMLSaleCartModel::addProduct(const QVariantMap &product)
{
    Utility::SaleCartProduct newProduct{ product };

    if (newProduct.availableQuantity == 0.0)
        return;

    if (!m_transaction.products.contains(newProduct)) {
        beginInsertRows(QModelIndex(), m_transaction.products.count(), m_transaction.products.count());
        m_transaction.products.append(newProduct);
        endInsertRows();
    } else {
        const int row = m_transaction.products.indexOf(newProduct);
        Utility::SaleCartProduct &existingProduct = m_transaction.products[row];
        const double oldQuantity = existingProduct.quantity;
        const double newQuantity = oldQuantity + 1;

        existingProduct.quantity = qMin(newQuantity, existingProduct.availableQuantity);
        existingProduct.cost = existingProduct.quantity * existingProduct.unitPrice;

        emit dataChanged(index(row), index(row));
    }

    calculateTotal();
}

void QMLSaleCartModel::updateProduct(int productId, const QVariantMap &product)
{
    if (productId <= 0 || product.isEmpty())
        return;

    const Utility::SaleCartProduct &updatedProduct{ product };
    const int row = m_transaction.products.indexOf(updatedProduct);
    Utility::SaleCartProduct &existingProduct{ m_transaction.products[row] };
    const double oldQuantity = existingProduct.quantity;
    const double availableQuantity = existingProduct.availableQuantity;
    const double quantity = existingProduct.quantity;
    const double oldUnitPrice = existingProduct.unitPrice;
    const double oldCost = existingProduct.cost;
    const double newQuantity = qMin(quantity, availableQuantity);
    const double newUnitPrice = existingProduct.unitPrice;
    const double newCost = existingProduct.cost;

    if (product.contains("quantity"))
        existingProduct.quantity = newQuantity;
    if (product.contains("cost"))
        existingProduct.cost = newCost;
    if (product.contains("unit_price"))
        existingProduct.unitPrice = newUnitPrice;

    if (oldQuantity != newQuantity || oldUnitPrice != newUnitPrice || oldCost != newCost) {
        emit dataChanged(index(row), index(row));
        calculateTotal();
    }
}

void QMLSaleCartModel::setProductQuantity(int productId, double quantity)
{
    if (productId <= 0 || quantity <= 0.0)
        return;

    const int row = m_transaction.products.indexOf(Utility::SaleCartProduct{ productId });
    Utility::SaleCartProduct &existingProduct{ m_transaction.products[row] };
    const double oldQuantity = existingProduct.quantity;
    const double availableQuantity = existingProduct.availableQuantity;
    const double newQuantity = qMin(quantity, availableQuantity);
    const double unitPrice = existingProduct.unitPrice;

    existingProduct.quantity = newQuantity;
    existingProduct.cost = newQuantity * unitPrice;

    if (oldQuantity != newQuantity) {
        emit dataChanged(index(row), index(row));
        calculateTotal();
    }
}

void QMLSaleCartModel::updateCanAcceptCash()
{
    const bool canAcceptCash = m_paymentModel->cashPaymentCount() < CASH_PAYMENT_LIMIT;
    if (m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCash)) {
        m_transaction.flags.setFlag(Utility::RecordGroup::AcceptsCash, canAcceptCash);
        emit canAcceptCashChanged();
    }
}

void QMLSaleCartModel::updateCanAcceptCard()
{
    const bool canAcceptCard = m_paymentModel->cardPaymentCount() < CARD_PAYMENT_LIMIT;
    if (m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCard)) {
        m_transaction.flags.setFlag(Utility::RecordGroup::AcceptsCard, canAcceptCard);
        emit canAcceptCashChanged();
    }
}

void QMLSaleCartModel::removeProduct(int productId)
{
    const int row = m_transaction.products.indexOf(Utility::SaleCartProduct{ productId });
    beginRemoveRows(QModelIndex(), row, row);
    m_transaction.products.removeAt(row);
    endRemoveRows();

    calculateTotal();
}

void QMLSaleCartModel::calculateTotal()
{
    double totalCost = 0.0;
    for (const auto &product : m_transaction.products)
        totalCost += product.cost;

    setTotalCost(totalCost);
    setBalance(m_transaction.totalCost - m_transaction.amountPaid);
}

void QMLSaleCartModel::calculateAmountPaid()
{
    double amountPaid = 0.0;
    for (const auto &payment : m_transaction.payments)
        amountPaid += payment.amount;

    setAmountPaid(amountPaid);
    setBalance(m_transaction.totalCost - m_transaction.amountPaid);
}

void QMLSaleCartModel::setClientId(int clientId)
{
    if (m_transaction.customer.client.id == clientId)
        return;

    m_transaction.customer.client.id = clientId;
    emit clientIdChanged();
}
