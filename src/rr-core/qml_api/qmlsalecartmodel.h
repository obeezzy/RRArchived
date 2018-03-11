#ifndef QMLSALECARTMODEL_H
#define QMLSALECARTMODEL_H

#include <QSqlRecord>
#include <QDateTime>

#include "rr-core/models/abstractvisuallistmodel.h"

class QMLSaleCartModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(qint64 transactionId READ transactionId WRITE setTransactionId NOTIFY transactionIdChanged)
    Q_PROPERTY(QString customerName READ customerName WRITE setCustomerName NOTIFY customerNameChanged)
    Q_PROPERTY(QString customerPhoneNumber READ customerPhoneNumber WRITE setCustomerPhoneNumber NOTIFY customerPhoneNumberChanged)
    Q_PROPERTY(int clientId READ clientId NOTIFY clientIdChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
    Q_PROPERTY(double totalCost READ totalCost NOTIFY totalCostChanged)
    Q_PROPERTY(double amountPaid READ amountPaid WRITE setAmountPaid NOTIFY amountPaidChanged)
    Q_PROPERTY(QDateTime date READ date WRITE setDate NOTIFY dateChanged)
public:
    explicit QMLSaleCartModel(QObject *parent = nullptr);

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

    enum SuccessCodes {
        TransactionRetrieved,
        TransactionSuspended,
        TransactionSubmitted
    }; Q_ENUM(SuccessCodes)

    enum ErrorCodes {
        FailedToConnect,
        FailedToSuspend
    }; Q_ENUM(ErrorCodes)

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
    void setAmountPaid(double amountPaid);

    QDateTime date() const;
    void setDate(const QDateTime &dateTime);

    Q_INVOKABLE void submitTransaction(const QVariantMap &paymentInfo = QVariantMap());
    Q_INVOKABLE void suspendTransaction(const QVariantMap &params = QVariantMap());
    Q_INVOKABLE void clearAll();
protected:
    void tryQuery() override final;
    void processResult(const QueryResult &result) override final;
signals:
    void transactionIdChanged();
    void customerNameChanged();
    void customerPhoneNumberChanged();
    void clientIdChanged();
    void noteChanged();
    void totalCostChanged();
    void amountPaidChanged();
    void dateChanged();
public slots:
    void addItem(const QVariantMap &itemInfo);
    void setItemQuantity(int itemId, double quantity);
    void incrementItemQuantity(int itemId, double quantity = 1.0);
    void decrementItemQuantity(int itemId, double quantity = 1.0);
    void removeItem(int itemId);
    void undoLastCommit() override final;
private:
    qint64 m_transactionId;
    QString m_customerName;
    QString m_customerPhoneNumber;
    int m_clientId;
    QString m_note;
    double m_totalCost;
    double m_amountPaid;
    QDateTime m_dateTime;
    QVariantList m_records;

    int m_lastTransactionId;
    int m_lastClientId;

    bool containsItem(int itemId);
    int indexOfItem(int itemId);
    void addTransaction(const QVariantMap &paymentInfo);
    void updateSuspendedTransaction(const QVariantMap &paymentInfo);

    void setTotalCost(double totalCost);
    void calculateTotals();
};

#endif // QMLSALECARTMODEL_H
