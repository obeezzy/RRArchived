#include "qmlpurchasecartmodel.h"
#include <QSqlField>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "models/purchasepaymentmodel.h"
#include "queryexecutors/purchase.h"
#include "utility/purchaseutils.h"
#include "utility/stockutils.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

const int CASH_PAYMENT_LIMIT = 1;
const int CARD_PAYMENT_LIMIT = 2;

QMLPurchaseCartModel::QMLPurchaseCartModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseCartModel::QMLPurchaseCartModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent),
    m_transactionId(-1),
    m_clientName(QString()),
    m_customerPhoneNumber(QString()),
    m_clientId(-1),
    m_note(QString()),
    m_totalCost(0.0),
    m_amountPaid(0.0),
    m_balance(0.0),
    m_canAcceptCash(true),
    m_canAcceptCard(false), // Toggle to disable, genius
    m_records(QVariantList())
{
    m_paymentModel = new PurchasePaymentModel(this);

    connect (m_paymentModel, &PurchasePaymentModel::cashPaymentCountChanged, this, &QMLPurchaseCartModel::updateCanAcceptCash);
    connect (m_paymentModel, &PurchasePaymentModel::cardPaymentCountChanged, this, &QMLPurchaseCartModel::updateCanAcceptCard);

    connect(this, &QMLPurchaseCartModel::transactionIdChanged, this, &QMLPurchaseCartModel::tryQuery);
}

int QMLPurchaseCartModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLPurchaseCartModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case CategoryIdRole:
        return m_records.at(index.row()).toMap().value("category_id").toInt();
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
    case ProductIdRole:
        return m_records.at(index.row()).toMap().value("product_id").toInt();
    case ProductRole:
        return m_records.at(index.row()).toMap().value("product").toString();
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

QHash<int, QByteArray> QMLPurchaseCartModel::roleNames() const
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

qint64 QMLPurchaseCartModel::transactionId() const
{
    return m_transactionId;
}

void QMLPurchaseCartModel::setTransactionId(qint64 transactionId)
{
    if (m_transactionId == transactionId)
        return;

    m_transactionId = transactionId;
    emit transactionIdChanged();
}

QString QMLPurchaseCartModel::customerName() const
{
    return m_clientName;
}

void QMLPurchaseCartModel::setCustomerName(const QString &customerName)
{
    if (m_clientName == customerName)
        return;

    m_clientName = customerName;
    emit customerNameChanged();
}

QString QMLPurchaseCartModel::customerPhoneNumber() const
{
    return m_customerPhoneNumber;
}

void QMLPurchaseCartModel::setCustomerPhoneNumber(const QString &customerPhoneNumber)
{
    if (m_customerPhoneNumber == customerPhoneNumber)
        return;

    m_customerPhoneNumber = customerPhoneNumber;
    emit customerPhoneNumberChanged();
}

int QMLPurchaseCartModel::clientId() const
{
    return m_clientId;
}

QString QMLPurchaseCartModel::note() const
{
    return m_note;
}

void QMLPurchaseCartModel::setNote(const QString &note)
{
    if (m_note == note)
        return;

    m_note = note;
    emit noteChanged();
}

double QMLPurchaseCartModel::totalCost() const
{
    return m_totalCost;
}

void QMLPurchaseCartModel::setTotalCost(double totalCost)
{
    if (m_totalCost == totalCost)
        return;

    m_totalCost = totalCost;
    emit totalCostChanged();
}

double QMLPurchaseCartModel::amountPaid() const
{
    return m_amountPaid;
}

double QMLPurchaseCartModel::balance() const
{
    return m_balance;
}

bool QMLPurchaseCartModel::canAcceptCash() const
{
    return m_canAcceptCash;
}

bool QMLPurchaseCartModel::canAcceptCard() const
{
    return m_canAcceptCard;
}

PurchasePaymentModel *QMLPurchaseCartModel::paymentModel() const
{
    return m_paymentModel;
}

void QMLPurchaseCartModel::setAmountPaid(double amountPaid)
{
    if (m_amountPaid == amountPaid)
        return;

    m_amountPaid = amountPaid;
    emit amountPaidChanged();
}

void QMLPurchaseCartModel::setBalance(double balance)
{
    if (m_balance == balance)
        return;

    m_balance = balance;
    emit balanceChanged();
}

