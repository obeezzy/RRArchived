#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QObject>
#include <QVariant>
#include <QUrl>

class QMLUserProfile;
class BusinessAdmin;
class BusinessDetails;

class UserProfile : public QObject
{
    friend class QMLUserProfile;
    Q_OBJECT
public:
    static UserProfile &instance();

    int userId() const;
    QString userName() const;
    QString password() const;

    bool isDatabaseReady() const;
    bool isAdmin() const;
    bool hasPrivilege(const QString &privilege) const;
    bool isServerTunnelingEnabled() const;

    BusinessDetails *businessDetails() const;
    BusinessAdmin *businessAdmin() const;

    QString rackId() const;
    QByteArray accessToken() const;
    QByteArray toJson() const;
signals:
    void adminChanged();
private:
    QUrl m_logoUrl;
    QString m_userName;
    QString m_password;
    int m_userId;
    QVariant m_privileges;
    BusinessDetails *m_businessDetails;
    BusinessAdmin *m_businessAdmin;

    explicit UserProfile(QObject *parent = nullptr);
    explicit UserProfile(const UserProfile &other) = delete;
    void setUser(int userId, const QString &userName, const QString &password,
                 const QVariant &privileges, const QByteArray &accessToken);
    void setDatabaseReady(bool databaseReady);
    void setRackId(const QString &rackId);
    void setUserId(int userId);
    void setAccessToken(const QByteArray &accessToken);
    void setServerTunnelingEnabled(bool enabled);

    void clearUser();
};

#endif // USERPROFILE_H
