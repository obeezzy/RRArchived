#ifndef QMLPRODUCTPUSHER_H
#define QMLPRODUCTPUSHER_H

#include "pusher/abstractpusher.h"
#include "utility/stock/product.h"

class DatabaseThread;
class QUrl;

class QMLProductPusher : public AbstractPusher
{
    Q_OBJECT
    Q_PROPERTY(
        int productId READ productId WRITE setProductId NOTIFY productIdChanged)
    Q_PROPERTY(
        QUrl imageUrl READ imageUrl WRITE setImageUrl NOTIFY imageUrlChanged)
    Q_PROPERTY(
        QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(
        QString product READ product WRITE setProduct NOTIFY productChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY
                   descriptionChanged)
    Q_PROPERTY(
        double quantity READ quantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(QString categoryNote READ categoryNote WRITE setCategoryNote
                   NOTIFY categoryNoteChanged)
    Q_PROPERTY(QString productNote READ productNote WRITE setProductNote NOTIFY
                   productNoteChanged)
    Q_PROPERTY(
        bool tracked READ isTracked WRITE setTracked NOTIFY trackedChanged)
    Q_PROPERTY(bool divisible READ isDivisible WRITE setDivisible NOTIFY
                   divisibleChanged)
    Q_PROPERTY(double costPrice READ costPrice WRITE setCostPrice NOTIFY
                   costPriceChanged)
    Q_PROPERTY(double retailPrice READ retailPrice WRITE setRetailPrice NOTIFY
                   retailPriceChanged)
public:
    explicit QMLProductPusher(QObject* parent = nullptr);
    explicit QMLProductPusher(DatabaseThread& thread,
                              QObject* parent = nullptr);  // For testing

    enum SuccessCode
    {
        UnknownSuccess,
        AddProductSuccess,
        UpdateProductSuccess
    };
    Q_ENUM(SuccessCode)

    enum ErrorCode
    {
        UnknownError,
        DuplicateEntryError,
        ImageTooLargeError
    };
    Q_ENUM(ErrorCode)

    bool isExistingProduct() const;

    int productId() const;
    void setProductId(int productId);

    QUrl imageUrl() const;
    void setImageUrl(const QUrl& imageUrl);

    QString category() const;
    void setCategory(const QString&);

    QString product() const;
    void setProduct(const QString&);

    QString description() const;
    void setDescription(const QString&);

    double quantity() const;
    void setQuantity(double);

    QString unit() const;
    void setUnit(const QString&);

    QString categoryNote() const;
    void setCategoryNote(const QString&);

    QString productNote() const;
    void setProductNote(const QString&);

    bool isTracked() const;
    void setTracked(bool);

    bool isDivisible() const;
    void setDivisible(bool);

    double costPrice() const;
    void setCostPrice(double);

    double retailPrice() const;
    void setRetailPrice(double);

protected:
    bool canProcessResult(const QueryResult& result) override;
    void processResult(const QueryResult result) override final;
signals:
    void productIdChanged();
    void imageUrlChanged();
    void categoryChanged();
    void productChanged();
    void descriptionChanged();
    void quantityChanged();
    void unitChanged();
    void noteChanged();
    void trackedChanged();
    void divisibleChanged();
    void categoryNoteChanged();
    void productNoteChanged();
    void costPriceChanged();
    void retailPriceChanged();
public slots:
    void push() override final;

private:
    Utility::Stock::Product m_product;
};

#endif  // QMLPRODUCTPUSHER_H
