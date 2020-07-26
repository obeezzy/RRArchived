#include "qmlproductdetailmodel.h"
#include "database/databasethread.h"
#include "query/stock/fetchproduct.h"
#include <QDateTime>

QMLProductDetailModel::QMLProductDetailModel(QObject *parent) :
    QMLProductDetailModel(DatabaseThread::instance(), parent)
{}

QMLProductDetailModel::QMLProductDetailModel(DatabaseThread &thread, QObject *parent) :
    AbstractVisualListModel(thread, parent)
{
    connect(this, &QMLProductDetailModel::productIdChanged,
            this, &QMLProductDetailModel::tryQuery);
}

int QMLProductDetailModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_productDetails.count();
}

QVariant QMLProductDetailModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TitleRole:
        return m_productDetails.at(index.row()).toMap().value("title");
    case DetailRole:
        return m_productDetails.at(index.row()).toMap().value("detail");
    case DatatypeRole:
        return m_productDetails.at(index.row()).toMap().value("datatype");
    }

    return QVariant();
}

QHash<int, QByteArray> QMLProductDetailModel::roleNames() const
{
    return {
        { TitleRole, "title" },
        { DetailRole, "detail" },
        { DatatypeRole, "datatype" }
    };
}

int QMLProductDetailModel::productId() const
{
    return m_product.id;
}

void QMLProductDetailModel::setProductId(int productId)
{
    if (m_product.id == productId)
        return;

    m_product.id = productId;
    emit productIdChanged();
}

int QMLProductDetailModel::categoryId() const
{
    return m_product.category.id;
}

QString QMLProductDetailModel::category() const
{
    return m_product.category.category;
}

QString QMLProductDetailModel::product() const
{
    return m_product.product;
}

QString QMLProductDetailModel::description() const
{
    return m_product.description;
}

bool QMLProductDetailModel::isDivisible() const
{
    return m_product.flags.testFlag(Utility::RecordGroup::Divisible);
}

QUrl QMLProductDetailModel::imageUrl() const
{
    return m_product.imageUrl;
}

double QMLProductDetailModel::quantity() const
{
    return m_product.quantity.toDouble();
}

int QMLProductDetailModel::unitId() const
{
    return m_product.unit.id;
}

QString QMLProductDetailModel::unit() const
{
    return m_product.unit.unit;
}

double QMLProductDetailModel::costPrice() const
{
    return m_product.monies.costPrice.toDouble();
}

double QMLProductDetailModel::retailPrice() const
{
    return m_product.monies.retailPrice.toDouble();
}

QString QMLProductDetailModel::currency() const
{
    return m_product.monies.retailPrice.currency().isoCode();
}

QDateTime QMLProductDetailModel::created() const
{
    return m_product.timestamp.created;
}

QDateTime QMLProductDetailModel::lastEdited() const
{
    return m_product.timestamp.lastEdited;
}

int QMLProductDetailModel::userId() const
{
    return m_product.user.id;
}

QString QMLProductDetailModel::user() const
{
    return m_product.user.user;
}

void QMLProductDetailModel::tryQuery()
{
    if (m_product.id <= 0)
        return;

    setBusy(true);
    emit execute(new Query::Stock::FetchProduct(m_product.id,
                                                   this));
}

bool QMLProductDetailModel::canProcessResult(const QueryResult &result) const
{
    Q_UNUSED(result)
    return true;
}

