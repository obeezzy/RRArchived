#include "qmlpurchasecartmodel.h"
#include "database/databasethread.h"
#include "query/purchase/addpurchasetransaction.h"
#include "query/purchase/updatesuspendedpurchasetransaction.h"
#include "query/purchase/viewpurchasecart.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

QMLPurchaseCartModel::QMLPurchaseCartModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseCartModel::QMLPurchaseCartModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    connect(this, &QMLPurchaseCartModel::transactionIdChanged,
            this, &QMLPurchaseCartModel::tryQuery);
}

int QMLPurchaseCartModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_transaction.products.count();
}

QVariant QMLPurchaseCartModel::data(const QModelIndex &index, int role) const
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

int QMLPurchaseCartModel::transactionId() const
{
    return m_transaction.id;
}

void QMLPurchaseCartModel::setTransactionId(int transactionId)
{
    if (m_transaction.id == transactionId)
        return;

    m_transaction.id = transactionId;
    emit transactionIdChanged();
}

QString QMLPurchaseCartModel::customerName() const
{
    return m_transaction.vendor.client.preferredName;
}

void QMLPurchaseCartModel::setCustomerName(const QString &customerName)
{
    if (m_transaction.vendor.client.preferredName == customerName)
        return;

    m_transaction.vendor.client.preferredName = customerName;
    emit customerNameChanged();
}

QString QMLPurchaseCartModel::customerPhoneNumber() const
{
    return m_transaction.vendor.client.preferredName;
}

void QMLPurchaseCartModel::setCustomerPhoneNumber(const QString &customerPhoneNumber)
{
    if (m_transaction.vendor.client.preferredName == customerPhoneNumber)
        return;

    m_transaction.vendor.client.preferredName = customerPhoneNumber;
    emit customerPhoneNumberChanged();
}

int QMLPurchaseCartModel::vendorId() const
{
    return m_transaction.vendor.id;
}

QString QMLPurchaseCartModel::note() const
{
    return m_transaction.note.note;
}

void QMLPurchaseCartModel::setNote(const QString &note)
{
    if (m_transaction.note.note == note)
        return;

    m_transaction.note.note = note;
    emit noteChanged();
}

double QMLPurchaseCartModel::totalCost() const
{
    return m_transaction.monies.totalCost.toDouble();
}

void QMLPurchaseCartModel::setTotalCost(double totalCost)
{
    if (m_transaction.monies.totalCost == Utility::Money(totalCost))
        return;

    m_transaction.monies.totalCost = Utility::Money(totalCost);
    emit totalCostChanged();
}

void QMLPurchaseCartModel::setBalance(double balance)
{
    if (m_transaction.monies.balance == Utility::Money(balance))
        return;

    m_transaction.monies.balance = Utility::Money(balance);
    emit balanceChanged();
}

double QMLPurchaseCartModel::balance() const
{
    return m_transaction.monies.balance.toDouble();
}

void QMLPurchaseCartModel::submitTransaction(const QVariantMap &addOns)
{
    if (m_transaction.monies.balance.isZero() && addOns.value("due_date_time").isNull())
        emit error(ModelResult{ NoDueDateSetError });
    else
        addTransaction(addOns);
}

void QMLPurchaseCartModel::suspendTransaction(const QVariantMap &params)
{
    if (!m_transaction.id)
        addTransaction( { { "suspended", true },
                          { "action", "suspend" },
                          { "note", params.value("note") } });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
}

void QMLPurchaseCartModel::clearAll()
{
    beginResetModel();
    setTransactionId(0);
    setCustomerName(QString());
    setCustomerPhoneNumber(QString());
    m_transaction.payments.clear();
    m_transaction.products.clear();
    endResetModel();
}

QString QMLPurchaseCartModel::toPrintableFormat() const
{
    QJsonObject rootObject;
    rootObject.insert("client_name", m_transaction.vendor.client.preferredName);
    rootObject.insert("phone_number", m_transaction.vendor.client.phoneNumber);
    rootObject.insert("group", "sales");
    rootObject.insert("transactions", QJsonArray::fromVariantList(QVariantList()));

    return QJsonDocument(rootObject).toJson();
}

