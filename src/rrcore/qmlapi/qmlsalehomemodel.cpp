#include "qmlsalehomemodel.h"
#include "database/queryrequest.h"
#include "database/queryresult.h"
#include "database/databasethread.h"
#include "utility/saleutils.h"
#include "queryexecutors/sales.h"

QMLSaleHomeModel::QMLSaleHomeModel(QObject *parent) :
    QMLSaleHomeModel(DatabaseThread::instance(), parent)
{}

QMLSaleHomeModel::QMLSaleHomeModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{}

int QMLSaleHomeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_records.count();
}

QVariant QMLSaleHomeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case DataTypeRole:
        return m_records.at(index.row()).toMap().value("data_type").toString();
    case DataModelRole:
        return QVariant::fromValue<QObject *>(m_dataModels.at(index.row()));
    }

    return QVariant();
}

QHash<int, QByteArray> QMLSaleHomeModel::roleNames() const
{
    QHash<int, QByteArray> roles(AbstractVisualListModel::roleNames());
    roles.insert(DataTypeRole, "data_type");
    roles.insert(DataModelRole, "data_model");

    return roles;
}


void QMLSaleHomeModel::tryQuery()
{
    setBusy(true);
    emit execute(new SaleQuery::ViewSaleHome(this));
}

bool QMLSaleHomeModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLSaleHomeModel::processResult(const QueryResult &result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        qDeleteAll(m_dataModels);
        m_dataModels.clear();

        m_records = result.outcome().toMap().value("records").toList();

        for (const QVariant &r : m_records) {
            const QVariantMap record = r.toMap();

            if (record.value("data_type").toString() == "total_revenue") {
                // Missing implementation
            } else if (record.value("data_type").toString() == "most_sold_products") {
                // Missing implementation
            }
        }

        endResetModel();

        emit success();
    } else {
        emit error();
    }
}
