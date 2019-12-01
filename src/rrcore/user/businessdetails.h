#ifndef BUSINESSDETAILS_H
#define BUSINESSDETAILS_H

#include <QObject>
#include <QUrl>

class BusinessStore;

class BusinessDetails : public QObject {
    friend class QMLUserProfile;
    Q_OBJECT
    Q_PROPERTY(QUrl logoUrl READ logoUrl WRITE setLogoUrl NOTIFY logoUrlChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(int establishmentYear READ establishmentYear WRITE setEstablishmentYear NOTIFY establishmentYearChanged)
public:
    explicit BusinessDetails(QObject *parent = nullptr);

    QUrl logoUrl() const;
    void setLogoUrl(const QUrl &logoUrl);

    QString name() const;
    void setName(const QString &name);

    QString address() const;
    void setAddress(const QString &address);

    QString businessFamily() const;
    void setBusinessFamily(const QString &businessFamily);

    QString phoneNumber() const;
    void setPhoneNumber(const QString &phoneNumber);

    int establishmentYear() const;
    void setEstablishmentYear(int establishmentYear);
signals:
    void logoUrlChanged();
    void nameChanged();
    void addressChanged();
    void businessFamilyChanged();
    void phoneNumberChanged();
    void establishmentYearChanged();
private:
    QUrl m_logoUrl;
    QString m_name;
    QString m_address;
    QString m_businessFamily;
    QString m_phoneNumber;
    int m_establishmentYear;

    void extractFromBusinessStore(const BusinessStore &businessStore);
};

#endif // BUSINESSDETAILS_H
