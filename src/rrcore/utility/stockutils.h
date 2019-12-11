#ifndef STOCKUTILS_H
#define STOCKUTILS_H

#include <QList>
#include <QVariantList>
#include <QVariantMap>
#include <QDateTime>

struct StockItem {
    int categoryId;
    QString category;
    int itemId;
    QString item;
    QString description;
    bool divisible;
    QUrl imageUrl;
    double quantity;
    int unitId;
    QString unit;
    qreal costPrice;
    qreal retailPrice;
    qreal unitPrice;
    qreal cost;
    qreal amountPaid;
    QString note;
    QString currency;
    QDateTime created;
    QDateTime lastEdited;
    QString user;

    explicit StockItem(int categoryId,
                       int itemId,
                       double quantity,
                       int unitId,
                       qreal retailPrice,
                       qreal unitPrice,
                       qreal cost,
                       qreal amountPaid,
                       const QString &note) :
        categoryId(categoryId),
        itemId(itemId),
        quantity(quantity),
        unitId(unitId),
        retailPrice(retailPrice),
        unitPrice(unitPrice),
        cost(cost),
        amountPaid(amountPaid),
        note(note)
    { }

    explicit StockItem(const QVariantMap &item) :
        categoryId(item.value("category_id").toInt()),
        category(item.value("category").toString()),
        itemId(item.value("item_id").toInt()),
        item(item.value("item").toString()),
        description(item.value("description").toString()),
        divisible(item.value("divisible").toBool()),
        imageUrl(item.value("image_url").toUrl()),
        quantity(item.value("quantity").toDouble()),
        unitId(item.value("unit_id").toInt()),
        unit(item.value("unit").toString()),
        costPrice(item.value("cost_price").toDouble()),
        retailPrice(item.value("retail_price").toDouble()),
        unitPrice(item.value("unit_price").toDouble()),
        cost(item.value("cost").toDouble()),
        amountPaid(item.value("amount_paid").toDouble()),
        note(item.value("note").toString()),
        currency(item.value("currency").toString()),
        created(item.value("created").toDateTime()),
        lastEdited(item.value("last_edited").toDateTime()),
        user(item.value("user").toString())
    {}

    inline QVariantMap toVariantMap() const {
        return {
            { "category_id", categoryId },
            { "item_id", itemId },
            { "quantity", quantity },
            { "unit_id", unitId },
            { "retail_price", retailPrice },
            { "unit_price", unitPrice },
            { "cost", cost },
            { "amount_paid", amountPaid },
            { "note", note }
        };
    }
};

class StockItemList : public QList<StockItem> {
public:
    explicit StockItemList() {}
    explicit StockItemList(std::initializer_list<StockItem> paymentList) :
        QList<StockItem>(paymentList) {}

    QVariantList toVariantList() const {
        QVariantList list;
        for (const auto &stockItem : *this)
            list.append(stockItem.toVariantMap());
        return list;
    }
};

#endif // STOCKUTILS_H
