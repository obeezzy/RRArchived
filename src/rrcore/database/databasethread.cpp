#include "databasethread.h"
#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QLoggingCategory>
#include <QSettings>
#include "database/exceptions/exceptions.h"
#include "databaseexception.h"
#include "network/networkthread.h"
#include "query/user/userexecutor.h"
#include "queryrequest.h"
#include "queryresult.h"
#include "user/userprofile.h"

Q_LOGGING_CATEGORY(lcdatabasethread, "rrcore.database.databasethread");

const QString CONNECTION_NAME(QStringLiteral("db_thread"));

DatabaseWorker::DatabaseWorker(QObject* parent) : QObject(parent) {}

DatabaseWorker::~DatabaseWorker()
{
    QSqlDatabase connection = QSqlDatabase::database(CONNECTION_NAME);
    connection.close();
}

void DatabaseWorker::execute(QueryExecutor* queryExecutor)
{
    qCInfo(lcdatabasethread) << queryExecutor->request();
    const auto request = QueryRequest{queryExecutor->request()};
    auto result = QueryResult{request};

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.command().trimmed().isEmpty())
            throw MissingCommandException(request.command());

        queryExecutor->setConnectionName(CONNECTION_NAME);
        result = queryExecutor->execute();
    } catch (const DatabaseException& e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorDetails(e.sqlError().databaseText());
        qCCritical(lcdatabasethread) << e;
    }

    queryExecutor->deleteLater();
    emit resultReady(result);
    qCInfo(lcdatabasethread)
        << result << " [elapsed = " << timer.elapsed() << " ms]";
}

DatabaseThread::DatabaseThread(QObject* parent) : QThread(parent)
{
    if (!isRunning()) {
        if (UserProfile::instance().isServerTunnelingEnabled()) {
            connect(this, &DatabaseThread::execute, &NetworkThread::instance(),
                    &NetworkThread::tunnelToServer);
            connect(&NetworkThread::instance(), &NetworkThread::resultReady,
                    this, &DatabaseThread::resultReady);
        } else {
            DatabaseWorker* worker = new DatabaseWorker;

            connect(worker, &DatabaseWorker::resultReady, this,
                    &DatabaseThread::resultReady);
            connect(this, &DatabaseThread::execute, worker,
                    &DatabaseWorker::execute);
            connect(this, &DatabaseThread::finished, worker,
                    &DatabaseWorker::deleteLater);
            connect(this, &DatabaseThread::resultReady,
                    &NetworkThread::instance(), &NetworkThread::syncWithServer);

            worker->moveToThread(this);
            start();
        }
    }
}

DatabaseThread::DatabaseThread(QueryResult*, QObject* parent) : QThread(parent)
{}

DatabaseThread::~DatabaseThread()
{
    quit();
    wait();
}

DatabaseThread& DatabaseThread::instance()
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
