#ifndef DEBTOR_H
#define DEBTOR_H

#include "utility/commonutils.h"
#include "debttransaction.h"
#include "utility/user/user.h"
#include <QString>
#include <QUrl>
#include <QDebug>

namespace Utility {
struct Debtor
{
    int id {-1};
    Client client;
    double totalDebt {0.0};
    DebtTransactionList transactions;
    Note note;
    QDateTime created;
    QDateTime lastEdited;
    User user;
    int row {-1};

    explicit Debtor() = default;
    explicit Debtor(const QVariantMap &map);
    explicit Debtor(int id);
    explicit Debtor(const Client &client,
                    const DebtTransactionList &transactions,
                    const Note &note);
    explicit Debtor(int id,
                    const Client &client,
                    const DebtTransactionList &transactions,
                    const Note &note);
    explicit Debtor(int id,
                    int row);

    QVariantMap toVariantMap() const;

    friend QDebug operator<<(QDebug debug, const Debtor &debtor)
    {
        debug.nospace() << "Debtor("
                        << "id=" << debtor.id
                        << ", client=" << debtor.client
                        << ", debtTransactionList=" << debtor.transactions
                        << ", note=" << debtor.note
                        << ", row=" << debtor.row
                        << ")";

        return debug.nospace();
    }
};

class DebtorList : public QList<Debtor>
{
public:
    explicit DebtorList() = default;
    DebtorList(std::initializer_list<Debtor> debtors) :
        QList<Debtor>(debtors) { }
    explicit DebtorList(const QVariantList &list) :
        QList<Debtor>() {
        for (const auto &variant : list)
            append(Debtor{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &debtor : *this)
            list.append(debtor.toVariantMap());
        return list;
    }
};
}
Q_DECLARE_TYPEINFO(Utility::Debtor, Q_PRIMITIVE_TYPE);

#endif // DEBTOR_H
