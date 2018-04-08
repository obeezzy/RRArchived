#include "abstractsqlmanager.h"

AbstractSqlManager::AbstractSqlManager(QSqlDatabase connection) :
    m_connection(connection)
{

}

AbstractSqlManager::~AbstractSqlManager()
{

}

QVariantMap AbstractSqlManager::recordToMap(const QSqlRecord &record)
{
    QVariantMap map;
    for (int i = 0; i < record.count(); i++)
        map.insert(record.fieldName(i), record.value(i));

    return map;
}

QSqlRecord AbstractSqlManager::mapToRecord(const QVariantMap &map)
{
    QSqlRecord record;

    QMapIterator<QString, QVariant> mapIter(map);
    while (mapIter.hasNext()) {
        auto pair = mapIter.next();
        record.setValue(pair.key(), pair.value());
    }

    return record;
}
