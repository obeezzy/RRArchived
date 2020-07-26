#ifndef PURCHASETRANSACTION_H
#define PURCHASETRANSACTION_H

#include "purchasecartproduct.h"
#include "purchasepayment.h"
#include "utility/common/note.h"
#include "utility/common/recordgroup.h"
#include "utility/common/recordtimestamp.h"
#include "utility/common/vendor.h"
#include <QString>

namespace Utility {
namespace Purchase {
struct PurchaseTransaction {
    int id {0};
    Vendor vendor;
    PurchaseMonies monies;
    PurchaseCartProductList products;
    PurchasePaymentList payments;
    QDateTime dueDateTime;
    QString action;
    RecordGroup::Flags flags;
    Note note;
    RecordTimestamp timestamp;
    User::User user;
    int row {-1};

    explicit PurchaseTransaction() = default;
    explicit PurchaseTransaction(int id);
    explicit PurchaseTransaction(int id,
                                 const Vendor &vendor,
                                 const PurchaseMonies &monies,
                                 const PurchaseCartProductList &products,
                                 const PurchasePaymentList &payments,
                                 const RecordGroup::Flags &flags,
                                 const QDateTime &dueDateTime = QDateTime(),
                                 const QString &action = QString(),
                                 const Note &note = Note());
    explicit PurchaseTransaction(int id,
                                 const Vendor &vendor,
                                 const PurchaseMonies &monies,
                                 const PurchaseCartProductList &products,
                                 const PurchasePaymentList &payments,
                                 const RecordGroup::Flags &flags,
                                 const Note &note = Note());
    explicit PurchaseTransaction(const QVariantMap &transaction);
    QVariantMap toVariantMap() const;
};

class PurchaseTransactionList : public QList<PurchaseTransaction> {
public:
    explicit PurchaseTransactionList() = default;
    PurchaseTransactionList(std::initializer_list<PurchaseTransaction> transactions) :
        QList<PurchaseTransaction>(transactions) { }
    explicit PurchaseTransactionList(const QVariantList &list) :
        QList<PurchaseTransaction>() {
        for (const auto &variant : list)
            append(PurchaseTransaction{ variant.toMap() });
    }

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &transaction : *this)
            list.append(transaction.toVariantMap());
        return list;
    }
};
}
}
Q_DECLARE_TYPEINFO(Utility::Purchase::PurchaseTransaction, Q_PRIMITIVE_TYPE);

#endif // PURCHASETRANSACTION_H
