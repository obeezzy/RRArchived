#include "recordtimestamp.h"
#include <QVariantMap>

using namespace Utility;

RecordTimestamp::RecordTimestamp()
{}

RecordTimestamp::RecordTimestamp(const QDateTime &created,
                                 const QDateTime &lastEdited) :
    created(created),
    lastEdited(lastEdited)
{}

RecordTimestamp::RecordTimestamp(const QVariantMap &map) :
    created(map.value("created").toDateTime()),
    lastEdited(map.value("last_edited").toDateTime())
{}

QVariantMap RecordTimestamp::toVariantMap() const
{
    return {
        { "created", created },
        { "last_edited", lastEdited }
    };
}
