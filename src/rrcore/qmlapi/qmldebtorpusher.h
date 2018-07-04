#ifndef QMLDEBTORPUSHER_H
#define QMLDEBTORPUSHER_H

#include <QObject>
#include "pusher/abstractpusher.h"
#include <QDateTime>

struct DebtPayment {
    double amount = 0.0;
    QString note;
}; Q_DECLARE_TYPEINFO(DebtPayment, Q_PRIMITIVE_TYPE);

struct DebtTransaction {
    double totalDebt = 0.0;
    QDateTime dueDateTime;
    QString note;
    QList<DebtPayment *> debtPayments;
}; Q_DECLARE_TYPEINFO(DebtTransaction, Q_PRIMITIVE_TYPE);

class QMLDebtorPusher : public AbstractPusher
{
    Q_OBJECT
    Q_PROPERTY(QString imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString preferredName READ preferredName WRITE setPreferredName NOTIFY preferredNameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)
public:
    enum SuccessCode {
        UnknownSuccess,
        DebtorAdded
    }; Q_ENUM(SuccessCode)

    enum ErrorCode {
        Unknown,
        NoPreferredNameError,
        NoPhoneNumberError,
        NoDebtError,
        DuplicateEntryError,
        AmountOverpaidError,
        InvalidDueDateError
    }; Q_ENUM(ErrorCode)

    explicit QMLDebtorPusher(QObject *parent = nullptr);

    QString imageSource() const;
    void setImageSource(const QString &imageSource);

    QString firstName() const;
    void setFirstName(const QString &firstName);

    QString lastName() const;
    void setLastName(const QString &lastName);

    QString preferredName() const;
    void setPreferredName(const QString &preferredName);

    QString phoneNumber() const;
    void setPhoneNumber(const QString phoneNumber);

    QString address() const;
    void setAddress(const QString &address);

    QString note() const;
    void setNote(const QString &note);
signals:
    void imageSourceChanged();
    void firstNameChanged();
    void lastNameChanged();
    void preferredNameChanged();
    void phoneNumberChanged();
    void addressChanged();
    void noteChanged();
protected:
    void processResult(const QueryResult result) override final;
public slots:
    void push() override final;

    void addDebt(double totalDebt, const QDateTime &dueDateTime, const QString &note = QString());
    void updateDebt(int debtIndex, double totalDebt, const QDateTime &dueDateTime, const QString &note = QString());
    void removeDebt(int debtIndex);

    void addPayment(int debtIndex, double amount, const QString &note = QString());
    void updatePayment(int debtIndex, int paymentIndex, double amount, const QString &note = QString());
    void removePayment(int debtIndex, int paymentIndex);
private:
    QList<DebtTransaction *> m_debtTransactions;

    QString m_imageSource;
    QString m_firstName;
    QString m_lastName;
    QString m_preferredName;
    QString m_phoneNumber;
    QString m_address;
    QString m_note;

    QVariant convertToVariant(const QList<DebtTransaction *> &debtTransactions);
};

#endif // QMLDEBTORPUSHER_H
