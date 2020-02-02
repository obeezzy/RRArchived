#ifndef QMLDEBTTRANSACTIONMODEL_H
#define QMLDEBTTRANSACTIONMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/debtorutils.h"

#include <QUrl>

class DebtPaymentModel;

class QMLDebtTransactionModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int debtorId READ debtorId WRITE setDebtorId NOTIFY debtorIdChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
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
        DataUnchangedError,
        DuplicateEntryError,
        AmountOverpaidError,
        InvalidDueDateError
    }; Q_ENUM(ErrorCode)

    enum Role {
        TransactionIdRole = Qt::UserRole,
        RelatedTransactionRole,
        RelatedTransactionIdRole,
        DueDateRole,
        DirtyRole,
        NoteRole,
        CreatedRole,
        RefRole,
        TotalDebtRole
    };

    explicit QMLDebtTransactionModel(QObject *parent = nullptr);
    explicit QMLDebtTransactionModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool isDirty() const;

    int debtorId() const;
    void setDebtorId(int debtorId);

    int clientId() const;

    QUrl imageUrl() const;
    void setImageUrl(const QUrl &imageUrl);

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

    Q_INVOKABLE void addDebt(double totalDebt, const QDateTime &dueDateTime, const QString &note = QString());
    Q_INVOKABLE void removeDebt(int row);
public slots:
    bool submit() override;
signals:
    void debtorIdChanged();
    void imageUrlChanged();
    void firstNameChanged();
    void lastNameChanged();
    void preferredNameChanged();
    void primaryPhoneNumberChanged();
    void alternatePhoneNumberModelChanged();
    void addressModelChanged();
    void emailAddressModelChanged();
    void noteChanged();
    void clientIdChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_debtorId;
    int m_clientId;
    bool m_dirty;
    QUrl m_imageUrl;
    QString m_firstName;
    QString m_lastName;
    QString m_preferredName;
    QString m_primaryPhoneNumber;
    QStringList m_alternatePhoneNumberModel;
    QStringList m_addressModel;
    QStringList m_emailAddressModel;
    Note m_note;
    DebtTransactionList m_transactions;

    void clearAll();
    bool isExistingDebtor() const;
    bool paymentsDirty() const;
    bool paymentsDueDateValid() const;
    void setClientId(int clientId);

    QString tableNameNormalized(const QString &tableName) const;
};

Q_DECLARE_LOGGING_CATEGORY(lcqmldebttransactionmodel);

#endif // QMLDEBTTRANSACTIONMODEL_H
