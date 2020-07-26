#include "qmldatabasecreator.h"
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrent>
#include "database/databasecreator.h"

Q_LOGGING_CATEGORY(lcqmldatabasecreator, "rrcore.qmlapi.qmldatabasecreator");

QMLDatabaseCreator::QMLDatabaseCreator(QObject* parent) : QObject(parent)
{
    m_databaseCreator = new DatabaseCreator();
}

bool QMLDatabaseCreator::busy() const
{
    return m_busy;
}

void QMLDatabaseCreator::start()
{
    if (!m_futureWatcher.isRunning()) {
        qCInfo(lcqmldatabasecreator) << "Building database...";
        disconnect(&m_futureWatcher, &QFutureWatcher<bool>::finished, this,
                   &QMLDatabaseCreator::onFinished);
        connect(&m_futureWatcher, &QFutureWatcher<bool>::finished, this,
                &QMLDatabaseCreator::onFinished);
        m_futureWatcher.setFuture(
            QtConcurrent::run(m_databaseCreator, &DatabaseCreator::start));
        setBusy(true);
    } else {
        qCWarning(lcdatabasecreator) << "Start called while process running. ";
    }
}

void QMLDatabaseCreator::setBusy(bool busy)
{
    if (m_busy == busy)
        return;

    m_busy = busy;
    emit busyChanged();
}

void QMLDatabaseCreator::onFinished()
{
    setBusy(false);

    if (m_futureWatcher.result())
        emit success();
    else
        emit error();
}
