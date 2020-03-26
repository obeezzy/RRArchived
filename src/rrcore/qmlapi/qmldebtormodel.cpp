#include "qmldebtormodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "queryexecutors/debtor.h"
#include <QDateTime>
#include <QDebug>

Q_LOGGING_CATEGORY(lcqmldebtormodel, "rrcore.qmlapi.qmldebtormodel");

QMLDebtorModel::QMLDebtorModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLDebtorModel::QMLDebtorModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{}

int QMLDebtorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_debtorList.count();
}

QVariant QMLDebtorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case ClientIdRole:
        return m_debtorList.at(index.row()).client.id;
    case DebtorIdRole:
        return m_debtorList.at(index.row()).id;
    case ImageUrlRole:
        return m_debtorList.at(index.row()).client.imageUrl;
    case PreferredNameRole:
        return m_debtorList.at(index.row()).client.preferredName;
    case TotalDebtRole:
        return m_debtorList.at(index.row()).totalDebt;
    case NoteRole:
        return m_debtorList.at(index.row()).note.note;
    case CreatedRole:
        return m_debtorList.at(index.row()).created;
    case LastEditedRole:
        return m_debtorList.at(index.row()).lastEdited;
    case UserRole:
        return m_debtorList.at(index.row()).user.user;
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtorModel::roleNames() const
{
    return {
        { ClientIdRole, "client_id" },
        { DebtorIdRole, "debtor_id" },
        { ImageUrlRole, "image_url" },
        { PreferredNameRole, "preferred_name" },
        { TotalDebtRole, "total_debt" },
        { NoteRole, "note" },
        { CreatedRole, "created" },
        { LastEditedRole, "last_edited" },
        { UserRole, "user" }
    };
}

void QMLDebtorModel::tryQuery()
{
    setBusy(true);
    emit execute(new DebtorQuery::ViewDebtors(this));
}

void QMLDebtorModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == DebtorQuery::ViewDebtors::COMMAND) {
            beginResetModel();
            m_debtorList = Utility::DebtorList{ result.outcome().toMap().value("debtors").toList() };
            endResetModel();
            emit success(ModelResult{ ViewDebtorsSuccess });
        } else if (result.request().command() == DebtorQuery::RemoveDebtor::COMMAND) {
            const auto &debtor = Utility::Debtor{ result.outcome().toMap() };
            removeDebtorFromModel(debtor);
            emit success(ModelResult{ RemoveDebtorSuccess });
        } else if (result.request().command() == DebtorQuery::RemoveDebtor::UNDO_COMMAND) {
            const auto &debtor = Utility::Debtor{ result.outcome().toMap() };
            undoRemoveDebtorFromModel(debtor);
            emit success(ModelResult{ UndoRemoveDebtorSuccess });
        }
    } else {
        emit error();
    }
}

void QMLDebtorModel::filter()
{
    if (filterColumn() == -1)
        return;

    setBusy(true);
    emit execute(new DebtorQuery::FilterDebtors(filterCriteria(),
                                                this));
}

void QMLDebtorModel::removeDebtor(int row)
{
    if (row < 0 || row >= m_debtorList.count()) {
        qCWarning(lcqmldebtormodel) << Q_FUNC_INFO << "Invalid row:" << row;
        return;
    }
    setBusy(true);

    Utility::Debtor &debtor = m_debtorList[row];
    debtor.row = row;

    emit execute(new DebtorQuery::RemoveDebtor(debtor, this));
}

void QMLDebtorModel::removeDebtorFromModel(const Utility::Debtor &debtor)
{
    if (debtor.row < 0 || debtor.row >= m_debtorList.count()) {
        qCWarning(lcqmldebtormodel) << Q_FUNC_INFO << "Invalid row:" << debtor.row << debtor;
        return;
    }

    beginRemoveRows(QModelIndex(), debtor.row, debtor.row);
    m_debtorList.removeAt(debtor.row);
    endRemoveRows();
}

void QMLDebtorModel::undoRemoveDebtorFromModel(const Utility::Debtor &debtor)
{
    beginInsertRows(QModelIndex(), debtor.row, debtor.row);
    m_debtorList.insert(debtor.row, debtor);
    endInsertRows();
}

QString QMLDebtorModel::columnName(int column) const
{
    switch (column) {
    case PreferredNameColumn:
        return QStringLiteral("preferred_name");
    case TotalDebtColumn:
        return QStringLiteral("total_debt");
    }

    return QString();
}

void QMLDebtorModel::undoLastCommit()
{
    QueryRequest request{ lastSuccessfulRequest() };
    QVariantMap params{ lastSuccessfulRequest().params() };
    if (lastSuccessfulRequest().command() == DebtorQuery::RemoveDebtor::COMMAND) {
        setBusy(true);
        auto query = new DebtorQuery::RemoveDebtor(request, this);
        query->undoOnNextExecution(true);
        emit execute(query);
    }
}

