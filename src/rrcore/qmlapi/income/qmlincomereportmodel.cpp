#include "qmlincomereportmodel.h"
#include "database/databasethread.h"
#include "query/income/viewincomereport.h"
#include "utility/income/incomereporttransaction.h"

QMLIncomeReportModel::QMLIncomeReportModel(QObject *parent) :
    QMLIncomeReportModel(DatabaseThread::instance(), parent)
{}

QMLIncomeReportModel::QMLIncomeReportModel(DatabaseThread &thread, QObject *parent) :
    AbstractReportModel(thread, parent)
{

}

int QMLIncomeReportModel::rowCount(const QModelIndex &index) const
{
    if (index.isValid())
        return 0;

    return m_transactions.count();
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
        return m_transactions.at(index.row()).purpose;
    case AmountRole:
        return m_transactions.at(index.row()).amount.toDouble();
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
            }
        } else if (role == Qt::TextAlignmentRole) {
            switch (section) {
            case PurposeColumn:
                return Qt::AlignLeft;
            case AmountColumn:
                return Qt::AlignRight;
            }
        } else if (role == Qt::SizeHintRole) {
            switch (section) {
            case PurposeColumn:
                return tableViewWidth() - 200;
            case AmountColumn:
                return 200;
            }
        }
    }

    return section + 1;
}

void QMLIncomeReportModel::tryQuery()
{
    setBusy(true);
    emit execute(new Query::Income::ViewIncomeReport(dateTimeSpan(),
                                                       this));
}

bool QMLIncomeReportModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLIncomeReportModel::processResult(const QueryResult &result)
{
    setBusy(false);
    if (result.isSuccessful()) {
        if (result.request().command() == Query::Income::ViewIncomeReport::COMMAND) {
            beginResetModel();
            m_transactions = Utility::Income::IncomeReportTransactionList{ result.outcome().toMap().value("transactions").toList() };
            endResetModel();
            emit success();
        } else {
            emit error();
        }
    } else {

    }
}
