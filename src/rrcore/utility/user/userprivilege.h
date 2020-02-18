#ifndef USERPRIVILEGE_H
#define USERPRIVILEGE_H

#include <QVariantList>
#include <initializer_list>

namespace Utility {
struct UserPrivilege
{
    int id {-1};
    int userId {-1};

    explicit UserPrivilege() = default;
    explicit UserPrivilege(int userId);
    explicit UserPrivilege(const QVariantMap &map);

    QVariantMap toVariantMap() const;
};

class UserPrivilegeList : public QList<UserPrivilege> {
public:
    explicit UserPrivilegeList() {}
    explicit UserPrivilegeList(std::initializer_list<UserPrivilege> productList) :
        QList<UserPrivilege>(productList) {}
    explicit UserPrivilegeList(const QVariantList &list) :
        QList<UserPrivilege>() {
        for (const auto &variant : list)
            append(UserPrivilege{variant.toMap()});
    }

    inline QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &userPrivilege : *this)
            list.append(userPrivilege.toVariantMap());
        return list;
    }
};
}

Q_DECLARE_TYPEINFO(Utility::UserPrivilege, Q_PRIMITIVE_TYPE);

#endif // USERPRIVILEGE_H
