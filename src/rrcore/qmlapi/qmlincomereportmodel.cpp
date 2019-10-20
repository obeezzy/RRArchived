#include "qmlincomereportmodel.h"
#include "database/databasethread.h"

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

    return ColumnCount;
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

QVariant QMLIncomeReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {
        if (role == Qt::DisplayRole) {
            switch (section) {
            case PurposeColumn:
                return tr("Purpose");
            case AmountColumn:
                return tr("Amount");
            case ActionColumn:
                return tr("Action");
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case PurposeColumn:
                return Qt::AlignLeft;
            case AmountColumn:
                return Qt::AlignRight;
            case ActionColumn:
                return Qt::AlignHCenter;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case PurposeColumn:
                return 500;
            case AmountColumn:
                return 200;
            case ActionColumn:
                return 220;
            }
        }
    }

    return section + 1;
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
