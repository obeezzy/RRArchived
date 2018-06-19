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

void Worker::initialize()
{

}

void Worker::execute(const QueryRequest request)
{
    qDebug() << Q_FUNC_INFO << request << ", sender=" << request.receiver();
    QueryResult result;

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.command().trimmed().isEmpty())
            throw DatabaseException(DatabaseException::RRErrorCode::NoCommand, "No command set.");

        if (!m_connection.isValid() && QSqlDatabase::database().isValid())
            m_connection = QSqlDatabase::database();

        switch (request.type()) {
        case QueryRequest::User:
            result = UserSqlManager(m_connection).execute(request);
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
            qWarning() << "Unhandled request in DatabaseThread::execute() ->" << request.command() << request.type();
            break;
        }

        if (!result.isSuccessful())
            throw DatabaseException(DatabaseException::RRErrorCode::UnsuccessfulQueryResult, result.errorMessage(), result.errorUserMessage());
    } catch (DatabaseException &e) {
        qDebug() << "DatabaseException in DatabaseThread:" << e.message() << e.userMessage();
    }

    emit resultReady(result);
    qDebug() << Q_FUNC_INFO << result << " [elapsed = " << timer.elapsed() << " ms]";
}

DatabaseThread::DatabaseThread(QObject *parent) :
    QThread(parent)
{
    if (!isRunning()) {
        Worker *worker = new Worker;

        connect(worker, &Worker::resultReady, this, &DatabaseThread::resultReady);
        connect(this, &DatabaseThread::execute, worker, &Worker::execute);
        connect(this, &DatabaseThread::finished, worker, &Worker::deleteLater);
        connect(this, &DatabaseThread::started, worker, &Worker::initialize);

        worker->moveToThread(this);
        start();
    }
}

DatabaseThread::~DatabaseThread()
{
    quit();
}

DatabaseThread &DatabaseThread::instance()
{
    static DatabaseThread instance;
    return instance;
}

void DatabaseThread::run()
{
    exec();
}
