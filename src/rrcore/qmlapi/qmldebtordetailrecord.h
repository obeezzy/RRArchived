#ifndef QMLDEBTORDETAILRECORD_H
#define QMLDEBTORDETAILRECORD_H

#include "detailrecord/abstractdetailrecord.h"
#include "utility/debtor/debtor.h"

class DatabaseThread;
class QDateTime;
class QUrl;

class QMLDebtorDetailRecord : public AbstractDetailRecord
{
    Q_OBJECT
    Q_PROPERTY(int debtorId READ debtorId WRITE setDebtorId NOTIFY debtorIdChanged)
    Q_PROPERTY(QString preferredName READ preferredName NOTIFY preferredNameChanged)
    Q_PROPERTY(QString firstName READ firstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QStringList addressModel READ addressModel NOTIFY addressModelChanged)
    Q_PROPERTY(QStringList emailModel READ emailModel NOTIFY emailModelChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(QDateTime created READ created NOTIFY createdChanged)
    Q_PROPERTY(QDateTime lastEdited READ lastEdited NOTIFY lastEditedChanged)
    Q_PROPERTY(int userId READ userId NOTIFY userIdChanged)
    Q_PROPERTY(QString user READ user NOTIFY userChanged)
public:
    explicit QMLDebtorDetailRecord(QObject *parent = nullptr);
    explicit QMLDebtorDetailRecord(DatabaseThread &thread, QObject *parent = nullptr);

    int debtorId() const;
    void setDebtorId(int debtorId);

    QString preferredName() const;
    QString firstName() const;
    QString lastName() const;
    QString phoneNumber() const;
    QUrl imageUrl() const;

    QStringList addressModel() const;
    QStringList emailModel() const;

    QDateTime created() const;
    QDateTime lastEdited() const;

    int userId() const;
    QString user() const;
signals:
    void debtorIdChanged();
    void preferredNameChanged();
    void firstNameChanged();
    void lastNameChanged();
    void phoneNumberChanged();
    void addressModelChanged();
    void emailModelChanged();
    void imageUrlChanged();
    void createdChanged();
    void lastEditedChanged();
    void userIdChanged();
    void userChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    Utility::Debtor m_debtor;

    void setPreferredName(const QString &preferredName);
    void setFirstName(const QString &firstName);
    void setLastName(const QString &lastName);
    void setPhoneNumber(const QString &phoneNumber);
    void setAddressModel(const QStringList &addressModel);
    void setEmailModel(const QStringList &emailModel);
    void setImageUrl(const QUrl &imageUrl);

    void setCreated(const QDateTime &created);
    void setLastEdited(const QDateTime &lastEdited);
    void setUserId(int userId);
    void setUser(const QString &user);
};

#endif // QMLDEBTORDETAILRECORD_H
