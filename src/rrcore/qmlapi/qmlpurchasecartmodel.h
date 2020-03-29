#ifndef QMLPURCHASECARTMODEL_H
#define QMLPURCHASECARTMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/purchase/purchasetransaction.h"

class PurchasePayment;
class PurchasePaymentModel;

class QMLPurchaseCartModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int transactionId READ transactionId WRITE setTransactionId NOTIFY transactionIdChanged)
    Q_PROPERTY(QString customerName READ customerName WRITE setCustomerName NOTIFY customerNameChanged)
    Q_PROPERTY(QString customerPhoneNumber READ customerPhoneNumber WRITE setCustomerPhoneNumber NOTIFY customerPhoneNumberChanged)
    Q_PROPERTY(int vendorId READ vendorId NOTIFY vendorIdChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
    Q_PROPERTY(double totalCost READ totalCost NOTIFY totalCostChanged)
    Q_PROPERTY(double balance READ balance NOTIFY balanceChanged)
public:
    explicit QMLPurchaseCartModel(QObject *parent = nullptr);
    explicit QMLPurchaseCartModel(DatabaseThread &thread, QObject *parent = nullptr);
    ~QMLPurchaseCartModel() override = default;

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ProductIdRole,
        ProductRole,
        QuantityRole,
        AvailableQuantityRole,
        UnitRole,
        UnitIdRole,
        CostPriceRole,
        RetailPriceRole,
        UnitPriceRole,
        CostRole
    };

    enum PaymentMethod {
        Cash,
        DebitCard,
        CreditCard
    }; Q_ENUM(PaymentMethod)

    enum SuccessCode {
        UnknownSuccess,
        RetrieveTransactionSuccess,
        SuspendTransactionSuccess,
        SubmitTransactionSuccess,
        UndoSubmitTransactionSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError,
        ConnectionError,
        SuspendTransactionError,
        RetrieveTransactionError,
        SubmitTransactionError,
        EmptyCartError,
        NoDueDateSetError
    }; Q_ENUM(ErrorCode)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    int transactionId() const;
    void setTransactionId(int transactionId);

    QString customerName() const;
    void setCustomerName(const QString &customerName);

    QString customerPhoneNumber() const;
    void setCustomerPhoneNumber(const QString &customerPhoneNumber);

    int vendorId() const;

    QString note() const;
    void setNote(const QString &note);

    double totalCost() const;
    double balance() const;

    Q_INVOKABLE void addProduct(const QVariantMap &product);
    Q_INVOKABLE void updateProduct(int productId, const QVariantMap &product);
    Q_INVOKABLE void setProductQuantity(int productId, double quantity);
    Q_INVOKABLE void removeProduct(int productId);

    Q_INVOKABLE void submitTransaction(const QVariantMap &addOns = QVariantMap());
    Q_INVOKABLE void suspendTransaction(const QVariantMap &addOns = QVariantMap());
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE QString toPrintableFormat() const;
protected:
    void tryQuery() override final;
    bool canProcessResult(const QueryResult &result) const override final;
    void processResult(const QueryResult &result) override final;
signals:
    void transactionIdChanged();
    void customerNameChanged();
    void customerPhoneNumberChanged();
    void vendorIdChanged();
    void noteChanged();
    void totalCostChanged();
    void balanceChanged();
private:
    Utility::PurchaseTransaction m_transaction;

    void addTransaction(const QVariantMap &transaction);
    void updateSuspendedTransaction(const QVariantMap &transactionAddOns);

    void setVendorId(int vendorId);
    void setTotalCost(double totalCost);
    void setBalance(double balance);
    void calculateTotalCost();
};

#endif // QMLPURCHASECARTMODEL_H
