#include "qmlsalecartmodel.h"
#include "database/databasethread.h"
#include "query/sales/addsaletransaction.h"
#include "query/sales/updatesuspendedsaletransaction.h"
#include "query/sales/viewsalecart.h"
#include "utility/sales/salecartproduct.h"
#include "utility/stock/productquantity.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

Q_LOGGING_CATEGORY(lcqmlsalecartmodel, "rrcore.qmlapi.qmlsalecartmodel")

QMLSaleCartModel::QMLSaleCartModel(QObject *parent) :
    QMLSaleCartModel(DatabaseThread::instance(), parent)
{}

QMLSaleCartModel::QMLSaleCartModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    connect(this, &QMLSaleCartModel::transactionIdChanged,
            this, &QMLSaleCartModel::tryQuery);
    connect(this, &QMLSaleCartModel::totalCostChanged,
            this, &QMLSaleCartModel::calculateBalance);
    connect(this, &QMLSaleCartModel::amountPaidChanged,
            this, &QMLSaleCartModel::calculateBalance);
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
        return m_transaction.products.at(index.row()).availableQuantity.toDouble();
    case QuantityRole:
        return m_transaction.products.at(index.row()).quantity.toDouble();
    case UnitRole:
        return m_transaction.products.at(index.row()).unit.unit;
    case UnitIdRole:
        return m_transaction.products.at(index.row()).unit.id;
    case CostPriceRole:
        return m_transaction.products.at(index.row()).monies.costPrice.toDouble();
    case RetailPriceRole:
        return m_transaction.products.at(index.row()).monies.retailPrice.toDouble();
    case UnitPriceRole:
        return m_transaction.products.at(index.row()).monies.unitPrice.toDouble();
    case CostRole:
        return m_transaction.products.at(index.row()).monies.cost.toDouble();
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
    return m_transaction.id > 0;
}

int QMLSaleCartModel::transactionId() const
{
    return m_transaction.id;
}

void QMLSaleCartModel::setTransactionId(int transactionId)
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

int QMLSaleCartModel::customerId() const
{
    return m_transaction.customer.id;
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
    return m_transaction.monies.totalCost.toDouble();
}

void QMLSaleCartModel::setTotalCost(double totalCost)
{
    if (m_transaction.monies.totalCost == Utility::Money(totalCost))
        return;

    m_transaction.monies.totalCost = Utility::Money(totalCost);
    emit totalCostChanged();
}

double QMLSaleCartModel::balance() const
{
    return m_transaction.monies.balance.toDouble();
}

void QMLSaleCartModel::setBalance(double balance)
{
    if (m_transaction.monies.balance == Utility::Money(balance))
        return;

    m_transaction.monies.balance = Utility::Money(balance);
    emit balanceChanged();
}

double QMLSaleCartModel::amountPaid() const
{
    return m_transaction.monies.amountPaid.toDouble();
}

void QMLSaleCartModel::setAmountPaid(double amountPaid)
{
    if (m_transaction.monies.amountPaid == Utility::Money(amountPaid))
        return;

    m_transaction.monies.amountPaid = Utility::Money(amountPaid);
    emit amountPaidChanged();
}

void QMLSaleCartModel::submitTransaction(const QVariantMap &transactionInfo)
{
    if (!m_transaction.monies.balance.isZero() && transactionInfo.value("due_date_time").isNull())
        emit error(ModelResult{ NoDueDateSetError });
    else
        addTransaction(transactionInfo);
}

void QMLSaleCartModel::suspendTransaction(const QVariantMap &params)
{
    if (!isExistingTransaction())
        addTransaction({ { "suspended", true },
                         { "action", "suspend" },
                         { "note", params.value("note") }
                       });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
}

void QMLSaleCartModel::clearAll()
{
    beginResetModel();
    setTransactionId(0);
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
        emit execute(new Query::Sales::AddSaleTransaction(m_transaction, this));
    } else {
        emit error(ModelResult{ EmptyCartError });
    }
}

void QMLSaleCartModel::updateSuspendedTransaction(const QVariantMap &addOns)
{
    if (!m_transaction.products.isEmpty()) {
        setBusy(true);
        m_transaction.note.note = addOns.value("note").toString();
        emit execute(new Query::Sales::UpdateSuspendedSaleTransaction(
                         m_transaction,
                         this));
    } else {
        emit error(ModelResult{ EmptyCartError });
    }
}

