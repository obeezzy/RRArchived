#ifndef RECORDTIMESTAMP_H
#define RECORDTIMESTAMP_H

#include <QDateTime>
#include <QVariantMap>

namespace Utility {
    inline namespace Common {
    struct RecordTimestamp
    {
        QDateTime created;
        QDateTime lastEdited;

        explicit RecordTimestamp();
        explicit RecordTimestamp(const QDateTime &created,
                                 const QDateTime &lastEdited);
        explicit RecordTimestamp(const QVariantMap &map);
        QVariantMap toVariantMap() const;
    };
}
}

#endif // RECORDTIMESTAMP_H
