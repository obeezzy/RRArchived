#ifndef QMLSTOCKREPORTMODEL_H
#define QMLSTOCKREPORTMODEL_H

#include "models/abstractvisualtablemodel.h"
#include <QVariantList>

class QMLStockReportModel : public AbstractVisualTableModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryRole = Qt::UserRole,
        ItemRole,
        OpeningStockQuantityRole,
        QuantitySoldRole,
        QuantityBoughtRole,
        QuantityInStockRole,
        UnitRole
    };

    enum SuccessCode {
        ViewStockReportSuccess
    };

    enum Columns {
        CategoryColumn,
        ItemColumn,
        OpeningStockQuantityColumn,
        QuantitySoldColumn,
        QuantityBoughtColumn,
        QuantityInStockColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    explicit QMLStockReportModel(QObject *parent = nullptr);
    explicit QMLStockReportModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
};

#endif // QMLSTOCKREPORTMODEL_H
