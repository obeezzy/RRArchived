#include "stockproductquantity.h"
#include "singletons/settings.h"
#include "../../3rdparty/BigDecimal-CPP/BigDecimal.h"
#include <QRegularExpression>

Q_LOGGING_CATEGORY(lcstockproductquantity, "rrcore.utility.stock.stockproductquantity");

using namespace Utility;

StockProductQuantity::StockProductQuantity() :
    m_value("0")
{}

StockProductQuantity::StockProductQuantity(const QString &num) :
    m_value(num.toStdString())
{
    if (decPart() > 4)
        m_value.round(4);
}

StockProductQuantity::StockProductQuantity(int num) :
    m_value(num)
{}

StockProductQuantity::StockProductQuantity(qlonglong num) :
    m_value(num)
{}

StockProductQuantity::StockProductQuantity(qulonglong num) :
    m_value(num)
{}

StockProductQuantity::StockProductQuantity(float num) :
    m_value(num)
{
    if (decPart() > 4)
        m_value.round(4);
}

StockProductQuantity::StockProductQuantity(double num) :
    m_value(num)
{
    if (decPart() > 4)
        m_value.round(4);
}

StockProductQuantity::StockProductQuantity(long double num) :
    m_value(num)
{
    if (decPart() > 4)
        m_value.round(4);
}

StockProductQuantity::StockProductQuantity(const QVariantMap &map) :
    m_value(map.value("quantity").toDouble()),
    m_productId(map.value("product_id").toInt())
{}

StockProductQuantity::StockProductQuantity(const StockProductQuantity &other)
{
    m_value = other.value();
}

StockProductQuantity &StockProductQuantity::operator=(const StockProductQuantity &other)
{
    m_value = other.value();
    return *this;
}

StockProductQuantity::~StockProductQuantity()
{
}

StockProductQuantity StockProductQuantity::operator+(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return StockProductQuantity::fromValue(a + b);
}

StockProductQuantity StockProductQuantity::operator-(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return StockProductQuantity::fromValue(a - b);
}

StockProductQuantity StockProductQuantity::operator*(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return StockProductQuantity::fromValue(a * b);
}

StockProductQuantity StockProductQuantity::operator/(const StockProductQuantity &other) const
{
    if (other.isZero())
        qFatal("Division by zero error!");

    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return StockProductQuantity::fromValue(a / b);
}

StockProductQuantity StockProductQuantity::operator%(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return StockProductQuantity::fromValue(a % b);
}

StockProductQuantity StockProductQuantity::operator^(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return StockProductQuantity::fromValue(a ^ b);
}

void StockProductQuantity::operator+=(const StockProductQuantity &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a += b;
}

void StockProductQuantity::operator-=(const StockProductQuantity &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a -= b;
}

void StockProductQuantity::operator*=(const StockProductQuantity &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a *= b;
}

void StockProductQuantity::operator/=(const StockProductQuantity &other)
{
    if (other.isZero())
        throw std::runtime_error("Division by zero error!");

    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a /= b;
}

void StockProductQuantity::operator^=(const StockProductQuantity &other)
{
    BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    a ^= b;
}

bool StockProductQuantity::operator >(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a > b;
}

bool StockProductQuantity::operator >=(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a >= b;
}

bool StockProductQuantity::operator ==(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a == b;
}

bool StockProductQuantity::operator !=(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return !(a == b);
}

bool StockProductQuantity::operator <(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a < b;
}

bool StockProductQuantity::operator <=(const StockProductQuantity &other) const
{
    const BigDecimal &a = m_value;
    const BigDecimal &b = other.value();
    return a <= b;
}

bool StockProductQuantity::isZero() const
{
    return toString().isEmpty()
            || toString() == QStringLiteral("0")
            || toString() == QStringLiteral("0.00");
}

int StockProductQuantity::productId() const
{
    return m_productId;
}

void StockProductQuantity::setProductId(int productId)
{
    m_productId = productId;
}

int StockProductQuantity::toInt(const StockProductUnit &unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace() << "Product ID mismatch: StockProductQuantity("
                                             << m_productId
                                             << "), StockProductUnit("
                                             << unit.productId
                                             << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0;
    }

    return m_value.toInt() * unit.baseUnitEquivalent;
}

unsigned int StockProductQuantity::toUInt(const StockProductUnit &unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace() << "Product ID mismatch: StockProductQuantity("
                                             << m_productId
                                             << "), StockProductUnit("
                                             << unit.productId
                                             << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0;
    }

    return m_value.toUInt() * unit.baseUnitEquivalent;
}

qlonglong StockProductQuantity::toLongLong(const StockProductUnit &unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace() << "Product ID mismatch: StockProductQuantity("
                                             << m_productId
                                             << "), StockProductUnit("
                                             << unit.productId
                                             << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0LL;
    }

    return m_value.toLongLong() * unit.baseUnitEquivalent;
}

qulonglong StockProductQuantity::toULongLong(const StockProductUnit &unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace() << "Product ID mismatch: StockProductQuantity("
                                             << m_productId
                                             << "), StockProductUnit("
                                             << unit.productId
                                             << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0ULL;
    }

    return m_value.toULongLong() * unit.baseUnitEquivalent;
}

long double StockProductQuantity::toLongDouble(const StockProductUnit &unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace() << "Product ID mismatch: StockProductQuantity("
                                             << m_productId
                                             << "), StockProductUnit("
                                             << unit.productId
                                             << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0.0L;
    }

    return m_value.toLongDouble() * unit.baseUnitEquivalent;
}

double StockProductQuantity::toDouble(const StockProductUnit &unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace() << "Product ID mismatch: StockProductQuantity("
                                             << m_productId
                                             << "), StockProductUnit("
                                             << unit.productId
                                             << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0.0;
    }

    return m_value.toDouble() * unit.baseUnitEquivalent;
}

float StockProductQuantity::toFloat(const StockProductUnit &unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace() << "Product ID mismatch: StockProductQuantity("
                                             << m_productId
                                             << "), StockProductUnit("
                                             << unit.productId
                                             << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0.0f;
    }

    return m_value.toFloat() * unit.baseUnitEquivalent;
}

QString StockProductQuantity::toString() const
{
    QString result = QString::fromStdString(toStdString()).remove(QRegularExpression("^(\\d+\\.\\d*?[1-9])0+$"));
    if (result.endsWith(".0000"))
        result.remove(".0000");

    return result;
}

std::string StockProductQuantity::toStdString() const
{
    return m_value.toString();
}

QString StockProductQuantity::intPart() const
{
    return QString::fromStdString(m_value.getIntPart());
}

QString StockProductQuantity::decPart() const
{
    return QString::fromStdString(m_value.getDecPart());
}

const BigDecimal &StockProductQuantity::value() const
{
    return m_value;
}

StockProductQuantity StockProductQuantity::fromValue(const BigDecimal &num)
{
    return StockProductQuantity{ QString::fromStdString(num.toString()) };
}
