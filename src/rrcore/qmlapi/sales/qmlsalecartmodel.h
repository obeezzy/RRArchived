#ifndef QMLSALECARTMODEL_H
#define QMLSALECARTMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/sales/saletransaction.h"
#include <QLoggingCategory>

class SalePaymentModel;

class QMLSaleCartModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int transactionId READ transactionId WRITE setTransactionId NOTIFY transactionIdChanged)
    Q_PROPERTY(QString customerName READ customerName WRITE setCustomerName NOTIFY customerNameChanged)
    Q_PROPERTY(QString customerPhoneNumber READ customerPhoneNumber WRITE setCustomerPhoneNumber NOTIFY customerPhoneNumberChanged)
    Q_PROPERTY(int customerId READ customerId NOTIFY customerIdChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
    Q_PROPERTY(double totalCost READ totalCost NOTIFY totalCostChanged)
    Q_PROPERTY(double amountPaid READ amountPaid WRITE setAmountPaid NOTIFY amountPaidChanged)
    Q_PROPERTY(double balance READ balance NOTIFY balanceChanged)
public:
    explicit QMLSaleCartModel(QObject *parent = nullptr);
    explicit QMLSaleCartModel(DatabaseThread &thread,
                              QObject *parent = nullptr);
    ~QMLSaleCartModel() override = default;

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
        RetrieveTransactionSuccess,
        SuspendTransactionSuccess,
        SubmitTransactionSuccess,
        UndoSubmitTransactionSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        ConnectionError,
        SuspendTransactionError,
        RetrieveTransactionError,
        SubmitTransactionError,
        UndoSubmitTransactionError,
        EmptyCartError,
        NoDueDateSetError
    }; Q_ENUM(ErrorCode)

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    bool isExistingTransaction() const;

    int transactionId() const;
    void setTransactionId(int transactionId);

    QString customerName() const;
    void setCustomerName(const QString &customerName);

    QString customerPhoneNumber() const;
    void setCustomerPhoneNumber(const QString &customerPhoneNumber);

    int customerId() const;

    QString note() const;
    void setNote(const QString &note);

    double totalCost() const;
    double balance() const;

    double amountPaid() const;
    void setAmountPaid(double amountPaid);

    Q_INVOKABLE void addProduct(const QVariantMap &product);
    Q_INVOKABLE void updateProduct(int productId,
                                   const QVariantMap &product);
    Q_INVOKABLE void setProductQuantity(int productId,
                                        double quantity);
    Q_INVOKABLE void removeProduct(int productId);

    Q_INVOKABLE void submitTransaction(const QVariantMap &transactionInfo = QVariantMap());
    Q_INVOKABLE void suspendTransaction(const QVariantMap &transactionInfo = QVariantMap());
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE QString toPrintableFormat() const;
protected:
    void tryQuery() override final;
    bool canProcessResult(const QueryResult &result) const override final;
    void processResult(const QueryResult &result) override final;
    void undoLastCommit() override;
signals:
    void transactionIdChanged();
    void customerNameChanged();
    void customerPhoneNumberChanged();
    void customerIdChanged();
    void noteChanged();
    void totalCostChanged();
    void amountPaidChanged();
    void balanceChanged();
private:
    Utility::Sales::SaleTransaction m_transaction;

    void addTransaction(const QVariantMap &transaction);
    void updateSuspendedTransaction(const QVariantMap &addOns);
    void calculateTotalCost();
    void calculateBalance();

    void setCustomerId(int customerId);
    void setTotalCost(double totalCost);
    void setBalance(double balance);
};

Q_DECLARE_LOGGING_CATEGORY(lcqmlsalecartmodel)

#endif // QMLSALECARTMODEL_H
