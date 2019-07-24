#ifndef QMLSALEHOMEMODEL_H
#define QMLSALEHOMEMODEL_H

#include <QObject>
#include <QVariantList>
#include <QVariantMap>
#include "models/abstractvisuallistmodel.h"

class QMLSaleHomeModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        DataTypeRole = Qt::UserRole,
        DataModelRole
    };

    explicit QMLSaleHomeModel(QObject *parent = nullptr);
    explicit QMLSaleHomeModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    virtual void tryQuery() override final;
    virtual void processResult(const QueryResult result) override final;
private:
    QVariantList m_records;
    QList<AbstractVisualListModel *> m_dataModels;
};

#endif // QMLSALEHOMEMODEL_H
