#ifndef QMLSTOCKPRODUCTDETAILRECORD_H
#define QMLSTOCKPRODUCTDETAILRECORD_H

#include "detailrecord/abstractdetailrecord.h"
#include <QVariantMap>
#include <QDateTime>
#include <QUrl>

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
    int m_productId;

    int m_categoryId;
    QString m_category;
    QString m_product;
    QString m_description;
    bool m_divisible;
    QUrl m_imageUrl;
    double m_quantity;
    int m_unitId;
    QString m_unit;
    qreal m_costPrice;
    qreal m_retailPrice;
    QString m_currency;
    QDateTime m_created;
    QDateTime m_lastEdited;
    int m_userId;
    QString m_user;

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
