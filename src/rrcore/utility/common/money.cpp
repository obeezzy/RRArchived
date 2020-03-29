#include "money.h"
#include "../../3rdparty/BigDecimal-CPP/BigDecimal.h"

using namespace Utility;

Money::Money() :
    m_value("0.00")
{}

Money::Money(const QString &num,
               const Currency &currency) :
    m_value(num.toStdString()),
    m_currency(currency)
{
    m_value.round(2);
}

Money::Money(int num,
             const Currency &currency) :
    m_value(num),
    m_currency(currency)
{}

Money::Money(qlonglong num,
             const Currency &currency) :
    m_value(num),
    m_currency(currency)
{}

Money::Money(qulonglong num,
             const Currency &currency) :
    m_value(num),
    m_currency(currency)
{}

Money::Money(float num,
             const Currency &currency) :
    m_value(num),
    m_currency(currency)
{
    m_value.round(2);
}

Money::Money(double num,
             const Currency &currency) :
    m_value(num),
    m_currency(currency)
{
    m_value.round(2);
}

Money::Money(long double num,
             const Currency &currency) :
    m_value(num),
    m_currency(currency)
{
    m_value.round(2);
}

Money::Money(const Money &other)
{
    m_value = other.value();
}

Money &Money::operator=(const Money &other)
{
    m_value = other.value();
    return *this;
}

Money Money::operator+(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return Money::fromValue(a + b);
}

Money Money::operator-(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return Money::fromValue(a - b);
}

Money Money::operator*(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return Money::fromValue(a * b);
}

Money Money::operator/(const Money &other) const
{
    if (other.isZero())
        qFatal("Division by zero error!");

    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return Money::fromValue(a / b);
}

Money Money::operator%(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return Money::fromValue(a % b);
}

Money Money::operator^(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return Money::fromValue(a ^ b);
}

void Money::operator+=(const Money &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a += b;
}

void Money::operator-=(const Money &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a -= b;
}

void Money::operator*=(const Money &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a *= b;
}

void Money::operator/=(const Money &other)
{
    if (other.isZero())
        throw std::runtime_error("Division by zero error!");

    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a /= b;
}

void Money::operator^=(const Money &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a ^= b;
}

bool Money::operator >(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a > b;
}

bool Money::operator >=(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a >= b;
}

bool Money::operator ==(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a == b;
}

bool Money::operator !=(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return !(a == b);
}

bool Money::operator <(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a < b;
}

bool Money::operator <=(const Money &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a <= b;
}

bool Money::isZero() const
{
    return toString().isEmpty()
            || toString() == QStringLiteral("0")
            || toString() == QStringLiteral("0.00");
}

int Money::toInt() const
{
    return m_value.toInt();
}

unsigned int Money::toUInt() const
{
    return m_value.toUInt();
}

qlonglong Money::toLongLong() const
{
    return m_value.toLongLong();
}

qulonglong Money::toULongLong() const
{
    return m_value.toULongLong();
}

long double Money::toLongDouble() const
{
    return m_value.toLongDouble();
}

double Money::toDouble() const
{
    return m_value.toDouble();
}

float Money::toFloat() const
{
    return m_value.toFloat();
}

QString Money::toString() const
{
    return QString::fromStdString(toStdString());
}

QString Money::toCurrencyString() const
{
    return QStringLiteral("%1%2").arg(m_currency.symbol(),
                                      QLocale().toString(toString().toDouble(), 'f', 2));
}

std::string Money::toStdString() const
{
    return m_value.toString();
}

Currency Money::currency() const
{
    return Currency();
}

const BigDecimal &Money::value() const
{
    return m_value;
}

Money Money::fromValue(const BigDecimal &num)
{
    return Money{ QString::fromStdString(num.toString()) };
}