void QMLSaleCartModel::tryQuery()
{
    if (isExistingTransaction()) {
        setBusy(true);
        emit execute(new Query::Sales::ViewSaleCart(m_transaction.id, this));
    } else {
        beginResetModel();
        setCustomerName(QString());
        setCustomerPhoneNumber(QString());
        m_transaction.products.clear();
        endResetModel();
    }
}

bool QMLSaleCartModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLSaleCartModel::processResult(const QueryResult &result)
{
    setBusy(false);

    // Do not re-query if the transaction ID is changed in this method
    disconnect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);

    if (result.isSuccessful()) {
        beginResetModel();
        m_transaction.products = Utility::Sales::SaleCartProductList{ result.outcome().toMap().value("products").toList() };
        endResetModel();

        if (result.request().command() == Query::Sales::AddSaleTransaction::COMMAND) {
            if (result.request().params().value("suspended").toBool()) {
                setTransactionId(0);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setCustomerId(result.outcome().toMap().value("customer_id").toInt());
                emit success(ModelResult{ SuspendTransactionSuccess });
            } else {
                setTransactionId(0);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setCustomerId(result.outcome().toMap().value("customer_id").toInt());
                emit success(ModelResult{ SubmitTransactionSuccess });
            }
        } else if (result.request().command() == Query::Sales::AddSaleTransaction::UNDO_COMMAND) {
            setTransactionId(0);
            setCustomerName(QString());
            setCustomerPhoneNumber(QString());
            setCustomerId(result.outcome().toMap().value("customer_id").toInt());
            emit success(ModelResult{ UndoSubmitTransactionSuccess });
        } else if (result.request().command() == Query::Sales::ViewSaleCart::COMMAND) {
            setCustomerId(result.outcome().toMap().value("customer_id").toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(ModelResult{ RetrieveTransactionSuccess });
        } else if (result.request().command() == Query::Sales::UpdateSuspendedSaleTransaction::COMMAND) {
            setTransactionId(0);
            setCustomerId(result.outcome().toMap().value("customer_id").toInt());
            setCustomerName(result.outcome().toMap().value("customer_name").toString());
            setCustomerPhoneNumber(result.outcome().toMap().value("customer_phone_number").toString());
            emit success(ModelResult{ SuspendTransactionSuccess });
        } else {
            emit success();
        }
    } else {
        if (result.request().command() == Query::Sales::AddSaleTransaction::COMMAND) {
            if (result.request().params().value("suspended").toBool())
                emit error(ModelResult{ SuspendTransactionError });
            else
                emit error(ModelResult{ SubmitTransactionError });
        } else if (result.request().command() == Query::Sales::AddSaleTransaction::UNDO_COMMAND) {
            emit error(ModelResult{ UndoSubmitTransactionError });
        } else if (result.request().command() == Query::Sales::ViewSaleCart::COMMAND) {
            emit error(ModelResult{ RetrieveTransactionError });
        } else if (result.request().command() == Query::Sales::UpdateSuspendedSaleTransaction::COMMAND) {
            emit error(ModelResult{ SuspendTransactionError });
        } else {
            emit error();
        }
    }

    connect(this, &QMLSaleCartModel::transactionIdChanged, this, &QMLSaleCartModel::tryQuery);
}

void QMLSaleCartModel::undoLastCommit()
{
    QueryRequest request{ lastSuccessfulRequest() };
    QVariantMap params{ lastSuccessfulRequest().params() };
    if (lastSuccessfulRequest().command() == Query::Sales::AddSaleTransaction::COMMAND) {
        setBusy(true);
        auto query = new Query::Sales::AddSaleTransaction(request, this);
        query->undoOnNextExecution(true);
        emit execute(query);
    }
}

