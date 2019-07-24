#include "qmlpurchasecartmodel.h"
#include <QSqlField>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "models/purchasepaymentmodel.h"
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

QMLPurchaseCartModel::~QMLPurchaseCartModel()
{
    qDeleteAll(m_purchasePayments);
    m_purchasePayments.clear();
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

QHash<int, QByteArray> QMLPurchaseCartModel::roleNames() const
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

QList<PurchasePayment *> QMLPurchaseCartModel::payments() const
{
    return m_purchasePayments;
}

void QMLPurchaseCartModel::addPayment(double amount, QMLPurchaseCartModel::PaymentMethod method, const QString &note)
{
    if (amount <= 0.0)
        return;

    PurchasePayment *payment = new PurchasePayment{ amount, static_cast<PurchasePayment::PaymentMethod>(method), note };
    m_paymentModel->addPayment(payment);
    m_purchasePayments.append(payment);

    calculateAmountPaid();
}

void QMLPurchaseCartModel::removePayment(int index)
{
    if (index <= -1 || index >= m_purchasePayments.count())
        return;

    m_paymentModel->removePayment(index);
    delete m_purchasePayments.takeAt(index);

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
    qDebug() << Q_FUNC_INFO << "note? " << params.value("note").toString();
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
        qDeleteAll(m_purchasePayments);
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

void QMLPurchaseCartModel::addTransaction(const QVariantMap &transactionInfo)
{
    qDebug() << Q_FUNC_INFO << "note? " << transactionInfo.value("note").toString();

    if (!m_records.isEmpty()) {
        QVariantMap params;
        params.insert("transaction_id", m_transactionId);
        params.insert("customer_name", m_clientName);
        params.insert("client_id", transactionInfo.value("client_id"));
        params.insert("customer_phone_number", m_customerPhoneNumber);
        params.insert("total_cost", m_totalCost);
        params.insert("amount_paid", m_amountPaid);
        params.insert("balance", m_balance);
        params.insert("suspended", transactionInfo.value("suspended", false));
        params.insert("due_date", transactionInfo.value("due_date", QDateTime()));
        params.insert("action", transactionInfo.value("action"));
        params.insert("note", transactionInfo.value("note"));

        QVariantList payments;
        for (PurchasePayment *purchasePayment : m_purchasePayments) {
            QVariantMap payment;
            payment.insert("amount", purchasePayment->amount);
            payment.insert("note", purchasePayment->note);
            payment.insert("currency", "NGN");

            switch (purchasePayment->method) {
            case PurchasePayment::PaymentMethod::Cash:
                payment.insert("method", "cash");
                break;
            case PurchasePayment::PaymentMethod::DebitCard:
                payment.insert("method", "debit_card");
                break;
            case PurchasePayment::PaymentMethod::CreditCard:
                payment.insert("method", "credit_card");
                break;
            }
        }

        QVariantList items;
        for (const QVariant &record : m_records) {
            QVariantMap itemInfo;

            itemInfo.insert("category_id", record.toMap().value("category_id"));
            itemInfo.insert("item_id", record.toMap().value("item_id"));
            itemInfo.insert("quantity", record.toMap().value("quantity"));
            itemInfo.insert("unit_id", record.toMap().value("unit_id"));
            itemInfo.insert("retail_price", record.toMap().value("retail_price"));
            itemInfo.insert("unit_price", record.toMap().value("unit_price"));
            itemInfo.insert("cost", record.toMap().value("cost"));
            itemInfo.insert("amount_paid", record.toMap().value("amount_paid"));
            itemInfo.insert("note", record.toMap().value("note"));

            items.append(itemInfo);
        }

        params.insert("payments", payments);
        params.insert("items", items);
        params.insert("can_undo", true);

        setBusy(true);

        QueryRequest request(this);
        request.setCommand("add_purchase_transaction", params, QueryRequest::Purchase);

        emit executeRequest(request);
    } else {
        emit error(EmptyCartError);
    }
}

void QMLPurchaseCartModel::updateSuspendedTransaction(const QVariantMap &transactionInfo)
{
    if (!m_records.isEmpty()) {
        QVariantMap params;
        params.insert("transaction_id", m_transactionId);
        params.insert("customer_name", m_clientName);
        params.insert("client_id", m_clientId);
        params.insert("customer_phone_number", m_customerPhoneNumber);
        params.insert("total_cost", m_totalCost);
        params.insert("amount_paid", m_amountPaid);
        params.insert("balance", m_balance);
        params.insert("note", transactionInfo.value("note", QVariant(QVariant::String)));
        params.insert("suspended", true);

        QVariantList items;

        for (const QVariant &record : m_records) {
            QVariantMap itemInfo;

            itemInfo.insert("category_id", record.toMap().value("category_id"));
            itemInfo.insert("item_id", record.toMap().value("item_id"));
            itemInfo.insert("quantity", record.toMap().value("quantity"));
            itemInfo.insert("unit_id", record.toMap().value("unit_id"));
            itemInfo.insert("retail_price", record.toMap().value("retail_price"));
            itemInfo.insert("unit_price", record.toMap().value("unit_price"));
            itemInfo.insert("cost", record.toMap().value("cost"));
            itemInfo.insert("amount_paid", record.toMap().value("amount_paid"));
            itemInfo.insert("note", record.toMap().value("note"));

            items.append(itemInfo);
        }

        params.insert("items", items);

        setBusy(true);

        QueryRequest request(this);
        request.setCommand("update_suspended_purchase_transaction", params, QueryRequest::Purchase);

        emit executeRequest(request);
    } else {
        emit error(EmptyCartError);
    }
}

void QMLPurchaseCartModel::tryQuery()
{
    if (m_transactionId > -1) {
        setBusy(true);

        QueryRequest request(this);
        request.setCommand("view_purchase_cart", { { "transaction_id", m_transactionId } }, QueryRequest::Purchase);

        emit executeRequest(request);
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
        m_records = result.outcome().toMap().value("items").toList();
        calculateTotal();

        endResetModel();

        if (result.request().command() == "add_purchase_transaction") {
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
        } else if (result.request().command() == "view_purchase_cart") {
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(RetrieveTransactionSuccess);
        } else if (result.request().command() == "update_suspended_purchase_transaction") {
            setTransactionId(-1);
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(SuspendTransactionSuccess);
        } else if (result.request().command() == "undo_add_purchase_transaction") {
            emit success(UndoSubmitTransactionSuccess);
        } else {
            emit success(UnknownSuccess);
        }
    } else {
        if (result.request().command() == "add_purchase_transaction") {
            if (result.request().params().value("suspended").toBool())
                emit error(SuspendTransactionError);
            else
                emit error(SubmitTransactionError);
        } else if (result.request().command() == "view_purchase_cart") {
            emit error(RetrieveTransactionError);
        } else if (result.request().command() == "update_suspended_purchase_transaction") {
            emit error(SuspendTransactionError);
        } else {
            emit error(UnknownError);
        }
    }

    connect(this, &QMLPurchaseCartModel::transactionIdChanged, this, &QMLPurchaseCartModel::tryQuery);
}

void QMLPurchaseCartModel::addItem(const QVariantMap &itemInfo)
{
    const int categoryId = itemInfo.value("category_id").toInt();
    const QString &category = itemInfo.value("category").toString();
    const int itemId = itemInfo.value("item_id").toInt();
    const QString &item = itemInfo.value("item").toString();
    const double availableQuantity = itemInfo.value("available_quantity", itemInfo.value("quantity").toDouble()).toDouble(); // TODO: Simplify
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

void QMLPurchaseCartModel::updateItem(int itemId, const QVariantMap &itemInfo)
{
    if (itemId <= 0 || itemInfo.isEmpty())
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record(m_records[row].toMap());
    const double oldQuantity = record.value("quantity").toDouble();
    const double newQuantity = itemInfo.value("quantity").toDouble();
    const double oldUnitPrice = record.value("unit_price").toDouble();
    const double oldCost = record.value("cost").toDouble();
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

void QMLPurchaseCartModel::setItemQuantity(int itemId, double quantity)
{
    if (itemId <= 0 || quantity <= 0.0)
        return;

    const int row = indexOfItem(itemId);
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

void QMLPurchaseCartModel::incrementItemQuantity(int itemId, double quantity)
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

void QMLPurchaseCartModel::decrementItemQuantity(int itemId, double quantity)
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

void QMLPurchaseCartModel::removeItem(int itemId)
{
    const int row = indexOfItem(itemId);
    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();

    calculateTotal();
}

bool QMLPurchaseCartModel::containsItem(int itemId)
{
    for (const QVariant &record : m_records)
        if (record.toMap().value("item_id").toInt() == itemId)
            return true;

    return false;
}

int QMLPurchaseCartModel::indexOfItem(int itemId)
{
    if (itemId <= 0)
        return -1;

    for (int i = 0; i < m_records.count(); ++i)
        if (m_records.at(i).toMap().value("item_id").toInt() == itemId)
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
    for (PurchasePayment *purchasePayment : m_purchasePayments)
        amountPaid += purchasePayment->amount;

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
