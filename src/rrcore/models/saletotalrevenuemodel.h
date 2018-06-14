#ifndef SALETOTALREVENUEMODEL_H
#define SALETOTALREVENUEMODEL_H

#include <QObject>
#include <QVariantList>
#include "models/abstractvisuallistmodel.h"

class SaleTotalRevenueModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        CreatedRole = Qt::UserRole,
        AmountPaidRole
    };

    explicit SaleTotalRevenueModel(const QVariantList &records, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
private:
    QVariantList m_records;
};

#endif // SALETOTALREVENUEMODEL_H
