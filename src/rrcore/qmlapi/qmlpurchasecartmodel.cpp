#include "qmlpurchasecartmodel.h"
#include "database/databasethread.h"
#include "models/purchasepaymentmodel.h"
#include "queryexecutors/purchase.h"
#include "utility/purchaseutils.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

const int CASH_PAYMENT_LIMIT = 1;
const int CARD_PAYMENT_LIMIT = 2;

QMLPurchaseCartModel::QMLPurchaseCartModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLPurchaseCartModel::QMLPurchaseCartModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    m_paymentModel = new PurchasePaymentModel(this);

    connect(m_paymentModel, &PurchasePaymentModel::cashPaymentCountChanged,
            this, &QMLPurchaseCartModel::updateCanAcceptCash);
    connect(m_paymentModel, &PurchasePaymentModel::cardPaymentCountChanged,
            this, &QMLPurchaseCartModel::updateCanAcceptCard);
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
    return m_transaction.id;
}

void QMLPurchaseCartModel::setTransactionId(qint64 transactionId)
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

int QMLPurchaseCartModel::clientId() const
{
    return m_transaction.vendor.client.id;
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
    return m_transaction.totalCost;
}

void QMLPurchaseCartModel::setTotalCost(double totalCost)
{
    if (m_transaction.totalCost == totalCost)
        return;

    m_transaction.totalCost = totalCost;
    emit totalCostChanged();
}

double QMLPurchaseCartModel::amountPaid() const
{
    return m_transaction.amountPaid;
}

double QMLPurchaseCartModel::balance() const
{
    return m_transaction.balance;
}

bool QMLPurchaseCartModel::canAcceptCash() const
{
    return m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCash);
}

bool QMLPurchaseCartModel::canAcceptCard() const
{
    return m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCard);
}

PurchasePaymentModel *QMLPurchaseCartModel::paymentModel() const
{
    return m_paymentModel;
}

void QMLPurchaseCartModel::setAmountPaid(double amountPaid)
{
    if (m_transaction.amountPaid == amountPaid)
        return;

    m_transaction.amountPaid = amountPaid;
    emit amountPaidChanged();
}

void QMLPurchaseCartModel::setBalance(double balance)
{
    if (m_transaction.balance == balance)
        return;

    m_transaction.balance = balance;
    emit balanceChanged();
}

Utility::PurchasePaymentList QMLPurchaseCartModel::payments() const
{
    return m_transaction.payments;
}

void QMLPurchaseCartModel::addPayment(qreal amount,
                                      PaymentMethod method,
                                      const QString &note)
{
    if (amount <= 0.0)
        return;

    Utility::PurchasePayment payment {
        amount,
                static_cast<Utility::PaymentMethod>(method),
                Utility::Note{ note }
    };
    m_paymentModel->addPayment(payment);
    m_transaction.payments.append(payment);

    calculateAmountPaid();
}

void QMLPurchaseCartModel::removePayment(int index)
{
    if (index <= -1 || index >= m_transaction.payments.count())
        return;

    m_paymentModel->removePayment(index);
    m_transaction.payments.removeAt(index);

    calculateAmountPaid();
}

void QMLPurchaseCartModel::submitTransaction(const QVariantMap &addOns)
{
    if (m_transaction.balance != 0.0
            && addOns.value("due_date_time").isNull())
        emit error(ModelResult{ NoDueDateSetError });
    else
        addTransaction(addOns);
}

void QMLPurchaseCartModel::suspendTransaction(const QVariantMap &params)
{
    if (m_transaction.id == -1)
        addTransaction( { { "suspended", true },
                          { "action", "suspend" },
                          { "note", params.value("note") } });
    else
        updateSuspendedTransaction({ { "note", params.value("note") } });
}

void QMLPurchaseCartModel::clearPayments()
{
    if (!m_transaction.payments.isEmpty()) {
        m_paymentModel->clearPayments();
        m_transaction.payments.clear();
        calculateAmountPaid();
    }
}

void QMLPurchaseCartModel::clearAll()
{
    beginResetModel();
    setTransactionId(-1);
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
        emit execute(new PurchaseQuery::AddPurchaseTransaction(m_transaction, this));
    } else {
        emit error(ModelResult{ EmptyCartError });
    }
}

