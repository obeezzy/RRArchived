#ifndef QMLPRODUCTDETAILRECORD_H
#define QMLPRODUCTDETAILRECORD_H

#include <QVariantList>
#include "models/abstractvisuallistmodel.h"
#include "utility/stock/product.h"

class QMLProductDetailModel : public AbstractVisualListModel
{
    Q_OBJECT
    Q_PROPERTY(
        int productId READ productId WRITE setProductId NOTIFY productIdChanged)
    Q_PROPERTY(int categoryId READ categoryId NOTIFY categoryIdChanged)
    Q_PROPERTY(QString category READ category NOTIFY categoryChanged)
    Q_PROPERTY(QString product READ product NOTIFY productChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool divisible READ isDivisible NOTIFY divisibleChanged)
    Q_PROPERTY(QUrl imageUrl READ imageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(double quantity READ quantity NOTIFY quantityChanged)
    Q_PROPERTY(int unitId READ unitId NOTIFY unitIdChanged)
    Q_PROPERTY(QString unit READ unit NOTIFY unitChanged)
    Q_PROPERTY(double costPrice READ costPrice NOTIFY costPriceChanged)
    Q_PROPERTY(double retailPrice READ retailPrice NOTIFY retailPriceChanged)
    Q_PROPERTY(QString currency READ currency NOTIFY currencyChanged)
    Q_PROPERTY(QDateTime created READ created NOTIFY createdChanged)
    Q_PROPERTY(QDateTime lastEdited READ lastEdited NOTIFY lastEditedChanged)
    Q_PROPERTY(int userId READ userId NOTIFY userIdChanged)
    Q_PROPERTY(QString user READ user NOTIFY userChanged)
public:
    enum Role
    {
        TitleRole = Qt::UserRole,
        DetailRole,
        DatatypeRole
    };
    explicit QMLProductDetailModel(QObject* parent = nullptr);
    explicit QMLProductDetailModel(DatabaseThread& thread,
                                   QObject* parent = nullptr);

    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;

    int productId() const;
    void setProductId(int productId);

    int categoryId() const;
    QString category() const;

    QString product() const;
    QString description() const;
    bool isDivisible() const;
    QUrl imageUrl() const;
    double quantity() const;
    int unitId() const;
    QString unit() const;
    double costPrice() const;
    double retailPrice() const;
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
    bool canProcessResult(const QueryResult& result) const override;
    void processResult(const QueryResult& result) override;

private:
    Utility::Stock::Product m_product;
    QVariantList m_productDetails;

    void setCategoryId(int categoryId);
    void setCategory(const QString& category);
    void setProduct(const QString& product);
    void setDescription(const QString& description);
    void setDivisible(bool divisible);
    void setImageUrl(const QUrl& imageUrl);
    void setQuantity(double quantity);
    void setUnitId(int unitId);
    void setUnit(const QString& unit);
    void setCostPrice(double costPrice);
    void setRetailPrice(double retailPrice);
    void setCreated(const QDateTime& created);
    void setLastEdited(const QDateTime& lastEdited);
    void setUserId(int userId);
    void setUser(const QString& user);
};

#endif  // QMLPRODUCTDETAILRECORD_H
