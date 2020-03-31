#ifndef QMLDEBTTRANSACTIONMODEL_H
#define QMLDEBTTRANSACTIONMODEL_H

#include "models/abstractvisuallistmodel.h"
#include "utility/debtor/debtor.h"

class QUrl;

class QMLDebtTransactionModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(int debtorId READ debtorId WRITE setDebtorId NOTIFY debtorIdChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString preferredName READ preferredName WRITE setPreferredName NOTIFY preferredNameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
public:
    enum SuccessCode {
        AddDebtorSuccess,
        UpdateDebtorSuccess,
        UndoAddDebtorSuccess,
        ViewDebtorTransactionsSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        NoPreferredNameError,
        NoPhoneNumberError,
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

    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

    QString note() const;
    void setNote(const QString &note);

    Q_INVOKABLE void addDebt(double totalDebt,
                             const QDateTime &dueDateTime,
                             const QString &note = QString());
    Q_INVOKABLE void removeDebt(int row);
public slots:
    bool submit() override;
signals:
    void debtorIdChanged();
    void imageUrlChanged();
    void firstNameChanged();
    void lastNameChanged();
    void preferredNameChanged();
    void phoneNumberChanged();
    void alternatePhoneNumberModelChanged();
    void addressModelChanged();
    void emailAddressModelChanged();
    void noteChanged();
    void clientIdChanged();
protected:
    void tryQuery() override;
    bool canProcessResult(const QueryResult &result) const override final;
    void processResult(const QueryResult &result) override final;
private:
    bool m_dirty {false};
    Utility::Debtor m_debtor;

    void clearAll();
    bool isExistingDebtor() const;
    bool paymentsDirty() const;
    bool paymentsDueDateValid() const;
    void setClientId(int clientId);
};

Q_DECLARE_LOGGING_CATEGORY(lcqmldebttransactionmodel);

#endif // QMLDEBTTRANSACTIONMODEL_H