void QMLPurchaseCartModel::addTransaction(const QVariantMap &transaction)
{
    Q_UNUSED(transaction)
    if (!m_transaction.products.isEmpty()) {
        setBusy(true);
        emit execute(new Query::Purchase::AddPurchaseTransaction(m_transaction, this));
    } else {
        emit error(ModelResult{ EmptyCartError });
    }
}

void QMLPurchaseCartModel::updateSuspendedTransaction(const QVariantMap &transactionAddOns)
{
    const auto &note = transactionAddOns.value("note").toString();
    m_transaction.flags.setFlag(Utility::RecordGroup::Suspended);
    m_transaction.note = Utility::Note{ note };

    setBusy(true);

    if (!m_transaction.products.isEmpty())
        emit execute(new Query::Purchase::UpdateSuspendedPurchaseTransaction(m_transaction, this));
    else
        emit error(ModelResult{ EmptyCartError });
}

void QMLPurchaseCartModel::tryQuery()
{
    if (m_transaction.id > 0) {
        setBusy(true);
        emit execute(new Query::Purchase::ViewPurchaseCart(m_transaction.id, this));
    } else {
        beginResetModel();
        setCustomerName(QString());
        setCustomerPhoneNumber(QString());
        m_transaction.products.clear();
        calculateTotalCost();
        endResetModel();
    }
}

bool QMLPurchaseCartModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLPurchaseCartModel::processResult(const QueryResult &result)
{
    setBusy(false);

    // Do not re-query if the transaction ID is changed in this method
    disconnect(this, &QMLPurchaseCartModel::transactionIdChanged, this, &QMLPurchaseCartModel::tryQuery);

    if (result.isSuccessful()) {
        beginResetModel();

        m_transaction.products = Utility::Purchase::PurchaseCartProductList{ result.outcome().toMap().value("products").toList() };
        calculateTotalCost();
        endResetModel();

        if (result.request().command() == Query::Purchase::AddPurchaseTransaction::COMMAND) {
            if (result.request().params().value("suspended").toBool()) {
                const auto vendor = Utility::Vendor{ result.outcome().toMap() };
                setTransactionId(0);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setVendorId(vendor.id);
                emit success(ModelResult{ SuspendTransactionSuccess });
            } else {
                const auto vendor = Utility::Vendor{ result.outcome().toMap() };
                setTransactionId(0);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setVendorId(vendor.id);
                emit success(ModelResult{ SubmitTransactionSuccess });
            }
        } else if (result.request().command() == Query::Purchase::ViewPurchaseCart::COMMAND) {
            const auto vendor = Utility::Vendor{ result.outcome().toMap() };
            setVendorId(vendor.client.id);
            setCustomerName(vendor.client.preferredName);
            setCustomerPhoneNumber(vendor.client.phoneNumber);
            emit success(ModelResult{ RetrieveTransactionSuccess });
        } else if (result.request().command() == Query::Purchase::UpdateSuspendedPurchaseTransaction::COMMAND) {
            const auto vendor = Utility::Vendor{ result.outcome().toMap() };
            setTransactionId(0);
            setVendorId(vendor.id);
            setCustomerName(vendor.client.preferredName);
            setCustomerPhoneNumber(vendor.client.phoneNumber);
            emit success(ModelResult{ SuspendTransactionSuccess });
        } else if (result.request().command() == Query::Purchase::AddPurchaseTransaction::UNDO_COMMAND) {
            emit success(ModelResult{ UndoSubmitTransactionSuccess });
        } else {
            emit success(ModelResult{ UnknownSuccess });
        }
    } else {
        if (result.request().command() == Query::Purchase::AddPurchaseTransaction::COMMAND) {
            const auto transaction = Utility::Purchase::PurchaseTransaction{ result.request().params() };
            if (transaction.flags.testFlag(Utility::RecordGroup::Suspended))
                emit error(ModelResult{ SuspendTransactionError });
            else
                emit error(ModelResult{ SubmitTransactionError });
        } else if (result.request().command() == Query::Purchase::ViewPurchaseCart::COMMAND) {
            emit error(ModelResult{ RetrieveTransactionError });
        } else if (result.request().command() == Query::Purchase::UpdateSuspendedPurchaseTransaction::COMMAND) {
            emit error(ModelResult{ SuspendTransactionError });
        } else {
            emit error();
        }
    }

    connect(this, &QMLPurchaseCartModel::transactionIdChanged, this, &QMLPurchaseCartModel::tryQuery);
}

