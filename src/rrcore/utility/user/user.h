#ifndef USER_H
#define USER_H

#include <QString>

struct User
{
    int id;
    QString user;

    explicit User();
    explicit User(int id, const QString &user);
};

#endif // USER_H
