#include "qmlexpensereportmodel.h"
#include "database/databasethread.h"

const int COLUMN_COUNT = 4;

QMLExpenseReportModel::QMLExpenseReportModel(QObject *parent) :
    QMLExpenseReportModel(DatabaseThread::instance(), parent)
{}

QMLExpenseReportModel::QMLExpenseReportModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{

}

int QMLExpenseReportModel::rowCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return m_records.count();
}

int QMLExpenseReportModel::columnCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return COLUMN_COUNT;
}

QVariant QMLExpenseReportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case PurposeRole:
        return m_records.at(index.row()).toMap().value("purpose").toString();
    case AmountRole:
        return m_records.at(index.row()).toMap().value("amount").toDouble();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLExpenseReportModel::roleNames() const
{
    return {
        { PurposeRole, "purpose" },
        { AmountRole, "amount" }
    };
}

void QMLExpenseReportModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_expense_report", { }, QueryRequest::Expense);
    emit executeRequest(request);
}

void QMLExpenseReportModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == "view_expense_report") {
            beginResetModel();
            m_records = result.outcome().toMap().value("transactions").toList();
            endResetModel();
            emit success(ViewExpenseReportSuccess);
        } else {
            emit success();
        }
    } else {

    }
}
