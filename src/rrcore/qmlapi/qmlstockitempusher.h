#ifndef QMLSTOCKITEMPUSHER_H
#define QMLSTOCKITEMPUSHER_H

#include <QObject>

#include "../pusher/abstractpusher.h"

class DatabaseThread;

class QMLStockItemPusher : public AbstractPusher
{
    Q_OBJECT
    Q_PROPERTY(int itemId READ itemId WRITE setItemId NOTIFY itemIdChanged)
    Q_PROPERTY(QString imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged)
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(QString item READ item WRITE setItem NOTIFY itemChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(double quantity READ quantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(QString categoryNote READ categoryNote WRITE setCategoryNote NOTIFY categoryNoteChanged)
    Q_PROPERTY(QString itemNote READ itemNote WRITE setItemNote NOTIFY itemNoteChanged)
    Q_PROPERTY(bool tracked READ isTracked WRITE setTracked NOTIFY trackedChanged)
    Q_PROPERTY(bool divisible READ isDivisible WRITE setDivisible NOTIFY divisibleChanged)
    Q_PROPERTY(double costPrice READ costPrice WRITE setCostPrice NOTIFY costPriceChanged)
    Q_PROPERTY(double retailPrice READ retailPrice WRITE setRetailPrice NOTIFY retailPriceChanged)
public:
    explicit QMLStockItemPusher(QObject *parent = nullptr);
    explicit QMLStockItemPusher(DatabaseThread &thread, QObject *parent = nullptr); // For testing

    enum SuccessCode {
        UnknownSuccess,
        AddItemSuccess,
        UpdateItemSuccess
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        UnknownError,
        DuplicateEntryError,
        ImageTooLargeError
    }; Q_ENUM(ErrorCode)

    bool isExistingItem() const;

    int itemId() const;
    void setItemId(int itemId);

    QString imageSource() const;
    void setImageSource(const QString &);

    QString category() const;
    void setCategory(const QString &);

    QString item() const;
    void setItem(const QString &);

    QString description() const;
    void setDescription(const QString &);

    double quantity() const;
    void setQuantity(double);

    QString unit() const;
    void setUnit(const QString &);

    QString categoryNote() const;
    void setCategoryNote(const QString &);

    QString itemNote() const;
    void setItemNote(const QString &);

    bool isTracked() const;
    void setTracked(bool);

    bool isDivisible() const;
    void setDivisible(bool);

    double costPrice() const;
    void setCostPrice(double);

    double retailPrice() const;
    void setRetailPrice(double);
protected:
    void processResult(const QueryResult result) override final;
signals:
    void itemIdChanged();
    void imageSourceChanged();
    void categoryChanged();
    void itemChanged();
    void descriptionChanged();
    void quantityChanged();
    void unitChanged();
    void noteChanged();
    void trackedChanged();
    void divisibleChanged();
    void categoryNoteChanged();
    void itemNoteChanged();
    void costPriceChanged();
    void retailPriceChanged();
public slots:
    void push() override final;
private:
    int m_itemId;
    QString m_imageSource;
    QString m_category;
    QString m_item;
    QString m_description;
    double m_quantity;
    QString m_unit;
    QString m_categoryNote;
    QString m_itemNote;
    bool m_tracked;
    bool m_divisible;
    double m_costPrice;
    double m_retailPrice;
    double m_baseUnitEquivalent;
};

#endif // QMLSTOCKITEMPUSHER_H
