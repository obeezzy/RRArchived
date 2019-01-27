#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QObject>
#include <QVariant>
#include "qmlapi/qmluserprofile.h"

class UserProfile : public QObject
{
    Q_OBJECT
    friend class QMLUserProfile;
public:
    static UserProfile &instance();

    int userId() const;
    QString userName() const;

    bool isAdmin() const;
    bool hasPrivilege(const QString &privilege) const;
signals:
    void adminChanged();
private:
    static UserProfile *m_instance;
    QString m_userName;
    int m_userId;
    QVariant m_privileges;

    explicit UserProfile(QObject *parent = nullptr);
    void setUser(int userId, const QString &userName, const QVariant &privileges);
};

#endif // USERPROFILE_H
