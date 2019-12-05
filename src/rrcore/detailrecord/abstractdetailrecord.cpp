#include "abstractdetailrecord.h"
#include "database/databasethread.h"
#include "database/queryexecutor.h"

AbstractDetailRecord::AbstractDetailRecord(QObject *parent) :
    AbstractDetailRecord(DatabaseThread::instance(), parent)
{}

AbstractDetailRecord::AbstractDetailRecord(DatabaseThread &thread, QObject *parent) :
    QObject(parent),
    m_autoQuery(true),
    m_busy(false)
{
    connect(this, &AbstractDetailRecord::execute, &thread, &DatabaseThread::execute);
    connect(&thread, &DatabaseThread::resultReady, this, &AbstractDetailRecord::processResult);
}

AbstractDetailRecord::~AbstractDetailRecord()
{

}

bool AbstractDetailRecord::autoQuery() const
{
    return m_autoQuery;
}

void AbstractDetailRecord::setAutoQuery(bool autoQuery)
{
    if (m_autoQuery == autoQuery)
        return;

    m_autoQuery = autoQuery;
    emit autoQueryChanged();
}

bool AbstractDetailRecord::isBusy() const
{
    return m_busy;
}

void AbstractDetailRecord::classBegin()
{

}

void AbstractDetailRecord::componentComplete()
{
    if (m_autoQuery)
        tryQuery();
}

void AbstractDetailRecord::refresh()
{

}

void AbstractDetailRecord::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}
