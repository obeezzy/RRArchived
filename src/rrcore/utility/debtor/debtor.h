#ifndef DEBTOR_H
#define DEBTOR_H

#include <QDebug>
#include <QString>
#include <QUrl>
#include "debttransaction.h"
#include "utility/common/client.h"
#include "utility/user/user.h"

namespace Utility {
namespace Debtor {
struct Debtor
{
    int id{0};
    Client client;
    Money totalDebt;
    DebtTransactionList transactions;
    Note note;
    RecordTimestamp timestamp;
    User::User user;
    int row{-1};

    explicit Debtor() = default;
    explicit Debtor(const QVariantMap& map);
    explicit Debtor(int id);
    explicit Debtor(const Client& client,
                    const DebtTransactionList& transactions,
                    const Note& note = Note());
    explicit Debtor(int id, const Client& client,
                    const DebtTransactionList& transactions,
                    const Note& note = Note());

    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const Debtor& debtor)
    {
        debug.nospace() << "Debtor("
                        << "id=" << debtor.id << ", client=" << debtor.client
                        << ", debtTransactionList=" << debtor.transactions
                        << ", note=" << debtor.note << ", row=" << debtor.row
                        << ")";

        return debug.nospace();
    }
};

class DebtorList : public QList<Debtor>
{
public:
    explicit DebtorList() = default;
    DebtorList(std::initializer_list<Debtor> debtors) : QList<Debtor>(debtors)
    {}
    explicit DebtorList(const QVariantList& list) : QList<Debtor>()
    {
        for (const auto& variant : list)
            append(Debtor{variant.toMap()});
    }

    QVariantList toVariantList() const
    {
        QVariantList list;
        for (const auto& debtor : *this)
            list.append(debtor.toVariantMap());
        return list;
    }
};
}  // namespace Debtor
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::Debtor::Debtor, Q_PRIMITIVE_TYPE);

#endif  // DEBTOR_H