void QMLPurchaseCartModel::addProduct(const QVariantMap &product)
{
    const auto productId = product.value("product_id").toInt();
    const auto &availableQuantity = Utility::Stock::ProductQuantity{ qMin(1.0,
                                                                        product.value("available_quantity").toDouble())};

    if (availableQuantity.isZero())
        return;

    if (!m_transaction.products.contains(Utility::Purchase::PurchaseCartProduct{ productId })) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        const auto newProduct = Utility::Purchase::PurchaseCartProduct{ product };
        m_transaction.products.append(newProduct);
        endInsertRows();
    } else {
        const auto row = m_transaction.products.indexOf(Utility::Purchase::PurchaseCartProduct{ productId });
        auto &existingProduct =  m_transaction.products[row];
        const auto &oldQuantity = existingProduct.quantity;
        const auto &newQuantity = oldQuantity + Utility::Stock::ProductQuantity(1.0);

        existingProduct.quantity = qMin(newQuantity, availableQuantity);
        existingProduct.monies.cost = Utility::Money{ existingProduct.quantity.toDouble() * existingProduct.monies.unitPrice.toDouble() };

        emit dataChanged(index(row), index(row));
    }

    calculateTotalCost();
}

void QMLPurchaseCartModel::updateProduct(int productId,
                                         const QVariantMap &product)
{
    if (productId <= 0 || product.isEmpty())
        return;

    const auto row = m_transaction.products.indexOf(Utility::Purchase::PurchaseCartProduct{ productId });
    const auto &newProduct = Utility::Purchase::PurchaseCartProduct{ product };
    auto &existingProduct = m_transaction.products[row];
    const auto &oldQuantity = existingProduct.quantity;
    const auto &newQuantity = newProduct.quantity;
    const auto &oldUnitPrice = existingProduct.monies.unitPrice;
    const auto &oldCost = existingProduct.monies.cost;
    const auto &newUnitPrice = newProduct.monies.unitPrice;
    const auto &newCost = newProduct.monies.cost;

    if (oldQuantity != newQuantity || oldUnitPrice != newUnitPrice || oldCost != newCost) {
        if (product.contains("quantity"))
            existingProduct.quantity = newQuantity;
        if (product.contains("cost"))
            existingProduct.monies.cost = newCost;
        if (product.contains("unit_price"))
            existingProduct.monies.unitPrice = newUnitPrice;

        emit dataChanged(index(row), index(row));
        calculateTotalCost();
    }
}

void QMLPurchaseCartModel::setProductQuantity(int productId,
                                              double quantity)
{
    if (productId <= 0 || quantity <= 0.0)
        return;

    const auto row = m_transaction.products.indexOf(Utility::Purchase::PurchaseCartProduct{ productId });
    auto &existingProduct =  m_transaction.products[row];
    const auto &oldQuantity = existingProduct.quantity;
    const auto &unitPrice = existingProduct.monies.unitPrice;

    if (oldQuantity != Utility::Stock::ProductQuantity(quantity)) {
        existingProduct.quantity = Utility::Stock::ProductQuantity(quantity);
        existingProduct.monies.cost = Utility::Money{ quantity * unitPrice.toDouble() };

        emit dataChanged(index(row), index(row));
        calculateTotalCost();
    }
}

void QMLPurchaseCartModel::removeProduct(int productId)
{
    const auto row = m_transaction.products.indexOf(Utility::Purchase::PurchaseCartProduct{ productId });
    beginRemoveRows(QModelIndex(), row, row);
    m_transaction.products.removeAt(row);
    endRemoveRows();

    calculateTotalCost();
}

void QMLPurchaseCartModel::calculateTotalCost()
{
    Utility::Money totalCost;
    for (const auto &product : m_transaction.products)
        totalCost += product.monies.cost;
    setTotalCost(totalCost.toDouble());
}

void QMLPurchaseCartModel::setVendorId(int vendorId)
{
    if (m_transaction.vendor.id == vendorId)
        return;

    m_transaction.vendor.id = vendorId;
    emit vendorIdChanged();
}
