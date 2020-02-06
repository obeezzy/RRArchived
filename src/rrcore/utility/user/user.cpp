#include "user.h"
#include <QString>

User::User() :
    id(-1)
{

}

User::User(int id, const QString &user) :
    id(id),
    user(user)
{

}
