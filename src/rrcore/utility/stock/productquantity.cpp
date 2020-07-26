#include "productquantity.h"
#include <QRegularExpression>
#include "../../3rdparty/BigDecimal-CPP/BigDecimal.h"
#include "singletons/settings.h"

Q_LOGGING_CATEGORY(lcstockproductquantity,
                   "rrcore.utility.stock.stockproductquantity");

using namespace Utility::Stock;

ProductQuantity::ProductQuantity() : m_value("0") {}

ProductQuantity::ProductQuantity(const QString& num)
    : m_value(num.toStdString())
{
    if (decPart() > 4)
        m_value.round(4);
}

ProductQuantity::ProductQuantity(int num) : m_value(num) {}

ProductQuantity::ProductQuantity(qlonglong num) : m_value(num) {}

ProductQuantity::ProductQuantity(qulonglong num) : m_value(num) {}

ProductQuantity::ProductQuantity(float num) : m_value(num)
{
    if (decPart() > 4)
        m_value.round(4);
}

ProductQuantity::ProductQuantity(double num) : m_value(num)
{
    if (decPart() > 4)
        m_value.round(4);
}

ProductQuantity::ProductQuantity(long double num) : m_value(num)
{
    if (decPart() > 4)
        m_value.round(4);
}

ProductQuantity::ProductQuantity(const QVariantMap& map)
    : m_value(map.value("quantity").toDouble()),
      m_productId(map.value("product_id").toInt())
{}

ProductQuantity::ProductQuantity(const ProductQuantity& other)
{
    m_value = other.value();
}

ProductQuantity& ProductQuantity::operator=(const ProductQuantity& other)
{
    m_value = other.value();
    return *this;
}

ProductQuantity::~ProductQuantity() {}

ProductQuantity ProductQuantity::operator+(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return ProductQuantity::fromValue(a + b);
}

ProductQuantity ProductQuantity::operator-(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return ProductQuantity::fromValue(a - b);
}

ProductQuantity ProductQuantity::operator*(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return ProductQuantity::fromValue(a * b);
}

ProductQuantity ProductQuantity::operator/(const ProductQuantity& other) const
{
    if (other.isZero())
        qFatal("Division by zero error!");

    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return ProductQuantity::fromValue(a / b);
}

ProductQuantity ProductQuantity::operator%(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return ProductQuantity::fromValue(a % b);
}

ProductQuantity ProductQuantity::operator^(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return ProductQuantity::fromValue(a ^ b);
}

void ProductQuantity::operator+=(const ProductQuantity& other)
{
    BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    a += b;
}

void ProductQuantity::operator-=(const ProductQuantity& other)
{
    BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    a -= b;
}

void ProductQuantity::operator*=(const ProductQuantity& other)
{
    BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    a *= b;
}

void ProductQuantity::operator/=(const ProductQuantity& other)
{
    if (other.isZero())
        throw std::runtime_error("Division by zero error!");

    BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    a /= b;
}

void ProductQuantity::operator^=(const ProductQuantity& other)
{
    BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    a ^= b;
}

bool ProductQuantity::operator>(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return a > b;
}

bool ProductQuantity::operator>=(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return a >= b;
}

bool ProductQuantity::operator==(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return a == b;
}

bool ProductQuantity::operator!=(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return !(a == b);
}

bool ProductQuantity::operator<(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return a < b;
}

bool ProductQuantity::operator<=(const ProductQuantity& other) const
{
    const BigDecimal& a = m_value;
    const BigDecimal& b = other.value();
    return a <= b;
}

bool ProductQuantity::isZero() const
{
    return toString().isEmpty() || toString() == QStringLiteral("0") ||
           toString() == QStringLiteral("0.00");
}

int ProductQuantity::productId() const
{
    return m_productId;
}

void ProductQuantity::setProductId(int productId)
{
    m_productId = productId;
}

int ProductQuantity::toInt(const ProductUnit& unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace()
            << "Product ID mismatch: ProductQuantity(" << m_productId
            << "), ProductUnit(" << unit.productId << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0;
    }

    return m_value.toInt() * unit.baseUnitEquivalent;
}

unsigned int ProductQuantity::toUInt(const ProductUnit& unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace()
            << "Product ID mismatch: ProductQuantity(" << m_productId
            << "), ProductUnit(" << unit.productId << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0;
    }

    return m_value.toUInt() * unit.baseUnitEquivalent;
}

qlonglong ProductQuantity::toLongLong(const ProductUnit& unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace()
            << "Product ID mismatch: ProductQuantity(" << m_productId
            << "), ProductUnit(" << unit.productId << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0LL;
    }

    return m_value.toLongLong() * unit.baseUnitEquivalent;
}

qulonglong ProductQuantity::toULongLong(const ProductUnit& unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace()
            << "Product ID mismatch: ProductQuantity(" << m_productId
            << "), StockProductUnit(" << unit.productId << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0ULL;
    }

    return m_value.toULongLong() * unit.baseUnitEquivalent;
}

long double ProductQuantity::toLongDouble(const ProductUnit& unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace()
            << "Product ID mismatch: ProductQuantity(" << m_productId
            << "), ProductUnit(" << unit.productId << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0.0L;
    }

    return m_value.toLongDouble() * unit.baseUnitEquivalent;
}

double ProductQuantity::toDouble(const ProductUnit& unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace()
            << "Product ID mismatch: ProductQuantity(" << m_productId
            << "), ProductUnit(" << unit.productId << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0.0;
    }

    return m_value.toDouble() * unit.baseUnitEquivalent;
}

float ProductQuantity::toFloat(const ProductUnit& unit) const
{
    if (m_productId != unit.productId) {
        qCWarning(lcstockproductquantity).nospace()
            << "Product ID mismatch: ProductQuantity(" << m_productId
            << "), ProductUnit(" << unit.productId << "). Aborting...";
        qCWarning(lcstockproductquantity).space();
        return 0.0f;
    }

    return m_value.toFloat() * unit.baseUnitEquivalent;
}

QString ProductQuantity::toString() const
{
    QString result = QString::fromStdString(toStdString())
                         .remove(QRegularExpression("^(\\d+\\.\\d*?[1-9])0+$"));
    if (result.endsWith(".0000"))
        result.remove(".0000");

    return result;
}

std::string ProductQuantity::toStdString() const
{
    return m_value.toString();
}

QString ProductQuantity::intPart() const
{
    return QString::fromStdString(m_value.getIntPart());
}

QString ProductQuantity::decPart() const
{
    return QString::fromStdString(m_value.getDecPart());
}

const BigDecimal& ProductQuantity::value() const
{
    return m_value;
}

ProductQuantity ProductQuantity::fromValue(const BigDecimal& num)
{
    return ProductQuantity{QString::fromStdString(num.toString())};
}
