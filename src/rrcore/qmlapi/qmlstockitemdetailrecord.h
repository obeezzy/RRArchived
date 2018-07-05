#ifndef QMLSTOCKITEMDETAILRECORD_H
#define QMLSTOCKITEMDETAILRECORD_H

#include "detailrecord/abstractdetailrecord.h"
#include <QVariantMap>
#include <QDateTime>

class QMLStockItemDetailRecord : public AbstractDetailRecord
{
    Q_OBJECT
    Q_PROPERTY(int itemId READ itemId WRITE setItemId NOTIFY itemIdChanged)
    Q_PROPERTY(int categoryId READ categoryId NOTIFY categoryIdChanged)
    Q_PROPERTY(QString category READ category NOTIFY categoryChanged)
    Q_PROPERTY(QString item READ item NOTIFY itemChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(bool divisible READ isDivisible NOTIFY divisibleChanged)
    Q_PROPERTY(QString imageSource READ imageSource NOTIFY imageSourceChanged)
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
    explicit QMLStockItemDetailRecord(QObject *parent = nullptr);

    int itemId() const;
    void setItemId(int itemId);

    int categoryId() const;
    QString category() const;

    QString item() const;

    QString description() const;

    bool isDivisible() const;

    QString imageSource() const;

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
    void itemIdChanged();
    void categoryIdChanged();
    void categoryChanged();
    void itemChanged();
    void descriptionChanged();
    void divisibleChanged();
    void imageSourceChanged();
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
    void reset() override;
private:
    int m_itemId;
    QVariantMap m_record;
};

#endif // QMLSTOCKITEMDETAILRECORD_H
