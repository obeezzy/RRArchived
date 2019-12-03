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

const QString CONNECTION_NAME(QStringLiteral("db_thread"));

Q_LOGGING_CATEGORY(databaseThread, "rrcore.database.databasethread");

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
    QSharedPointer<QueryExecutor> executor(queryExecutor, &QueryExecutor::deleteLater);
    qCInfo(databaseThread) << "DatabaseWorker->" << executor->request();
    const QueryRequest &request(executor->request());
    QueryResult result{ request };

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.command().trimmed().isEmpty())
            throw DatabaseException(DatabaseError::QueryErrorCode::NoCommand, "No command set.");

        executor->setConnectionName(CONNECTION_NAME);
        result = executor->execute();
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
        qCCritical(databaseThread) << "DatabaseException in Worker->" << e.code() << e.message() << e.userMessage();
    }

    emit resultReady(result);
    qCInfo(databaseThread) << "DatabaseWorker->" << result << " [elapsed = " << timer.elapsed() << " ms]";
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
            connect(this, &DatabaseThread::resultReady, &NetworkThread::instance(), &NetworkThread::syncWithServer);

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
    connect(QCoreApplication::instance(), &QCoreApplication::aboutToQuit, &instance, &DatabaseThread::quit);
    return instance;
}

void DatabaseThread::run()
{
    exec();
}
