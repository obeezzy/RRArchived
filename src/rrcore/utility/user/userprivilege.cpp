#include "userprivilege.h"

using namespace Utility::User;

UserPrivilege::UserPrivilege(int userId) :
    userId(userId)
{}

UserPrivilege::UserPrivilege(const QVariantMap &map) :
    id(map.value("user_privilege_id").toInt()),
    userId(map.value("user_id").toInt())
{}

QVariantMap UserPrivilege::toVariantMap() const
{
    return { };
}
