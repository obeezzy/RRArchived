#ifndef QMLSTOCKCATEGORYMODEL_H
#define QMLSTOCKCATEGORYMODEL_H

#include <QObject>
#include <QVariantList>
#include "models/abstractvisuallistmodel.h"

class QMLStockCategoryModel : public AbstractVisualListModel
{
    Q_OBJECT
public:
    explicit QMLStockCategoryModel(QObject *parent = nullptr);

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
private:
    QVariantList m_records;
};

#endif // QMLSTOCKCATEGORYMODEL_H
