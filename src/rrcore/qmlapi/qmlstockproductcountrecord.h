#ifndef QMLSTOCKPRODUCTCOUNTRECORD_H
#define QMLSTOCKPRODUCTCOUNTRECORD_H

#include "detailrecord/abstractdetailrecord.h"

class QMLStockProductCountRecord : public AbstractDetailRecord
{
    Q_OBJECT
    Q_PROPERTY(QString filterText READ filterText WRITE setFilterText NOTIFY filterTextChanged)
    Q_PROPERTY(int filterColumn READ filterColumn WRITE setFilterColumn NOTIFY filterColumnChanged)
    Q_PROPERTY(int productCount READ productCount NOTIFY productCountChanged)
public:
    enum FilterColumns {
        CategoryColumn,
        ProductColumn
    }; Q_ENUM(FilterColumns)
    explicit QMLStockProductCountRecord(QObject *parent = nullptr);
    explicit QMLStockProductCountRecord(DatabaseThread &thread, QObject *parent = nullptr);

    int categoryId() const;
    void setCategoryId(int categoryId);

    QString filterText() const;
    void setFilterText(const QString &filterText);

    int filterColumn() const;
    void setFilterColumn(int filterColumn);

    int productCount() const;
public slots:
    void refresh() override;
signals:
    void categoryIdChanged();
    void filterTextChanged();
    void filterColumnChanged();

    void productCountChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    int m_categoryId;
    QString m_filterText;
    int m_filterColumn;
    int m_productCount;

    void setProductCount(int productCount);
    void filter();
    QString columnName(int column) const;
};

#endif // QMLSTOCKPRODUCTCOUNTRECORD_H
