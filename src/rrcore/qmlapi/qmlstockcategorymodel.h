#ifndef QMLSTOCKCATEGORYMODEL_H
#define QMLSTOCKCATEGORYMODEL_H

#include <QObject>
#include <QVariantList>
#include "models/abstractvisuallistmodel.h"

class QMLStockCategoryModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(QString itemFilterText READ itemFilterText WRITE setItemFilterText NOTIFY itemFilterTextChanged)
public:
    explicit QMLStockCategoryModel(QObject *parent = nullptr);
    explicit QMLStockCategoryModel(DatabaseThread &thread, QObject *parent = nullptr);

    enum Roles {
        CategoryIdRole = Qt::UserRole,
        CategoryRole
    };

    enum SuccessCode {
        UnknownSuccess = -1,
        ViewStockCategoriesSuccess,
        RemoveStockCategorySuccess,
        UndoRemoveStockCategorySuccess,
        UnarchiveItemSuccess
    }; Q_ENUM(SuccessCode)

    QString itemFilterText() const;
    void setItemFilterText(const QString &itemFilterText);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override final;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;
    QHash<int, QByteArray> roleNames() const override final;

    Q_INVOKABLE void unarchiveItem(int itemId);
    Q_INVOKABLE void removeCategory(int row);
signals:
    void itemFilterTextChanged();
protected:
    void tryQuery() override final;
    void processResult(const QueryResult result) override final;
    void filter() override final;
private:
    QVariantList m_records;
    QString m_itemFilterText;

    void removeCategoryFromModel(int row);
    void undoRemoveCategoryFromModel(int row, const QVariantMap &categoryInfo);
    void updateCategory(int categoryId, const QVariantMap &categoryInfo);
    int rowFromCategoryId(int categoryId) const;
};

#endif // QMLSTOCKCATEGORYMODEL_H
