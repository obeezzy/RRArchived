#ifndef QMLSTOCKITEMCOUNTRECORD_H
#define QMLSTOCKITEMCOUNTRECORD_H

#include "detailrecord/abstractdetailrecord.h"

class QMLStockItemCountRecord : public AbstractDetailRecord
{
    Q_OBJECT
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(int filterColumn READ filterColumn WRITE setFilterColumn NOTIFY filterColumnChanged)
    Q_PROPERTY(int itemCount READ itemCount NOTIFY itemCountChanged)
public:
    enum FilterColumns {
        CategoryColumn,
        ItemColumn
    }; Q_ENUM(FilterColumns)
    explicit QMLStockItemCountRecord(QObject *parent = nullptr);
    explicit QMLStockItemCountRecord(DatabaseThread &thread, QObject *parent = nullptr);

    int categoryId() const;
    void setCategoryId(int categoryId);

    QString filterText() const;
    void setFilterText(const QString &filterText);

    int filterColumn() const;
    void setFilterColumn(int filterColumn);

    int itemCount() const;
public slots:
    void refresh() override;
signals:
    void categoryIdChanged();
    void filterTextChanged();
    void filterColumnChanged();

    void itemCountChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_categoryId;
    QString m_filterText;
    int m_filterColumn;
    int m_itemCount;

    void setItemCount(int itemCount);
    void filter();
    QString columnName(int column) const;
};

#endif // QMLSTOCKITEMCOUNTRECORD_H
