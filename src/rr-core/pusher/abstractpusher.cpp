#include "abstractpusher.h"

#include "rr-core/database/databasethread.h"

AbstractPusher::AbstractPusher(QObject *parent)
    : m_busy(false)
    , QObject(parent)
{
    connect(this, &AbstractPusher::executeRequest, &DatabaseThread::instance(), &DatabaseThread::execute);
    connect(&DatabaseThread::instance(), &DatabaseThread::resultsReady, this, &AbstractPusher::processResult);
}

AbstractPusher::~AbstractPusher()
{

}

bool AbstractPusher::isBusy() const
{
    return m_busy;
}

void AbstractPusher::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}