void QMLPurchaseCartModel::updateSuspendedTransaction(const QVariantMap &transactionAddOns)
{
    const QString &note = transactionAddOns.value("note").toString();
    m_transaction.flags.setFlag(Utility::RecordGroup::Suspended);
    m_transaction.note = Utility::Note{ note };

    setBusy(true);

    if (!m_transaction.products.isEmpty())
        emit execute(new PurchaseQuery::UpdateSuspendedPurchaseTransaction(m_transaction, this));
    else
        emit error(ModelResult{ EmptyCartError });
}

void QMLPurchaseCartModel::tryQuery()
{
    if (m_transaction.id > -1) {
        setBusy(true);
        emit execute(new PurchaseQuery::ViewPurchaseCart(m_transaction.id, this));
    } else {
        beginResetModel();
        setCustomerName(QString());
        setCustomerPhoneNumber(QString());
        m_transaction.products.clear();
        m_transaction.payments.clear();
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
        m_transaction.products = Utility::PurchaseCartProductList{ result.outcome().toMap().value("products").toList() };
        calculateTotal();

        endResetModel();

        if (result.request().command() == PurchaseQuery::AddPurchaseTransaction::COMMAND) {
            if (result.request().params().value("suspended").toBool()) {
                const Utility::Client &client{ result.outcome().toMap() };
                setTransactionId(-1);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setClientId(client.id);
                emit success(ModelResult{ SuspendTransactionSuccess });
            } else {
                const Utility::Client &client{ result.outcome().toMap() };
                setTransactionId(-1);
                setCustomerName(QString());
                setCustomerPhoneNumber(QString());
                setClientId(client.id);
                emit success(ModelResult{ SubmitTransactionSuccess });
            }
        } else if (result.request().command() == PurchaseQuery::ViewPurchaseCart::COMMAND) {
            const Utility::Client &client{ result.outcome().toMap() };
            setClientId(client.id);
            setCustomerName(client.preferredName);
            setCustomerPhoneNumber(client.phoneNumber);
            emit success(ModelResult{ RetrieveTransactionSuccess });
        } else if (result.request().command() == PurchaseQuery::UpdateSuspendedPurchaseTransaction::COMMAND) {
            const Utility::Client &client{ result.outcome().toMap() };
            setTransactionId(-1);
            setClientId(client.id);
            setCustomerName(client.preferredName);
            setCustomerPhoneNumber(client.phoneNumber);
            emit success(ModelResult{ SuspendTransactionSuccess });
        } else if (result.request().command() == PurchaseQuery::AddPurchaseTransaction::UNDO_COMMAND) {
            emit success(ModelResult{ UndoSubmitTransactionSuccess });
        } else {
            emit success(ModelResult{ UnknownSuccess });
        }
    } else {
        if (result.request().command() == PurchaseQuery::AddPurchaseTransaction::COMMAND) {
            const auto transaction = Utility::PurchaseTransaction{ result.request().params() };
            if (transaction.flags.testFlag(Utility::RecordGroup::Suspended))
                emit error(ModelResult{ SuspendTransactionError });
            else
                emit error(ModelResult{ SubmitTransactionError });
        } else if (result.request().command() == PurchaseQuery::ViewPurchaseCart::COMMAND) {
            emit error(ModelResult{ RetrieveTransactionError });
        } else if (result.request().command() == PurchaseQuery::UpdateSuspendedPurchaseTransaction::COMMAND) {
            emit error(ModelResult{ SuspendTransactionError });
        } else {
            emit error();
        }
    }

    connect(this, &QMLPurchaseCartModel::transactionIdChanged, this, &QMLPurchaseCartModel::tryQuery);
}

