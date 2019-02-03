#include "qmlsalecartmodel.h"
#include <QSqlField>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "models/salepaymentmodel.h"
#include "utility/saleutils.h"

const int CASH_PAYMENT_LIMIT = 1;
const int CARD_PAYMENT_LIMIT = 2;

QMLSaleCartModel::QMLSaleCartModel(QObject *parent) :
    AbstractVisualListModel(parent),
    m_transactionId(-1),
    m_customerName(QString()),
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
    m_paymentModel = new SalePaymentModel(this);

    connect (m_paymentModel, &SalePaymentModel::cashPaymentCountChanged, this, &QMLSaleCartModel::updateCanAcceptCash);
    connect (m_paymentModel, &SalePaymentModel::cardPaymentCountChanged, this, &QMLSaleCartModel::updateCanAcceptCard);

    connect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);
}

QMLSaleCartModel::QMLSaleCartModel(DatabaseThread &thread) :
    AbstractVisualListModel(thread),
    m_transactionId(-1),
    m_customerName(QString()),
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
    m_paymentModel = new SalePaymentModel(this);

    connect (m_paymentModel, &SalePaymentModel::cashPaymentCountChanged, this, &QMLSaleCartModel::updateCanAcceptCash);
    connect (m_paymentModel, &SalePaymentModel::cardPaymentCountChanged, this, &QMLSaleCartModel::updateCanAcceptCard);

    connect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);
}

QMLSaleCartModel::~QMLSaleCartModel()
{
    qDeleteAll(m_salePayments);
    m_salePayments.clear();
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

QList<SalePayment *> QMLSaleCartModel::payments() const
{
    return m_salePayments;
}

void QMLSaleCartModel::addPayment(double amount, QMLSaleCartModel::PaymentMethod method, const QString &note)
{
    if (amount <= 0.0)
        return;

    SalePayment *payment = new SalePayment{ amount, static_cast<SalePayment::PaymentMethod>(method), note };
    m_paymentModel->addPayment(payment);
    m_salePayments.append(payment);

    calculateAmountPaid();
}

void QMLSaleCartModel::removePayment(int index)
{
    if (index <= -1 || index >= m_salePayments.count())
        return;

    m_paymentModel->removePayment(index);
    delete m_salePayments.takeAt(index);

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
    qDebug() << Q_FUNC_INFO << "note? " << params.value("note").toString();
    if (m_transactionId == -1)
        addTransaction( { { "suspended", true },
                          { "action", "suspend" },
                          { "note", params.value("note") } });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
}

void QMLSaleCartModel::clearPayments()
{
    if (!m_salePayments.isEmpty()) {
        m_paymentModel->clearPayments();
        qDeleteAll(m_salePayments);
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
    rootObject.insert("group", "sales");
    rootObject.insert("records", QJsonArray::fromVariantList(m_records));

    return QJsonDocument(rootObject).toJson();
}

void QMLSaleCartModel::addTransaction(const QVariantMap &transactionInfo)
{
    qDebug() << Q_FUNC_INFO << "note? " << transactionInfo.value("note").toString();

    if (!m_records.isEmpty()) {
        QVariantMap params;
        params.insert("transaction_id", m_transactionId);
        params.insert("customer_name", m_customerName);
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
        for (SalePayment *salePayment : m_salePayments) {
            QVariantMap payment;
            payment.insert("amount", salePayment->amount);
            payment.insert("note", salePayment->note);
            payment.insert("currency", "NGN");

            switch (salePayment->method) {
            case SalePayment::PaymentMethod::Cash:
                payment.insert("method", "cash");
                break;
            case SalePayment::PaymentMethod::DebitCard:
                payment.insert("method", "debit_card");
                break;
            case SalePayment::PaymentMethod::CreditCard:
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
        request.setCommand("add_sale_transaction", params, QueryRequest::Sales);

        emit executeRequest(request);
    } else {
        emit error(EmptyCartError);
    }
}

void QMLSaleCartModel::updateSuspendedTransaction(const QVariantMap &transactionInfo)
{
    if (!m_records.isEmpty()) {
        QVariantMap params;
        params.insert("transaction_id", m_transactionId);
        params.insert("customer_name", m_customerName);
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
        request.setCommand("update_suspended_sale_transaction", params, QueryRequest::Sales);

        emit executeRequest(request);
    } else {
        emit error(EmptyCartError);
    }
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

        if (result.request().command() == "add_sale_transaction") {
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
        } else if (result.request().command() == "view_sale_cart") {
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(RetrieveTransactionSuccess);
        } else if (result.request().command() == "update_suspended_sale_transaction") {
            setTransactionId(-1);
            setClientId(result.outcome().toMap().value("client_id", -1).toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(SuspendTransactionSuccess);
        } else {
            emit success(UnknownSuccess);
        }
    } else {
        if (result.request().command() == "add_sale_transaction") {
            if (result.request().params().value("suspended").toBool())
                emit error(SuspendTransactionError);
            else
                emit error(SubmitTransactionError);
        } else if (result.request().command() == "view_sale_cart") {
            emit error(RetrieveTransactionError);
        } else if (result.request().command() == "update_suspended_sale_transaction") {
            emit error(SuspendTransactionError);
        } else {
            emit error(UnknownError);
        }
    }

    connect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);
}

void QMLSaleCartModel::addItem(const QVariantMap &itemInfo)
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
    for (SalePayment *salePayment : m_salePayments)
        amountPaid += salePayment->amount;

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
