#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QObject>
#include "qmlapi/qmluserprofile.h"

class UserProfile : public QObject
{
    Q_OBJECT
    friend class QMLUserProfile;
public:
    static UserProfile &instance();

    int userId() const;
    QString userName() const;
private:
    static UserProfile *m_instance;
    QString m_userName;
    int m_userId;

    explicit UserProfile(QObject *parent = nullptr);
    void setUser(int userId, const QString &userName);
};

#endif // USERPROFILE_H
