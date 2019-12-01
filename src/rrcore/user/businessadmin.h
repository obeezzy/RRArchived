#ifndef BUSINESSADMIN_H
#define BUSINESSADMIN_H

#include <QObject>
#include "businessstore.h"

class BusinessAdmin : public QObject {
    friend class QMLUserProfile;
    Q_OBJECT
    Q_PROPERTY(QString emailAddress READ emailAddress NOTIFY emailAddressChanged)
    Q_PROPERTY(QList<BusinessStore> businessStores READ businessStores WRITE setBusinessStores NOTIFY businessStoresChanged)
public:
    explicit BusinessAdmin(QObject *parent = nullptr);

    QString emailAddress() const { return m_emailAddress; }
    QString password() const { return m_password; }
    QList<BusinessStore> businessStores() const { return m_businessStores; }
signals:
    void emailAddressChanged();
    void businessStoresChanged();
private:
    QString m_emailAddress;
    QString m_password;
    QList<BusinessStore> m_businessStores;

    void extractFromVariantMap(const QVariantMap &map);
    void setEmailAddress(const QString &emailAddress);
    void setPassword(const QString &password);
    void setBusinessStores(const QList<BusinessStore> &businessStores);
};


#endif // BUSINESSADMIN_H
