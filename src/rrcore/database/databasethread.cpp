#include "databasethread.h"
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>
#include <QLoggingCategory>

#include "databaseexception.h"
#include "queryrequest.h"
#include "queryresult.h"
#include "sqlmanager/usersqlmanager.h"
#include "sqlmanager/dashboardsqlmanager.h"
#include "sqlmanager/stocksqlmanager.h"
#include "sqlmanager/salesqlmanager.h"
#include "sqlmanager/debtorsqlmanager.h"
#include "sqlmanager/clientsqlmanager.h"
#include "sqlmanager/purchasesqlmanager.h"
#include "sqlmanager/incomesqlmanager.h"
#include "sqlmanager/expensesqlmanager.h"
#include "network/networkthread.h"

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

void DatabaseWorker::execute(const QueryRequest request)
{
    qCInfo(databaseThread) << "DatabaseWorker->" << request;
    QueryResult result;

    QElapsedTimer timer;
    timer.start();

    try {
        if (request.command().trimmed().isEmpty())
            throw DatabaseException(DatabaseException::RRErrorCode::NoCommand, "No command set.");

        switch (request.type()) {
        case QueryRequest::User:
            result = UserSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Client:
            result = ClientSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Dashboard:
            result = DashboardSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Stock:
            result = StockSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Sales:
            result = SaleSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Purchase:
            result = PurchaseSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Income:
            result = IncomeSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Expense:
            result = ExpenseSqlManager(CONNECTION_NAME).execute(request);
            break;
        case QueryRequest::Debtor:
            result = DebtorSqlManager(CONNECTION_NAME).execute(request);
            break;
        default:
            throw DatabaseException(DatabaseException::RRErrorCode::RequestTypeNotFound, "Unhandled request type.");
        }

        if (!result.isSuccessful())
            throw DatabaseException(result.errorCode(), result.errorMessage(), result.errorUserMessage());
    } catch (DatabaseException &e) {
        result.setSuccessful(false);
        result.setErrorCode(e.code());
        result.setErrorMessage(e.message());
        result.setErrorUserMessage(e.userMessage());
        qCCritical(databaseThread) << "DatabaseException in Worker->" << e.code() << e.message() << e.userMessage();
    }

    emit resultReady(result);
    qInfo() << "DatabaseWorker->" << result << " [elapsed = " << timer.elapsed() << " ms]";
}

DatabaseThread::DatabaseThread(QObject *parent) :
    QThread(parent)
{
    if (!isRunning()) {
        DatabaseWorker *worker = new DatabaseWorker;

        connect(worker, &DatabaseWorker::resultReady, this, &DatabaseThread::resultReady);
        connect(this, &DatabaseThread::execute, worker, &DatabaseWorker::execute);
        connect(this, &DatabaseThread::finished, worker, &DatabaseWorker::deleteLater);

        connect(this, &DatabaseThread::resultReady, &NetworkThread::instance(), &NetworkThread::syncWithServer);

        worker->moveToThread(this);
        start();
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
