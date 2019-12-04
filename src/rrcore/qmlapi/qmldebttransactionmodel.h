#ifndef QMLDEBTTRANSACTIONMODEL_H
#define QMLDEBTTRANSACTIONMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/debtorutils.h"

class DebtPaymentModel;

class QMLDebtTransactionModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int debtorId READ debtorId WRITE setDebtorId NOTIFY debtorIdChanged)
    Q_PROPERTY(QString imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString preferredName READ preferredName WRITE setPreferredName NOTIFY preferredNameChanged)
    Q_PROPERTY(QString primaryPhoneNumber READ primaryPhoneNumber WRITE setPrimaryPhoneNumber NOTIFY primaryPhoneNumberChanged)
    Q_PROPERTY(QStringList alternatePhoneNumberModel READ alternatePhoneNumberModel NOTIFY alternatePhoneNumberModelChanged)
    Q_PROPERTY(QStringList addressModel READ addressModel NOTIFY addressModelChanged)
    Q_PROPERTY(QStringList emailAddressModel READ emailAddressModel NOTIFY emailAddressModelChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
public:
    enum SuccessCode {
        UnknownSuccess,
        AddDebtorSuccess,
        UpdateDebtorSuccess,
        UndoAddDebtorSuccess,
        ViewDebtorTransactionsSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError,
        NoPreferredNameError,
        NoPrimaryPhoneNumberError,
        NoDebtError,
        NoPaymentError,
        DuplicateEntryError,
        AmountOverpaidError,
        InvalidDueDateError
    }; Q_ENUM(ErrorCode)

    enum Role {
        TransactionIdRole = Qt::UserRole,
        RelatedTransactionRole,
        RelatedTransactionIdRole,
        DueDateRole,
        NoteRole,
        CreatedRole,
        PaymentModelRole,
        CurrentBalanceRole
    };

    explicit QMLDebtTransactionModel(QObject *parent = nullptr);
    explicit QMLDebtTransactionModel(DatabaseThread &thread, QObject *parent = nullptr);
    ~QMLDebtTransactionModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    bool isDirty() const;

    int debtorId() const;
    void setDebtorId(int debtorId);

    int clientId() const;

    QString imageSource() const;
    void setImageSource(const QString &imageSource);

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    QString preferredName() const;
    void setPreferredName(const QString &preferredName);

    QString primaryPhoneNumber() const;
    void setPrimaryPhoneNumber(const QString &primaryPhoneNumber);

    QStringList alternatePhoneNumberModel() const;
    QStringList addressModel() const;
    QStringList emailAddressModel() const;

    QString note() const;
    void setNote(const QString &note);

    Q_INVOKABLE void addAlternatePhoneNumber(const QString &alternatePhoneNumber);
    Q_INVOKABLE void removeAlternatePhoneNumber(int row);

    Q_INVOKABLE void addAddress(const QString &address);
    Q_INVOKABLE void removeAddress(int row);

    Q_INVOKABLE void addEmailAddress(const QString &emailAddress);
    Q_INVOKABLE void removeEmailAddress(int row);

    Q_INVOKABLE int addDebt(double totalDebt, const QDateTime &dueDateTime, const QString &note = QString());
    Q_INVOKABLE void updateDebt(int debtIndex, const QDateTime &dueDateTime, const QString &note = QString());
    Q_INVOKABLE void removeDebt(int debtIndex);

    Q_INVOKABLE void addPayment(int debtIndex, double amount, const QString &note = QString());
    Q_INVOKABLE void updatePayment(int debtIndex, int paymentIndex, double amount, const QString &note = QString());
    Q_INVOKABLE void removePayment(int debtIndex, int paymentIndex);
public slots:
    bool submit() override;
signals:
    void debtorIdChanged();
    void imageSourceChanged();
    void firstNameChanged();
    void lastNameChanged();
    void preferredNameChanged();
    void primaryPhoneNumberChanged();
    void alternatePhoneNumberModelChanged();
    void addressModelChanged();
    void emailAddressModelChanged();
    void noteChanged();

    void dirtyChanged();
    void clientIdChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_debtorId;
    int m_clientId;
    bool m_dirty;
    QString m_imageSource;
    QString m_firstName;
    QString m_lastName;
    QString m_preferredName;
    QString m_primaryPhoneNumber;
    QStringList m_alternatePhoneNumberModel;
    QStringList m_addressModel;
    QStringList m_emailAddressModel;
    QString m_note;
    QVariantList m_records;
    QList<DebtPaymentModel *> m_debtPaymentModels;
    DebtTransactionList m_existingDebtTransactions;
    DebtTransactionList m_newDebtTransactions;

    QVector<int> m_archivedDebtTransactionIds;
    QVector<int> m_archivedDebtPaymentIds;

    QVariant convertToVariant(const DebtTransactionList &debtTransactions);
    QVariant convertToVariant(const QVector<int> &archivedDebtTransactionIds);

    void clearAll();
    void clearDebtTransactions();
    void clearPayments();

    bool isExistingRecord(int row) const;
    void setDirty(bool dirty);
    void setClientId(int clientId);
};

#endif // QMLDEBTTRANSACTIONMODEL_H
