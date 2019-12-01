#ifndef BUSINESSSTOREMODEL_H
#define BUSINESSSTOREMODEL_H

#include <QAbstractListModel>

class BusinessStore;

class BusinessStoreModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
public:
    enum Roles {
        BusinessStoreId,
        NameRole,
        AddressRole,
        BusinessFamilyRole,
        EstablishmentYearRole,
        PhoneNumberRole,
        LogoUrlRole
    }; Q_ENUM(Roles)
    explicit BusinessStoreModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    BusinessStore currentBusinessStore() const;

    void setCurrentIndex(int currentIndex);
    int currentIndex() const;
signals:
    void currentIndexChanged();
private:
    int m_currentIndex;
    void reset();
};

#endif // BUSINESSSTOREMODEL_H