void QMLPurchaseCartModel::addProduct(const QVariantMap &product)
{
    const int productId = product.value("product_id").toInt();
    const qreal quantity = product.value("quantity").toDouble();
    const qreal availableQuantity = qMin(1.0,
                                         product.value("available_quantity",
                                                       quantity).toDouble());

    if (availableQuantity <= 0.0)
        return;

    if (!m_transaction.products.contains(Utility::PurchaseCartProduct{ productId })) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        const Utility::PurchaseCartProduct &newProduct{ product };
        m_transaction.products.append(newProduct);
        endInsertRows();
    } else {
        const int row = m_transaction.products.indexOf(Utility::PurchaseCartProduct{ productId });
        Utility::PurchaseCartProduct &existingProduct{ m_transaction.products[row] };
        const qreal oldQuantity = existingProduct.quantity;
        const qreal newQuantity = oldQuantity + 1;

        existingProduct.quantity = qMin(newQuantity, availableQuantity);
        existingProduct.cost = existingProduct.quantity * existingProduct.unitPrice;

        emit dataChanged(index(row), index(row));
    }

    calculateTotal();
}

void QMLPurchaseCartModel::updateProduct(int productId,
                                         const QVariantMap &product)
{
    if (productId <= 0 || product.isEmpty())
        return;

    const int row = m_transaction.products.indexOf(Utility::PurchaseCartProduct{ productId });
    const auto &newProduct = Utility::PurchaseCartProduct{ product };
    Utility::PurchaseCartProduct &existingProduct{ m_transaction.products[row] };
    const double oldQuantity = existingProduct.quantity;
    const double newQuantity = newProduct.quantity;
    const double oldUnitPrice = existingProduct.unitPrice;
    const double oldCost = existingProduct.cost;
    const double newUnitPrice = newProduct.unitPrice;
    const double newCost = newProduct.cost;

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

void QMLPurchaseCartModel::setProductQuantity(int productId,
                                              double quantity)
{
    if (productId <= 0 || quantity <= 0.0)
        return;

    const int row = m_transaction.products.indexOf(Utility::PurchaseCartProduct{ productId });
    Utility::PurchaseCartProduct &existingProduct{ m_transaction.products[row] };
    const qreal oldQuantity = existingProduct.quantity;
    const qreal unitPrice = existingProduct.unitPrice;

    existingProduct.quantity = quantity;
    existingProduct.cost = quantity * unitPrice;

    if (oldQuantity != quantity) {
        emit dataChanged(index(row), index(row));
        calculateTotal();
    }
}

void QMLPurchaseCartModel::updateCanAcceptCash()
{
    const bool canAcceptCash = m_paymentModel->cashPaymentCount() < CASH_PAYMENT_LIMIT;
    if (m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCash) != canAcceptCash) {
        m_transaction.flags.setFlag(Utility::RecordGroup::AcceptsCash, canAcceptCash);
        emit canAcceptCashChanged();
    }
}

void QMLPurchaseCartModel::updateCanAcceptCard()
{
    const bool canAcceptCard = m_paymentModel->cardPaymentCount() < CARD_PAYMENT_LIMIT;
    if (m_transaction.flags.testFlag(Utility::RecordGroup::AcceptsCard) != canAcceptCard) {
        m_transaction.flags.setFlag(Utility::RecordGroup::AcceptsCard, canAcceptCard);
        emit canAcceptCardChanged();
    }
}

void QMLPurchaseCartModel::removeProduct(int productId)
{
    const int row = m_transaction.products.indexOf(Utility::PurchaseCartProduct{ productId });
    beginRemoveRows(QModelIndex(), row, row);
    m_transaction.products.removeAt(row);
    endRemoveRows();

    calculateTotal();
}

void QMLPurchaseCartModel::calculateTotal()
{
    double totalCost = 0.0;
    for (const auto &product : m_transaction.products)
        totalCost += product.cost;

    setTotalCost(totalCost);
    setBalance(totalCost - m_transaction.amountPaid);
}

void QMLPurchaseCartModel::calculateAmountPaid()
{
    double amountPaid = 0.0;
    for (const auto &payment : m_transaction.payments)
        amountPaid += payment.amount;

    setAmountPaid(amountPaid);
    setBalance(m_transaction.totalCost - m_transaction.amountPaid);
}

void QMLPurchaseCartModel::setClientId(int clientId)
{
    if (m_transaction.vendor.client.id == clientId)
        return;

    m_transaction.vendor.client.id = clientId;
    emit clientIdChanged();
}
