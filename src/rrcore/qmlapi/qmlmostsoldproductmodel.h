#ifndef QMLMOSTSOLDPRODUCTMODEL_H
#define QMLMOSTSOLDPRODUCTMODEL_H

#include "models/abstractvisuallistmodel.h"
#include <QVariantList>

class DatabaseThread;

class QMLMostSoldProductModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole,
        ProductIdRole,
        ProductRole,
        TotalRevenueRole,
        TotalQuantityRole,
        UnitIdRole,
        UnitRole
    };

    explicit QMLMostSoldProductModel(QObject *parent = nullptr);
    explicit QMLMostSoldProductModel(DatabaseThread &thread, QObject *parent = nullptr);
    ~QMLMostSoldProductModel() override = default;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    bool canProcessResult(const QueryResult &result) const override;
    void processResult(const QueryResult &result) override final;
private:
    QVariantList m_records;
};

#endif // QMLMOSTSOLDPRODUCTMODEL_H
