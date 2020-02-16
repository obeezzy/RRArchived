#ifndef RELATEDTRANSACTION_H
#define RELATEDTRANSACTION_H

#include <QString>

namespace Utility {
struct RelatedTransaction {
    int id {-1};
    QString tableName;

    explicit RelatedTransaction() = default;
    explicit RelatedTransaction(int id,
                                const QString &tableName);

    QString toString() const;
};
}

#endif // RELATEDTRANSACTION_H
