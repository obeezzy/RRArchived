#include "databasethread.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>
#include <QLoggingCategory>
#include <QSettings>

#include "databaseexception.h"
#include "queryrequest.h"
#include "queryresult.h"
#include "network/networkthread.h"
#include "user/userprofile.h"
#include "queryexecutors/user/userexecutor.h"

Q_LOGGING_CATEGORY(databaseThread, "rrcore.database.databasethread");

const QString CONNECTION_NAME(QStringLiteral("db_thread"));

DatabaseWorker::DatabaseWorker(QObject *parent) :
    QObject(parent)
{
}

DatabaseWorker::~DatabaseWorker()
{
    QSqlDatabase connection = QSqlDatabase::database(CONNECTION_NAME);
    connection.close();
}

void DatabaseWorker::execute(QueryExecutor *queryExecutor)
{
    qCInfo(databaseThread) << queryExecutor->request();
    const QueryRequest &request(queryExecutor->request());
    QueryResult result{ request };

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.command().trimmed().isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::NoCommand);

        queryExecutor->setConnectionName(CONNECTION_NAME);
        result = queryExecutor->execute();
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
        qCCritical(databaseThread) << e;
    }

    queryExecutor->deleteLater();
    emit resultReady(result);
    qCInfo(databaseThread) << result << " [elapsed = " << timer.elapsed() << " ms]";
}

DatabaseThread::DatabaseThread(QObject *parent) :
    QThread(parent)
{
    if (!isRunning()) {
        if (UserProfile::instance().isServerTunnelingEnabled()) {
            connect(this, &DatabaseThread::execute,
                    &NetworkThread::instance(), &NetworkThread::tunnelToServer);
            connect(&NetworkThread::instance(), &NetworkThread::resultReady,
                    this, &DatabaseThread::resultReady);
        } else {
            DatabaseWorker *worker = new DatabaseWorker;

            connect(worker, &DatabaseWorker::resultReady, this, &DatabaseThread::resultReady);
            connect(this, &DatabaseThread::execute, worker, &DatabaseWorker::execute);
            connect(this, &DatabaseThread::finished, worker, &DatabaseWorker::deleteLater);
            connect(this, &DatabaseThread::resultReady,
                    &NetworkThread::instance(), &NetworkThread::syncWithServer);

            worker->moveToThread(this);
            start();
        }
    }
}

DatabaseThread::DatabaseThread(QueryResult *, QObject *parent) :
    QThread(parent)
{
}

DatabaseThread::~DatabaseThread()
{
    quit();
    wait();
}

DatabaseThread &DatabaseThread::instance()
{
    static DatabaseThread instance;
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit,
            &instance, &DatabaseThread::quit);
    return instance;
}

void DatabaseThread::run()
{
    exec();
}
