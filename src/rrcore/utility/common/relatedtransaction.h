#ifndef RELATEDTRANSACTION_H
#define RELATEDTRANSACTION_H

#include <QString>

namespace Utility {
inline namespace Common {
struct RelatedTransaction
{
    int id{0};
    QString tableName;

    explicit RelatedTransaction() = default;
    explicit RelatedTransaction(int id, const QString& tableName);

    QString toString() const;
};
}  // namespace Common
}  // namespace Utility
Q_DECLARE_TYPEINFO(Utility::RelatedTransaction, Q_PRIMITIVE_TYPE);

#endif  // RELATEDTRANSACTION_H
