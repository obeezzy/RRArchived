#ifndef MONEY_H
#define MONEY_H

#include "currency.h"
#include "singletons/settings.h"
#include "../../3rdparty/BigDecimal-CPP/BigDecimal.h"
#include <QString>
#include <QDebug>
#include <QWeakPointer>

namespace Utility {
class Money
{
public:
    explicit Money();
    explicit Money(const QString &num,
                   const Currency &currency = Currency());
    explicit Money(int num,
                   const Currency &currency = Currency());
    explicit Money(qlonglong num,
                   const Currency &currency = Currency());
    explicit Money(qulonglong num,
                   const Currency &currency = Currency());
    explicit Money(float num,
                   const Currency &currency = Currency());
    explicit Money(double num,
                   const Currency &currency = Currency());
    explicit Money(long double num,
                   const Currency &currency = Currency());
    explicit Money(const Money &other);
    ~Money();

    Money &operator=(const Money &other);
    Money operator+(const Money& other) const;
    Money operator-(const Money& other) const;
    Money operator*(const Money& other) const;
    Money operator/(const Money& other) const;
    Money operator%(const Money& other) const;
    Money operator^(const Money& other) const;
    void operator+=(const Money& other);
    void operator-=(const Money& other);
    void operator*=(const Money& other);
    void operator/=(const Money& other);
    void operator^=(const Money& other);
    bool operator > (const Money& other) const;
    bool operator >= (const Money& other) const;
    bool operator == (const Money& other) const;
    bool operator != (const Money& other) const;
    bool operator < (const Money& other) const;
    bool operator <= (const Money& other) const;

    bool isZero() const;

    int toInt() const;
    unsigned int toUInt() const;
    qlonglong toLongLong() const;
    qulonglong toULongLong() const;
    long double toLongDouble() const;
    double toDouble() const;
    float toFloat() const;
    QString toString() const;
    QString toCurrencyString() const;
    std::string toStdString() const;

    QString intPart() const;
    QString decPart() const;

    Currency currency() const;
    const BigDecimal &value() const;

    friend QDebug operator<<(QDebug debug, const Money &money)
    {
        debug.nospace() << "Money("
                        << money.toCurrencyString()
                        << ")";

        return debug.nospace();
    }
private:
    BigDecimal m_value;
    Currency m_currency;

    static Money fromValue(const BigDecimal &num);
};
}
Q_DECLARE_TYPEINFO(Utility::Money, Q_PRIMITIVE_TYPE);

#endif // MONEY_H
