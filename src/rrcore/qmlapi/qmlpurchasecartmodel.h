#ifndef QMLPURCHASECARTMODEL_H
#define QMLPURCHASECARTMODEL_H

#include <QSqlRecord>
#include <QDateTime>

#include "models/abstractvisuallistmodel.h"
#include "utility/purchaseutils.h"

class PurchasePayment;
class PurchasePaymentModel;

class QMLPurchaseCartModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(qint64 transactionId READ transactionId WRITE setTransactionId NOTIFY transactionIdChanged)
    Q_PROPERTY(QString customerName READ customerName WRITE setCustomerName NOTIFY customerNameChanged)
    Q_PROPERTY(QString customerPhoneNumber READ customerPhoneNumber WRITE setCustomerPhoneNumber NOTIFY customerPhoneNumberChanged)
    Q_PROPERTY(int clientId READ clientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
    Q_PROPERTY(double totalCost READ totalCost NOTIFY totalCostChanged)
    Q_PROPERTY(double amountPaid READ amountPaid NOTIFY amountPaidChanged)
    Q_PROPERTY(double balance READ balance NOTIFY balanceChanged)
    Q_PROPERTY(bool canAcceptCash READ canAcceptCash NOTIFY canAcceptCashChanged)
    Q_PROPERTY(bool canAcceptCard READ canAcceptCard NOTIFY canAcceptCardChanged)
    Q_PROPERTY(PurchasePaymentModel *paymentModel READ paymentModel NOTIFY paymentModelChanged)
public:
    explicit QMLPurchaseCartModel(QObject *parent = nullptr);
    explicit QMLPurchaseCartModel(DatabaseThread &thread, QObject *parent = nullptr);
    ~QMLPurchaseCartModel() override = default;

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ItemIdRole,
        ItemRole,
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

    qint64 transactionId() const;
    void setTransactionId(qint64 transactionId);

    QString customerName() const;
    void setCustomerName(const QString &customerName);

    QString customerPhoneNumber() const;
    void setCustomerPhoneNumber(const QString &customerPhoneNumber);

    int clientId() const;

    QString note() const;
    void setNote(const QString &note);

    double totalCost() const;
    double amountPaid() const;
    double balance() const;

    bool canAcceptCash() const;
    bool canAcceptCard() const;

    PurchasePaymentModel *paymentModel() const;

    PurchasePaymentList payments() const;

    Q_INVOKABLE void addPayment(double amount, PaymentMethod method, const QString &note = QString());
    Q_INVOKABLE void removePayment(int index);
    Q_INVOKABLE void clearPayments();
    Q_INVOKABLE void submitTransaction(const QVariantMap &transactionInfo = QVariantMap());
    Q_INVOKABLE void suspendTransaction(const QVariantMap &transactionInfo = QVariantMap());
    Q_INVOKABLE void clearAll();
    Q_INVOKABLE QString toPrintableFormat() const;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
signals:
    void transactionIdChanged();
    void customerNameChanged();
    void customerPhoneNumberChanged();
    void clientIdChanged();
    void noteChanged();
    void totalCostChanged();
    void amountPaidChanged();
    void balanceChanged();
    void canAcceptCashChanged();
    void canAcceptCardChanged();

    void paymentModelChanged();
public slots:
    void addItem(const QVariantMap &itemInfo);
    void updateItem(int itemId, const QVariantMap &itemInfo);
    void setItemQuantity(int itemId, double quantity);
    void removeItem(int itemId);
private:
    qint64 m_transactionId;
    QString m_clientName;
    QString m_customerPhoneNumber;
    int m_clientId;
    QString m_note;
    double m_totalCost;
    double m_amountPaid;
    double m_balance;
    bool m_canAcceptCash;
    bool m_canAcceptCard;
    QVariantList m_records;
    PurchasePaymentList m_purchasePayments;
    PurchasePaymentModel *m_paymentModel;

    bool containsItem(int itemId);
    int indexOfItem(int itemId);
    void addTransaction(const QVariantMap &transactionInfo);
    void updateSuspendedTransaction(const QVariantMap &transactionInfo);

    void setTotalCost(double totalCost);
    void setAmountPaid(double amountPaid);
    void setBalance(double balance);
    void calculateTotal();
    void calculateAmountPaid();

    void setClientId(int clientId);

    void incrementItemQuantity(int itemId, double quantity = 1.0);
    void decrementItemQuantity(int itemId, double quantity = 1.0);

    void updateCanAcceptCash();
    void updateCanAcceptCard();
};

#endif // QMLPURCHASECARTMODEL_H