PurchasePaymentList QMLPurchaseCartModel::payments() const
{
    return m_purchasePayments;
}

void QMLPurchaseCartModel::addPayment(double amount, QMLPurchaseCartModel::PaymentMethod method, const QString &note)
{
    if (amount <= 0.0)
        return;

    PurchasePayment payment{ amount, static_cast<PurchasePayment::PaymentMethod>(method), note, "NGN" };
    m_paymentModel->addPayment(payment);
    m_purchasePayments.append(payment);

    calculateAmountPaid();
}

void QMLPurchaseCartModel::removePayment(int index)
{
    if (index <= -1 || index >= m_purchasePayments.count())
        return;

    m_paymentModel->removePayment(index);
    m_purchasePayments.removeAt(index);

    calculateAmountPaid();
}

void QMLPurchaseCartModel::submitTransaction(const QVariantMap &transactionInfo)
{
    if (m_balance != 0.0 && transactionInfo.value("due_date").isNull())
        emit error(NoDueDateSetError);
    else
        addTransaction(transactionInfo);
}

void QMLPurchaseCartModel::suspendTransaction(const QVariantMap &params)
{
    if (m_transactionId == -1)
        addTransaction( { { "suspended", true },
                          { "action", "suspend" },
                          { "note", params.value("note") } });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
}

void QMLPurchaseCartModel::clearPayments()
{
    if (!m_purchasePayments.isEmpty()) {
        m_paymentModel->clearPayments();
        m_purchasePayments.clear();
        calculateAmountPaid();
    }
}

void QMLPurchaseCartModel::clearAll()
{
    beginResetModel();
    setTransactionId(-1);
    setCustomerName(QString());
    setCustomerPhoneNumber(QString());
    m_records.clear();
    endResetModel();
}

QString QMLPurchaseCartModel::toPrintableFormat() const
{
    QJsonObject rootObject;
    rootObject.insert("name", m_clientName);
    rootObject.insert("phone_number", m_customerPhoneNumber);
    rootObject.insert("group", "sales");
    rootObject.insert("records", QJsonArray::fromVariantList(m_records));

    return QJsonDocument(rootObject).toJson();
}

void QMLPurchaseCartModel::addTransaction(const QVariantMap &transaction)
{
    if (!m_records.isEmpty()) {
        PurchaseCartProductList products;
        for (const QVariant &record : m_records) {
            QVariantMap product;

            product.insert("product_category_id", record.toMap().value("product_category_id"));
            product.insert("product_id", record.toMap().value("product_id"));
            product.insert("quantity", record.toMap().value("quantity"));
            product.insert("product_unit_id", record.toMap().value("product_unit_id"));
            product.insert("retail_price", record.toMap().value("retail_price"));
            product.insert("unit_price", record.toMap().value("unit_price"));
            product.insert("cost", record.toMap().value("cost"));
            product.insert("amount_paid", record.toMap().value("amount_paid"));
            product.insert("note", record.toMap().value("note"));

            products.append(PurchaseCartProduct {
                                record.toMap().value("product_id").toInt(),
                                record.toMap().value("product").toString(),
                                StockProductCategory {
                                    record.toMap().value("product_category_id").toInt(),
                                    record.toMap().value("category").toString()
                                },
                                record.toMap().value("quantity").toDouble(),
                                StockProductUnit {
                                    record.toMap().value("product_unit_id").toInt(),
                                    record.toMap().value("unit").toString()
                                },
                                record.toMap().value("retail_price").toDouble(),
                                record.toMap().value("unit_price").toDouble(),
                                record.toMap().value("cost").toDouble(),
                                record.toMap().value("amount_paid").toDouble(),
                                Note{ record.toMap().value("note").toString() }
                            });
        }

        setBusy(true);
        emit execute(new PurchaseQuery::AddPurchaseTransaction(
                         m_transactionId,
                         m_clientId,
                         m_clientName,
                         m_customerPhoneNumber,
                         m_totalCost,
                         m_amountPaid,
                         m_balance,
                         transaction.value("suspended", false).toBool(),
                         transaction.value("due_date", QDateTime()).toDateTime(),
                         transaction.value("action").toString(),
                         m_purchasePayments,
                         products,
                         m_note,
                         this));
    } else {
        emit error(EmptyCartError);
    }
}

