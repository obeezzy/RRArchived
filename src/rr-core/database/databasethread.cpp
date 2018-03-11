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
#include "rr-core/sql_manager/usersqlmanager.h"
#include "rr-core/sql_manager/dashboardsqlmanager.h"
#include "rr-core/sql_manager/stocksqlmanager.h"
#include "rr-core/sql_manager/salesqlmanager.h"

DatabaseThread *DatabaseThread::m_instance = nullptr;

DatabaseThread::DatabaseThread(QObject *parent) :
    QThread(parent)
{

}

DatabaseThread::~DatabaseThread()
{
    m_connection.close();
}

DatabaseThread &DatabaseThread::instance()
{
    if (m_instance == nullptr) {
        m_instance = new DatabaseThread;
        connect(qApp, &QCoreApplication::aboutToQuit, m_instance, &DatabaseThread::quit);
        connect(m_instance, &DatabaseThread::finished, m_instance, &DatabaseThread::deleteLater);
    }

    return *m_instance;
}

void DatabaseThread::run()
{
    exec();
}

void DatabaseThread::execute(const QueryRequest &request)
{
    qDebug() << Q_FUNC_INFO << request << ", sender=" << sender();
    QueryResult result;

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.command().trimmed().isEmpty())
            throw DatabaseException(DatabaseException::NoCommand);

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
        default:
            qWarning() << "Unhandled request in DatabaseThread::execute() ->" << request.command() << request.type();
            break;
        }

        if (!result.isSuccessful())
            throw DatabaseException(DatabaseException::UnsuccessfulQueryResult, result.errorMessage(), result.errorUserMessage());
    } catch (DatabaseException &e) {
        qDebug() << "DatabaseException in DatabaseThread:" << e.message() << e.userMessage();
    }

    emit resultsReady(result);
    qDebug() << Q_FUNC_INFO << result << " [elapsed = " << timer.elapsed() << " ms]";
}
