#include "databasethread.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QSqlError>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QDebug>

#include "databaseserver.h"
#include "databaseexception.h"
#include "queryrequest.h"
#include "queryresult.h"
#include "sqlmanager/usersqlmanager.h"
#include "sqlmanager/dashboardsqlmanager.h"
#include "sqlmanager/stocksqlmanager.h"
#include "sqlmanager/salesqlmanager.h"
#include "sqlmanager/debtorsqlmanager.h"
#include "sqlmanager/clientsqlmanager.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>

Worker::Worker(QObject *parent) :
    QObject(parent)
{
}

Worker::~Worker()
{
    m_connection.close();
}

void Worker::execute(const QueryRequest request)
{
    qInfo() << "Worker->" << request << ", receiver=" << request.receiver();
    QueryResult result;

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.command().trimmed().isEmpty())
            throw DatabaseException(DatabaseException::RRErrorCode::NoCommand, "No command set.");

        if ((!m_connection.isValid() || !m_connection.isOpen()) && qEnvironmentVariable("TEST_GUI_ENABLED", "") == "1")
            connectToTestDatabase();

        switch (request.type()) {
        case QueryRequest::User:
            result = UserSqlManager(m_connection).execute(request);
            break;
        case QueryRequest::Client:
            result = ClientSqlManager(m_connection).execute(request);
            break;
        case QueryRequest::Dashboard:
            result = DashboardSqlManager(m_connection).execute(request);
            break;
        case QueryRequest::Stock:
            result = StockSqlManager(m_connection).execute(request);
            break;
        case QueryRequest::Sales:
            result = SaleSqlManager(m_connection).execute(request);
            break;
        case QueryRequest::Debtor:
            result = DebtorSqlManager(m_connection).execute(request);
            break;
        default:
            throw DatabaseException(DatabaseException::RRErrorCode::RequestTypeNotFound, "Unhandled request type.");
            break;
        }

        if (!result.isSuccessful())
            throw DatabaseException(result.errorCode(), result.errorMessage(), result.errorUserMessage());
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
        qCritical() << "DatabaseException in Worker->" << e.code() << e.message() << e.userMessage();
    }

    emit resultReady(result);
    qInfo() << "Worker->" << result << " [elapsed = " << timer.elapsed() << " ms]";
}

void Worker::connectToTestDatabase()
{
    try {
        if (!QSqlDatabase::contains())
            m_connection = QSqlDatabase::addDatabase("QMYSQL");
        else
            m_connection = QSqlDatabase::database();

        // Disconnect and connect to 'mysql'
        if (m_connection.isOpen())
            m_connection.close();

        m_connection.setDatabaseName("rr_test");
        m_connection.setHostName("localhost");
        m_connection.setPort(3306);
        m_connection.setUserName("root");
        m_connection.setPassword("hello");
        m_connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

        if (!m_connection.open())
            throw DatabaseException(DatabaseException::RRErrorCode::ConnectToTestDatabaseFailed, m_connection.lastError().text(), "Failed to connect to test database in worker thread.");

        qInfo() << "----------------TEST DATABASE MODE------------------------";
    } catch (DatabaseException &e) {
        qCritical() << "Exception caught:" << e.message() << e.userMessage();
        throw;
    }
}

DatabaseThread::DatabaseThread(QObject *parent) :
    QThread(parent)
{
    if (!isRunning()) {
        Worker *worker = new Worker;

        connect(worker, &Worker::resultReady, this, &DatabaseThread::resultReady);
        connect(this, &DatabaseThread::execute, worker, &Worker::execute);
        connect(this, &DatabaseThread::finished, worker, &Worker::deleteLater);

        worker->moveToThread(this);
        start();
    }
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