void QMLPurchaseCartModel::updateSuspendedTransaction(const QVariantMap &transaction)
{
    if (!m_records.isEmpty()) {
        PurchaseCartProductList products;
        for (const QVariant &record : m_records) {
            products.append(PurchaseCartProduct {
                                record.toMap().value("product_id").toInt(),
                                record.toMap().value("product").toString(),
                                StockProductCategory {
                                    record.toMap().value("product_category_id").toInt(),
                                    record.toMap().value("category").toString()
                                },
                                record.toMap().value("quantity").toDouble(),
                                StockProductUnit {
                                    record.toMap().value("product_unit_id").toInt(),
                                    record.toMap().value("unit").toString()
                                },
                                record.toMap().value("retail_price").toDouble(),
                                record.toMap().value("unit_price").toDouble(),
                                record.toMap().value("cost").toDouble(),
                                record.toMap().value("amount_paid").toDouble(),
                                Note{ record.toMap().value("note").toString() }
                            });
        }

        setBusy(true);

        emit execute(new PurchaseQuery::UpdateSuspendedPurchaseTransaction(
                         m_transactionId,
                         m_clientId,
                         m_clientName,
                         m_customerPhoneNumber,
                         m_totalCost,
                         m_amountPaid,
                         m_balance,
                         true,
                         products,
                         Note{transaction.value("note").toString()},
                         this));
    } else {
        emit error(EmptyCartError);
    }
}

void QMLPurchaseCartModel::tryQuery()
{
    if (m_transactionId > -1) {
        setBusy(true);
        emit execute(new PurchaseQuery::ViewPurchaseCart(m_transactionId, this));
    } else {
        beginResetModel();
        setCustomerName(QString());
        setCustomerPhoneNumber(QString());
        m_records.clear();
        calculateTotal();
        endResetModel();
    }
}

void QMLPurchaseCartModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);

    // Do not re-query if the transaction ID is changed in this method
    disconnect(this, &QMLPurchaseCartModel::transactionIdChanged, this, &QMLPurchaseCartModel::tryQuery);

    if (result.isSuccessful()) {
        beginResetModel();

        clearPayments();
        m_records = result.outcome().toMap().value("products").toList();
        calculateTotal();

        endResetModel();

        if (result.request().command() == PurchaseQuery::AddPurchaseTransaction::COMMAND) {
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
        } else if (result.request().command() == PurchaseQuery::ViewPurchaseCart::COMMAND) {
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(RetrieveTransactionSuccess);
        } else if (result.request().command() == PurchaseQuery::UpdateSuspendedPurchaseTransaction::COMMAND) {
            setTransactionId(-1);
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(SuspendTransactionSuccess);
        } else if (result.request().command() == PurchaseQuery::AddPurchaseTransaction::UNDO_COMMAND) {
            emit success(UndoSubmitTransactionSuccess);
        } else {
            emit success(UnknownSuccess);
        }
    } else {
        if (result.request().command() == PurchaseQuery::AddPurchaseTransaction::COMMAND) {
            if (result.request().params().value("suspended").toBool())
                emit error(SuspendTransactionError);
            else
                emit error(SubmitTransactionError);
        } else if (result.request().command() == PurchaseQuery::ViewPurchaseCart::COMMAND) {
            emit error(RetrieveTransactionError);
        } else if (result.request().command() == PurchaseQuery::UpdateSuspendedPurchaseTransaction::COMMAND) {
            emit error(SuspendTransactionError);
        } else {
            emit error(UnknownError);
        }
    }

    connect(this, &QMLPurchaseCartModel::transactionIdChanged, this, &QMLPurchaseCartModel::tryQuery);
}