void QMLSaleCartModel::addProduct(const QVariantMap &product)
{
    Utility::Sales::SaleCartProduct newProduct{ product };

    if (newProduct.availableQuantity.isZero()) {
        qCWarning(lcqmlsalecartmodel) << Q_FUNC_INFO << "Available quantity is zero. Aborting...";
        return;
    }

    if (!m_transaction.products.contains(newProduct)) {
        beginInsertRows(QModelIndex(), m_transaction.products.count(), m_transaction.products.count());
        m_transaction.products.append(newProduct);
        calculateTotalCost();
        endInsertRows();
    } else {
        const int row = m_transaction.products.indexOf(newProduct);
        Utility::Sales::SaleCartProduct &existingProduct = m_transaction.products[row];
        const Utility::Stock::ProductQuantity &oldQuantity = existingProduct.quantity;
        const Utility::Stock::ProductQuantity &newQuantity = oldQuantity + newProduct.quantity;

        existingProduct.quantity = qMin(newQuantity, existingProduct.availableQuantity);
        existingProduct.monies.cost = Utility::Money(existingProduct.quantity.toDouble() * existingProduct.monies.unitPrice.toDouble());

        calculateTotalCost();
        emit dataChanged(index(row), index(row));
    }
}

void QMLSaleCartModel::updateProduct(int productId, const QVariantMap &product)
{
    if (productId <= 0 || product.isEmpty())
        return;

    const auto updatedProduct = Utility::Sales::SaleCartProduct{ product };
    const int row = m_transaction.products.indexOf(updatedProduct);
    Utility::Sales::SaleCartProduct &existingProduct{ m_transaction.products[row] };
    const Utility::Stock::ProductQuantity &oldQuantity = existingProduct.quantity;
    const Utility::Stock::ProductQuantity &availableQuantity = existingProduct.availableQuantity;
    const Utility::Money &oldUnitPrice = existingProduct.monies.unitPrice;
    const Utility::Money &oldCost = existingProduct.monies.cost;
    const Utility::Stock::ProductQuantity &newQuantity = qMin(updatedProduct.quantity, availableQuantity);
    const Utility::Money &newUnitPrice = updatedProduct.monies.unitPrice;
    const Utility::Money &newCost = updatedProduct.monies.cost;

    if (oldQuantity != newQuantity || oldUnitPrice != newUnitPrice || oldCost != newCost) {
        if (product.contains("quantity"))
            existingProduct.quantity = newQuantity;
        if (product.contains("cost"))
            existingProduct.monies.cost = newCost;
        if (product.contains("unit_price"))
            existingProduct.monies.unitPrice = newUnitPrice;

        emit dataChanged(index(row), index(row));
    }
}

void QMLSaleCartModel::setProductQuantity(int productId, double quantity)
{
    if (productId <= 0 || quantity <= 0.0)
        return;

    const int row = m_transaction.products.indexOf(Utility::Sales::SaleCartProduct{ productId });
    Utility::Sales::SaleCartProduct &existingProduct{ m_transaction.products[row] };
    const Utility::Stock::ProductQuantity &oldQuantity = existingProduct.quantity;
    const Utility::Stock::ProductQuantity &availableQuantity = existingProduct.availableQuantity;
    const Utility::Stock::ProductQuantity &newQuantity = qMin(Utility::Stock::ProductQuantity(quantity),
                                                            availableQuantity);
    const Utility::Money &unitPrice = existingProduct.monies.unitPrice;

    if (oldQuantity != newQuantity) {
        existingProduct.quantity = newQuantity;
        existingProduct.monies.cost = Utility::Money{ newQuantity.toDouble() * unitPrice.toDouble() };
        emit dataChanged(index(row), index(row));
    }
}

void QMLSaleCartModel::removeProduct(int productId)
{
    const int row = m_transaction.products.indexOf(Utility::Sales::SaleCartProduct{ productId });
    beginRemoveRows(QModelIndex(), row, row);
    m_transaction.products.removeAt(row);
    endRemoveRows();
}

void QMLSaleCartModel::calculateTotalCost()
{
    Utility::Money totalCost;
    for (const auto &product : m_transaction.products)
        totalCost += product.monies.cost;

    setTotalCost(totalCost.toDouble());
}

void QMLSaleCartModel::calculateBalance()
{
    setBalance(m_transaction.monies.totalCost.toDouble() - m_transaction.monies.amountPaid.toDouble());
}

void QMLSaleCartModel::setCustomerId(int customerId)
{
    if (m_transaction.customer.id == customerId)
        return;

    m_transaction.customer.id = customerId;
    emit customerIdChanged();
}
