#include "qmlsalecartmodel.h"
#include <QSqlField>

#include "rr-core/database/queryrequest.h"
#include "rr-core/database/queryresult.h"

QMLSaleCartModel::QMLSaleCartModel(QObject *parent) :
    m_transactionId(-1),
    m_clientId(-1),
    m_totalCost(0.0),
    m_amountPaid(0.0),
    m_lastTransactionId(-1),
    AbstractVisualListModel(parent)
{
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
        break;
    case CategoryRole:
        return m_records.at(index.row()).toMap().value("category").toString();
        break;
    case ItemIdRole:
        return m_records.at(index.row()).toMap().value("item_id").toInt();
        break;
    case ItemRole:
        return m_records.at(index.row()).toMap().value("item").toString();
        break;
        break;
    case AvailableQuantityRole:
        return m_records.at(index.row()).toMap().value("available_quantity").toDouble();
        break;
    case QuantityRole:
        return m_records.at(index.row()).toMap().value("quantity").toDouble();
        break;
    case UnitRole:
        return m_records.at(index.row()).toMap().value("unit").toString();
        break;
    case UnitIdRole:
        return m_records.at(index.row()).toMap().value("unit_id").toInt();
        break;
    case CostPriceRole:
        return m_records.at(index.row()).toMap().value("cost_price").toDouble();
        break;
    case RetailPriceRole:
        return m_records.at(index.row()).toMap().value("retail_price").toDouble();
        break;
    case UnitPriceRole:
        return m_records.at(index.row()).toMap().value("unit_price").toDouble();
        break;
    case CostRole:
        return m_records.at(index.row()).toMap().value("cost").toDouble();
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleCartModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(CategoryIdRole, "category_id");
    roles.insert(CategoryRole, "category");
    roles.insert(ItemIdRole, "item_id");
    roles.insert(ItemRole, "item");
    roles.insert(AvailableQuantityRole, "available_quantity");
    roles.insert(QuantityRole, "quantity");
    roles.insert(UnitRole, "unit");
    roles.insert(UnitIdRole, "unit_id");
    roles.insert(CostPriceRole, "cost_price");
    roles.insert(RetailPriceRole, "retail_price");
    roles.insert(UnitPriceRole, "unit_price");
    roles.insert(CostRole, "cost");

    return roles;
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

void QMLSaleCartModel::setAmountPaid(double amountPaid)
{
    if (m_amountPaid == amountPaid)
        return;

    m_amountPaid = amountPaid;
    emit amountPaidChanged();
}

QDateTime QMLSaleCartModel::date() const
{
    return m_dateTime;
}

void QMLSaleCartModel::setDate(const QDateTime &dateTime)
{
    if (m_dateTime == dateTime)
        return;

    m_dateTime = dateTime;
    emit dateChanged();
}

void QMLSaleCartModel::submitTransaction(const QVariantMap &paymentInfo)
{
    qDebug() << "Submit transaction: " << m_records;

    addTransaction(paymentInfo);
}

void QMLSaleCartModel::suspendTransaction(const QVariantMap &params)
{
    qDebug() << "Suspend transaction: " << m_records;

    if (m_transactionId == -1)
        addTransaction( {{ "suspended", true }, { "note", params.value("note") } });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
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

void QMLSaleCartModel::addTransaction(const QVariantMap &paymentInfo)
{
    QVariantMap params;
    params.insert("transaction_id", m_transactionId);
    params.insert("customer_name", m_customerName);
    params.insert("customer_id", m_clientId);
    params.insert("customer_phone_number", m_customerPhoneNumber);
    params.insert("total_cost", m_totalCost);
    params.insert("amount_paid", paymentInfo.value("amount_paid", 0.0));
    params.insert("suspended", paymentInfo.value("suspended", false));
    params.insert("balance", paymentInfo.value("balance", 0.0));
    params.insert("due_date", paymentInfo.value("due_date", QDateTime()));
    params.insert("overlook_balance", paymentInfo.value("overlook_balance", false));
    params.insert("note", paymentInfo.value("note", QString()));

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

    QueryRequest request(this);
    request.setCommand("add_sale_transaction", params, QueryRequest::Sales);

    emit executeRequest(request);
}

void QMLSaleCartModel::updateSuspendedTransaction(const QVariantMap &paymentInfo)
{
    QVariantMap params;
    params.insert("transaction_id", m_transactionId);
    params.insert("customer_name", m_customerName);
    params.insert("customer_id", m_clientId);
    params.insert("customer_phone_number", m_customerPhoneNumber);
    params.insert("total_cost", m_totalCost);
    params.insert("balance", paymentInfo.value("balance", 0.0));
    params.insert("note", paymentInfo.value("note", QString()));

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

    QueryRequest request(this);
    request.setCommand("update_suspended_sale_transaction", params, QueryRequest::Sales);

    emit executeRequest(request);
}

void QMLSaleCartModel::tryQuery()
{
    if (m_transactionId > -1) {
        setBusy(true);

        QueryRequest request(this);
        request.setCommand("view_sale_cart", { { "transaction_id", m_transactionId } }, QueryRequest::Sales);

        emit executeRequest(request);
    } else {
        beginResetModel();
        setCustomerName(QString());
        setCustomerPhoneNumber(QString());
        m_records.clear();
        calculateTotals();
        endResetModel();
    }
}

void QMLSaleCartModel::processResult(const QueryResult &result)
{
    if (this != result.request().parent())
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        m_records = result.outcome().toMap().value("items").toList();
        calculateTotals();
        endResetModel();

        if (result.request().command() == "add_sale_transaction" && result.request().params()["suspended"].toBool()) {
            setTransactionId(-1);
            setCustomerName(QString());
            setCustomerPhoneNumber(QString());
            emit success(TransactionSuspended);
        } else if (result.request().command() == "add_sale_transaction") {
            setTransactionId(-1);
            setCustomerName(QString());
            setCustomerPhoneNumber(QString());
            m_lastTransactionId = result.outcome().toMap().value("transaction_id").toInt();
            m_lastClientId = result.outcome().toMap().value("client_id").toInt();
            emit success(TransactionSubmitted);
        } else if (result.request().command() == "view_sale_cart") {
            setTransactionId(result.outcome().toMap().value("transaction_id").toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(TransactionRetrieved);
        } else if (result.request().command() == "update_suspended_sale_transaction") {
            setTransactionId(result.outcome().toMap().value("transaction_id").toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(TransactionSuspended);
        }
    } else {
        emit error(FailedToSuspend);
    }
}

void QMLSaleCartModel::addItem(const QVariantMap &itemInfo)
{
    const int categoryId = itemInfo.value("category_id").toInt();
    const QString category = itemInfo.value("category").toString();
    const int itemId = itemInfo.value("item_id").toInt();
    const QString item = itemInfo.value("item").toString();
    const double availableQuantity = itemInfo.value("quantity").toDouble();
    const int unitId = itemInfo.value("unit_id").toInt();
    const QString unit = itemInfo.value("unit").toString();
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
        QVariantMap record = m_records.at(row).toMap();
        const double oldQuantity = record.value("quantity").toDouble();
        const double newQuantity = oldQuantity + 1;

        record.insert("quantity", qMin(newQuantity, availableQuantity));
        record.insert("cost", record.value("quantity").toDouble() * unitPrice);
        m_records.replace(row, record);

        emit dataChanged(index(row), index(row));
    }

    calculateTotals();
}

void QMLSaleCartModel::setItemQuantity(int itemId, double quantity)
{
    if (itemId <= 0 || quantity < 0.0)
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record = m_records[row].toMap();
    const double oldQuantity = record.value("quantity").toDouble();
    const double availableQuantity = record.value("available_quantity").toDouble();
    const double newQuantity = qMin(quantity, availableQuantity);
    const double unitPrice = record.value("unit_price").toDouble();

    record.insert("quantity", newQuantity);
    record.insert("cost", newQuantity * unitPrice);
    m_records.replace(row, record);

    if (oldQuantity != quantity) {
        emit dataChanged(index(row), index(row));
        calculateTotals();
    }
}

void QMLSaleCartModel::incrementItemQuantity(int itemId, double quantity)
{
    if (quantity <= 0.0)
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record = m_records[row].toMap();
    const double oldQuantity = record.value("quantity").toDouble();
    const double availableQuantity = record.value("available_quantity").toDouble();
    const double newQuantity = qMin(oldQuantity + quantity, availableQuantity);
    const double unitPrice = record.value("unit_price").toDouble();

    record.insert("quantity", newQuantity);
    record.insert("cost", newQuantity * unitPrice);
    m_records.replace(row, record);

    emit dataChanged(index(row), index(row));

    calculateTotals();
}

void QMLSaleCartModel::decrementItemQuantity(int itemId, double quantity)
{
    if (quantity <= 0.0)
        return;

    const int row = indexOfItem(itemId);
    QVariantMap record = m_records[row].toMap();
    const double oldQuantity = record.value("quantity").toDouble();
    const double newQuantity = qMax(oldQuantity - quantity, 0.0);
    const double unitPrice = record.value("unit_price").toDouble();

    record.insert("quantity", newQuantity);
    record.insert("cost", newQuantity * unitPrice);
    m_records.replace(row, record);

    emit dataChanged(index(row), index(row));

    calculateTotals();
}

void QMLSaleCartModel::removeItem(int itemId)
{
    const int row = indexOfItem(itemId);
    beginRemoveRows(QModelIndex(), row, row);
    m_records.removeAt(row);
    endRemoveRows();

    calculateTotals();
}

void QMLSaleCartModel::undoLastCommit()
{
    setBusy(true);
    QVariantMap params;

    params.insert("transaction_id", m_lastTransactionId);
    params.insert("client_id", m_lastClientId);

    QueryRequest request(this);
    request.setCommand("undo_last_sale_transaction", params, QueryRequest::Sales);

    emit executeRequest(request);
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
    for (int i = 0; i < m_records.count(); ++i)
        if (m_records.at(i).toMap().value("item_id").toInt() == itemId)
            return i;

    return -1;
}

void QMLSaleCartModel::calculateTotals()
{
    double total = 0.0;
    for (const QVariant &record : m_records)
        total += record.toMap().value("cost").toDouble();

    setTotalCost(total);
}