void QMLPurchaseCartModel::addProduct(const QVariantMap &product)
{
    const int categoryId = product.value("product_category_id").toInt();
    const QString &category = product.value("category").toString();
    const int productId = product.value("product_id").toInt();
    const QString &productName = product.value("product").toString();
    const double availableQuantity = product.value("available_quantity", product.value("quantity").toDouble()).toDouble(); // TODO: Simplify
    const int unitId = product.value("unit_id").toInt();
    const QString &unit = product.value("unit").toString();
    const double costPrice = product.value("cost_price").toDouble();
    const double retailPrice = product.value("retail_price").toDouble();
    const double unitPrice = product.value("unit_price", retailPrice).toDouble();

    if (availableQuantity == 0.0)
        return;

    if (!containsProduct(productId)) {
        beginInsertRows(QModelIndex(), m_records.count(), m_records.count());

        QVariantMap record;
        record.insert("category_id", categoryId);
        record.insert("category", category);
        record.insert("product_id", productId);
        record.insert("product", productName);
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
        const int row = indexOfProduct(productId);
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

void QMLPurchaseCartModel::updateProduct(int productId, const QVariantMap &product)
{
    if (productId <= 0 || product.isEmpty())
        return;

    const int row = indexOfProduct(productId);
    QVariantMap record(m_records[row].toMap());
    const double oldQuantity = record.value("quantity").toDouble();
    const double newQuantity = product.value("quantity").toDouble();
    const double oldUnitPrice = record.value("unit_price").toDouble();
    const double oldCost = record.value("cost").toDouble();
    const double newUnitPrice = product.value("unit_price").toDouble();
    const double newCost = product.value("cost").toDouble();

    if (product.contains("quantity"))
        record.insert("quantity", newQuantity);
    if (product.contains("cost"))
        record.insert("cost", newCost);
    if (product.contains("unit_price"))
        record.insert("unit_price", newUnitPrice);
    m_records.replace(row, record);

    if (oldQuantity != newQuantity || oldUnitPrice != newUnitPrice || oldCost != newCost) {
        emit dataChanged(index(row), index(row));
        calculateTotal();
    }
}

void QMLPurchaseCartModel::setProductQuantity(int productId, double quantity)
{
    if (productId <= 0 || quantity <= 0.0)
        return;

    const int row = indexOfProduct(productId);
    QVariantMap record(m_records[row].toMap());
    const double oldQuantity = record.value("quantity").toDouble();
    const double unitPrice = record.value("unit_price").toDouble();

    record.insert("quantity", quantity);
    record.insert("cost", quantity * unitPrice);
    m_records.replace(row, record);

    if (oldQuantity != quantity) {
        emit dataChanged(index(row), index(row));
        calculateTotal();
    }
}

void QMLPurchaseCartModel::incrementProductQuantity(int productId, double quantity)
{
    if (quantity <= 0.0)
        return;

    const int row = indexOfProduct(productId);
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

void QMLPurchaseCartModel::decrementProductQuantity(int productId, double quantity)
{
    if (quantity <= 0.0)
        return;

    const int row = indexOfProduct(productId);
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

void QMLPurchaseCartModel::updateCanAcceptCash()
{
    const bool canAcceptCash = m_paymentModel->cashPaymentCount() < CASH_PAYMENT_LIMIT;
    if (m_canAcceptCash != canAcceptCash) {
        m_canAcceptCash = canAcceptCash;
        emit canAcceptCashChanged();
    }
}

void QMLPurchaseCartModel::updateCanAcceptCard()
{
    const bool canAcceptCard = m_paymentModel->cardPaymentCount() < CARD_PAYMENT_LIMIT;
    if (m_canAcceptCard != canAcceptCard) {
        m_canAcceptCard = canAcceptCard;
        emit canAcceptCardChanged();
    }
}

void QMLPurchaseCartModel::removeProduct(int productId)
{
    const int row = indexOfProduct(productId);
    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();

    calculateTotal();
}

bool QMLPurchaseCartModel::containsProduct(int productId)
{
    for (const QVariant &record : m_records)
        if (record.toMap().value("product_id").toInt() == productId)
            return true;

    return false;
}

int QMLPurchaseCartModel::indexOfProduct(int productId)
{
    if (productId <= 0)
        return -1;

    for (int i = 0; i < m_records.count(); ++i)
        if (m_records.at(i).toMap().value("product_id").toInt() == productId)
            return i;

    return -1;
}

void QMLPurchaseCartModel::calculateTotal()
{
    double totalCost = 0.0;
    for (const QVariant &record : m_records)
        totalCost += record.toMap().value("cost").toDouble();

    setTotalCost(totalCost);
    setBalance(m_totalCost - m_amountPaid);
}

void QMLPurchaseCartModel::calculateAmountPaid()
{
    double amountPaid = 0.0;
    for (const PurchasePayment &purchasePayment : m_purchasePayments)
        amountPaid += purchasePayment.amount;

    setAmountPaid(amountPaid);
    setBalance(m_totalCost - m_amountPaid);
}

void QMLPurchaseCartModel::setClientId(int clientId)
{
    if (m_clientId == clientId)
        return;

    m_clientId = clientId;
    emit clientIdChanged();
}
