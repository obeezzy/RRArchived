#include "qmldebtormodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include <QDateTime>

QMLDebtorModel::QMLDebtorModel(QObject *parent) :
    AbstractVisualListModel(DatabaseThread::instance(), parent)
{}

QMLDebtorModel::QMLDebtorModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{

}

int QMLDebtorModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLDebtorModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case ClientIdRole:
        return m_records.at(index.row()).toMap().value("client_id").toInt();
    case DebtorIdRole:
        return m_records.at(index.row()).toMap().value("debtor_id").toInt();
    case ImageSourceRole:
        return m_records.at(index.row()).toMap().value("image_source").toString();
    case PreferredNameRole:
        return m_records.at(index.row()).toMap().value("preferred_name").toString();
    case TotalDebtRole:
        return m_records.at(index.row()).toMap().value("total_debt").toDouble();
    case NoteRole:
        return m_records.at(index.row()).toMap().value("note").toString();
    case CreatedRole:
        return m_records.at(index.row()).toMap().value("created").toDateTime();
    case LastEditedRole:
        return m_records.at(index.row()).toMap().value("last_edited").toDateTime();
    case UserRole:
        return m_records.at(index.row()).toMap().value("user").toString();
    }

    return QVariant();
}

QHash<int, QByteArray> QMLDebtorModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(ClientIdRole, "client_id");
    roles.insert(DebtorIdRole, "debtor_id");
    roles.insert(ImageSourceRole, "image_source");
    roles.insert(PreferredNameRole, "preferred_name");
    roles.insert(TotalDebtRole, "total_debt");
    roles.insert(NoteRole, "note");
    roles.insert(CreatedRole, "created");
    roles.insert(LastEditedRole, "last_edited");
    roles.insert(UserRole, "user");

    return roles;
}

void QMLDebtorModel::tryQuery()
{
    setBusy(true);

    QueryRequest request(this);
    request.setCommand("view_debtors", QVariantMap(), QueryRequest::Debtor);

    emit executeRequest(request);
}

void QMLDebtorModel::processResult(const QueryResult result)
{
    if (result.request().receiver() != this)
        return;

    setBusy(false);

    if (result.isSuccessful()) {
        if (result.request().command() == "view_debtors") {
            beginResetModel();
            m_records = result.outcome().toMap().value("debtors").toList();
            endResetModel();
            emit success(ViewDebtorsSuccess);
        } else if (result.request().command() == "remove_debtor") {
            removeItemFromModel(result.outcome().toMap().value("debtor_id").toInt());
            emit success(RemoveDebtorSuccess);
        } else if (result.request().command() == "undo_remove_debtor") {
            undoRemoveItemFromModel(result.outcome().toMap().value("debtor_row").toInt(),
                                    result.outcome().toMap().value("debtor_id").toInt(),
                                    result.outcome().toMap().value("debtor").toMap());
            emit success(UndoRemoveDebtorSuccess);
        }
    } else {
        emit error(UnknownError);
    }
}

void QMLDebtorModel::filter()
{
    if (filterColumn() == -1)
        return;

    setBusy(true);

    QueryRequest request(this);
    QVariantMap params;

    params.insert("filter_text", filterText());

    switch (filterColumn()) {
    case PreferredNameColumn:
        params.insert("filter_column", "preferred_name");
        break;
    default:
        params.insert("filter_column", "total_debt");
        break;
    }

    request.setCommand("view_debtors", params, QueryRequest::Debtor);
    emit executeRequest(request);
}

void QMLDebtorModel::removeDebtor(int debtorId)
{
    if (debtorId <= 0) {
        emit error(InvalidDebtorError);
        return;
    }

    setBusy(true);

    QueryRequest request(this);
    request.setCommand("remove_debtor",
                       QVariantMap { { "debtor_id", debtorId }, { "can_undo", true }, { "debtor_row", debtorRowFromId(debtorId) } },
                       QueryRequest::Debtor);
    emit executeRequest(request);
}

int QMLDebtorModel::debtorRowFromId(int debtorId)
{
    if (debtorId <= 0)
        return -1;

    for (int i = 0; i < rowCount(); ++i) {
        if (index(i).data(DebtorIdRole).toInt() == debtorId)
            return i;
    }

    return -1;
}

void QMLDebtorModel::removeItemFromModel(int debtorId)
{
    if (debtorId <= 0)
        return;

    for (int i = 0; i < m_records.count(); ++i) {
        const QVariantMap &record = m_records.at(i).toMap();

        if (record.value("debtor_id").toInt() == debtorId) {
            beginRemoveRows(QModelIndex(), i, i);
            m_records.removeAt(i);
            endRemoveRows();
        }
    }
}

void QMLDebtorModel::undoRemoveItemFromModel(int row, int debtorId, const QVariantMap &debtorInfo)
{
    if (debtorId <= 0)
        return;

    beginInsertRows(QModelIndex(), row, row);
    m_records.insert(row, debtorInfo);
    endInsertRows();
}