void QMLProductDetailModel::processResult(const QueryResult &result)
{
    setBusy(false);

    if (result.isSuccessful()) {
        beginResetModel();
        const auto product = Utility::Stock::Product{ result.outcome().toMap().value("product").toMap() };
        m_productDetails.clear();
        m_productDetails.append(QVariantMap{
                { "title", tr("Category") },
                { "detail", product.category.category },
                { "datatype", "string" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("Product") },
                { "detail", product.product },
                { "datatype", "string" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("Description") },
                { "detail", product.description },
                { "datatype", "string" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("Quantity") },
                { "detail", product.quantity.toString().append(" ").append(product.unit.unit) },
                { "datatype", "string" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("Cost price") },
                { "detail", product.monies.costPrice.toDouble() },
                { "datatype", "money" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("Retail price") },
                { "detail", product.monies.retailPrice.toDouble() },
                { "datatype", "money" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("Created") },
                { "detail", product.timestamp.created },
                { "datatype", "datetime" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("Last edited") },
                { "detail", product.timestamp.lastEdited },
                { "datatype", "datetime" }
        });
        m_productDetails.append(QVariantMap{
                { "title", tr("User") },
                { "detail", product.user.user },
                { "datatype", "string" }
        });

        setCategoryId(product.category.id);
        setCategory(product.category.category);
        setProduct(product.product);
        setDescription(product.description);
        setDivisible(product.flags.testFlag(Utility::RecordGroup::Divisible));
        setImageUrl(product.imageUrl);
        setQuantity(product.quantity.toDouble());
        setUnitId(product.id);
        setUnit(product.unit.unit);
        setCostPrice(product.monies.costPrice.toDouble());
        setRetailPrice(product.monies.retailPrice.toDouble());
        setCreated(product.timestamp.created);
        setLastEdited(product.timestamp.lastEdited);
        setUserId(product.user.id);
        setUser(product.user.user);
        endResetModel();
        emit success();
    } else {
        emit error();
    }
} 

void QMLProductDetailModel::setCategoryId(int categoryId)
{
    if (m_product.category.id == categoryId)
        return;

    m_product.category.id = categoryId;
    emit categoryIdChanged();
}

void QMLProductDetailModel::setCategory(const QString &category)
{
    if (m_product.category.category == category)
        return;

    m_product.category.category = category;
    emit categoryChanged();
}

void QMLProductDetailModel::setProduct(const QString &product)
{
    if (m_product.product == product)
        return;

    m_product.product = product;
    emit productChanged();
}

void QMLProductDetailModel::setDescription(const QString &description)
{
    if (m_product.description == description)
        return;

    m_product.description = description;
    emit descriptionChanged();
}

void QMLProductDetailModel::setDivisible(bool divisible)
{
    if (m_product.flags.testFlag(Utility::RecordGroup::Divisible) == divisible)
        return;

    m_product.flags.setFlag(Utility::RecordGroup::Divisible, divisible);
    emit divisibleChanged();
}

void QMLProductDetailModel::setImageUrl(const QUrl &imageUrl)
{
    if (m_product.imageUrl == imageUrl)
        return;

    m_product.imageUrl = imageUrl;
    emit imageUrlChanged();
}

void QMLProductDetailModel::setQuantity(double quantity)
{
    if (m_product.quantity == Utility::Stock::ProductQuantity(quantity))
        return;

    m_product.quantity = Utility::Stock::ProductQuantity(quantity);
    emit quantityChanged();
}

void QMLProductDetailModel::setUnitId(int unitId)
{
    if (m_product.unit.id == unitId)
        return;

    m_product.unit.id = unitId;
    emit unitIdChanged();
}

void QMLProductDetailModel::setUnit(const QString &unit)
{
    if (m_product.unit.unit == unit)
        return;

    m_product.unit.unit = unit;
    emit unitChanged();
}

void QMLProductDetailModel::setCostPrice(double costPrice)
{
    if (m_product.monies.costPrice == Utility::Money(costPrice))
        return;

    m_product.monies.costPrice = Utility::Money(costPrice);
    emit costPriceChanged();
}

void QMLProductDetailModel::setRetailPrice(double retailPrice)
{
    if (m_product.monies.retailPrice == Utility::Money(retailPrice))
        return;

    m_product.monies.retailPrice = Utility::Money(retailPrice);
    emit retailPriceChanged();
}

void QMLProductDetailModel::setCreated(const QDateTime &created)
{
    if (m_product.timestamp.created == created)
        return;

    m_product.timestamp.created = created;
    emit createdChanged();
}

void QMLProductDetailModel::setLastEdited(const QDateTime &lastEdited)
{
    if (m_product.timestamp.lastEdited == lastEdited)
        return;

    m_product.timestamp.lastEdited = lastEdited;
    emit lastEditedChanged();
}

void QMLProductDetailModel::setUserId(int userId)
{
    if (m_product.user.id == userId)
        return;

    m_product.user.id = userId;
    emit userIdChanged();
}

void QMLProductDetailModel::setUser(const QString &user)
{
    if (m_product.user.user == user)
        return;

    m_product.user.user = user;
    emit userChanged();
}
