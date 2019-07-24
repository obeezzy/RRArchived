#include "qmlincomereportmodel.h"
#include "database/databasethread.h"

const int COLUMN_COUNT = 4;

QMLIncomeReportModel::QMLIncomeReportModel(QObject *parent) :
    QMLIncomeReportModel(DatabaseThread::instance(), parent)
{}

QMLIncomeReportModel::QMLIncomeReportModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualTableModel(thread, parent)
{

}

int QMLIncomeReportModel::rowCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return m_records.count();
}

int QMLIncomeReportModel::columnCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return COLUMN_COUNT;
}

QVariant QMLIncomeReportModel::data(const QModelIndex &index, int role) const
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

QHash<int, QByteArray> QMLIncomeReportModel::roleNames() const
{
    return {
        { PurposeRole, "purpose" },
        { AmountRole, "amount" }
    };
}

void QMLIncomeReportModel::tryQuery()
{
    setBusy(true);
    QueryRequest request(this);
    request.setCommand("view_income_report", { }, QueryRequest::Income);
    emit executeRequest(request);
}

void QMLIncomeReportModel::processResult(const QueryResult result)
{
    if (this != result.request().receiver())
        return;

    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == "view_income_report") {
            beginResetModel();
            m_records = result.outcome().toMap().value("transactions").toList();
            endResetModel();
            emit success(ViewIncomeReportSuccess);
        } else {
            emit success();
        }
    } else {

    }
}
