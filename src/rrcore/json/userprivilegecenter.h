#ifndef USERPRIVILEGECENTER_H
#define USERPRIVILEGECENTER_H

#include <QObject>
#include <QVariant>

class UserPrivilegeCenter : public QObject
{
    Q_OBJECT
public:
    explicit UserPrivilegeCenter(QObject* parent = nullptr);

    QVariant getPrivileges(const QString& group = QString()) const;
};

#endif  // USERPRIVILEGECENTER_H
