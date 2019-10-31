#ifndef QMLSALEREPORTMODEL_H
#define QMLSALEREPORTMODEL_H

#include "models/abstractvisualtablemodel.h"
#include <QVariantList>

class QMLSaleReportModel : public AbstractVisualTableModel
{
    Q_OBJECT
public:
    enum Roles {
        CategoryRole = Qt::UserRole,
        ItemRole,
        QuantitySoldRole,
        TotalAmountRole,
        UnitRole
    };

    enum Columns {
        CategoryColumn,
        ItemColumn,
        QuantitySoldColumn,
        TotalAmountColumn,
        ColumnCount
    }; Q_ENUM(Columns)

    enum SuccessCode {
        ViewSalesReportSuccess
    }; Q_ENUM(SuccessCode)

    explicit QMLSaleReportModel(QObject *parent = nullptr);
    explicit QMLSaleReportModel(DatabaseThread &thread, QObject *parent = nullptr);

    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    int columnCount(const QModelIndex &index = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
};

#endif // QMLSALEREPORTMODEL_H
