#ifndef QMLSTOCKPRODUCTDETAILRECORD_H
#define QMLSTOCKPRODUCTDETAILRECORD_H

#include "detailrecord/abstractdetailrecord.h"
#include "utility/stock/stockproduct.h"

class QDateTime;
class QUrl;

class QMLStockProductDetailRecord : public AbstractDetailRecord
{
    Q_OBJECT
    Q_PROPERTY(int productId READ productId WRITE setProductId NOTIFY productIdChanged)
    Q_PROPERTY(int categoryId READ categoryId NOTIFY categoryIdChanged)
    Q_PROPERTY(QString category READ category NOTIFY categoryChanged)
    Q_PROPERTY(QString product READ product NOTIFY productChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool divisible READ isDivisible NOTIFY divisibleChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(double quantity READ quantity NOTIFY quantityChanged)
    Q_PROPERTY(int unitId READ unitId NOTIFY unitIdChanged)
    Q_PROPERTY(QString unit READ unit NOTIFY unitChanged)
    Q_PROPERTY(qreal costPrice READ costPrice NOTIFY costPriceChanged)
    Q_PROPERTY(qreal retailPrice READ retailPrice NOTIFY retailPriceChanged)
    Q_PROPERTY(QString currency READ currency NOTIFY currencyChanged)
    Q_PROPERTY(QDateTime created READ created NOTIFY createdChanged)
    Q_PROPERTY(QDateTime lastEdited READ lastEdited NOTIFY lastEditedChanged)
    Q_PROPERTY(int userId READ userId NOTIFY userIdChanged)
    Q_PROPERTY(QString user READ user NOTIFY userChanged)
public:
    explicit QMLStockProductDetailRecord(QObject *parent = nullptr);
    explicit QMLStockProductDetailRecord(DatabaseThread &thread, QObject *parent = nullptr);

    int productId() const;
    void setProductId(int productI);

    int categoryId() const;
    QString category() const;

    QString product() const;
    QString description() const;
    bool isDivisible() const;
    QUrl imageUrl() const;
    double quantity() const;
    int unitId() const;
    QString unit() const;
    qreal costPrice() const;
    qreal retailPrice() const;
    QString currency() const;

    QDateTime created() const;
    QDateTime lastEdited() const;

    int userId() const;
    QString user() const;
signals:
    void productIdChanged();
    void categoryIdChanged();
    void categoryChanged();
    void productChanged();
    void descriptionChanged();
    void divisibleChanged();
    void imageUrlChanged();
    void quantityChanged();
    void unitIdChanged();
    void unitChanged();
    void costPriceChanged();
    void retailPriceChanged();
    void currencyChanged();
    void createdChanged();
    void lastEditedChanged();
    void userIdChanged();
    void userChanged();
protected:
    void tryQuery() override;
    void processResult(const QueryResult result) override;
private:
    Utility::StockProduct m_product;

    void setCategoryId(int categoryId);
    void setCategory(const QString &category);
    void setProduct(const QString &product);
    void setDescription(const QString &description);
    void setDivisible(bool divisible);
    void setImageUrl(const QUrl &imageUrl);
    void setQuantity(double quantity);
    void setUnitId(int unitId);
    void setUnit(const QString &unit);
    void setCostPrice(qreal costPrice);
    void setRetailPrice(qreal retailPrice);
    void setCurrency(const QString &currency);
    void setCreated(const QDateTime &created);
    void setLastEdited(const QDateTime &lastEdited);
    void setUserId(int userId);
    void setUser(const QString & user);
};

#endif // QMLSTOCKPRODUCTDETAILRECORD_H
