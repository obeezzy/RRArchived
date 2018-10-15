#include "qmldatabasecreator.h"
#include "database/databasecreator.h"

#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>

QMLDatabaseCreator::QMLDatabaseCreator(QObject *parent) : QObject(parent)
{
    m_databaseCreator = new DatabaseCreator();
}

void QMLDatabaseCreator::start()
{
    if (!m_futureWatcher.isRunning()) {
        qDebug() << "Called!";
        disconnect(&m_futureWatcher, &QFutureWatcher<bool>::finished, this, &QMLDatabaseCreator::onFinished);
        connect(&m_futureWatcher, &QFutureWatcher<bool>::finished, this, &QMLDatabaseCreator::onFinished);
        m_futureWatcher.setFuture(QtConcurrent::run(m_databaseCreator, &DatabaseCreator::start));
    }
}

void QMLDatabaseCreator::onFinished()
{
    qDebug() << "What is the result of database creation? " << m_futureWatcher.result();
}
