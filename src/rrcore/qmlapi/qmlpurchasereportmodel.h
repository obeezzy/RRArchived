#ifndef QMLPURCHASEREPORTMODEL_H
#define QMLPURCHASEREPORTMODEL_H

#include "models/abstractvisualtablemodel.h"

class QMLPurchaseReportModel : public AbstractVisualTableModel
{
public:
    enum Roles {
        CategoryRole = Qt::UserRole,
        ItemRole,
        QuantityBoughtRole,
        TotalAmountRole,
        UnitRole
    };

    enum SuccessCode {
        ViewPurchaseReportSuccess
    }; Q_ENUM(SuccessCode)

    explicit QMLPurchaseReportModel(QObject *parent = nullptr);
    explicit QMLPurchaseReportModel(DatabaseThread &thread);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    QVariantList m_records;
};

#endif // QMLPURCHASEREPORTMODEL_H
