#ifndef STOCKQUANTITY_H
#define STOCKQUANTITY_H

#include "stockproductunit.h"
#include "../../3rdparty/BigDecimal-CPP/BigDecimal.h"
#include <QDebug>
#include <QLoggingCategory>
#include <QString>

namespace Utility {
class StockProductQuantity
{
public:
    explicit StockProductQuantity();
    explicit StockProductQuantity(const QString &num);
    explicit StockProductQuantity(int num);
    explicit StockProductQuantity(qlonglong num);
    explicit StockProductQuantity(qulonglong num);
    explicit StockProductQuantity(float num);
    explicit StockProductQuantity(double num);
    explicit StockProductQuantity(long double num);
    explicit StockProductQuantity(const QVariantMap &map);
    explicit StockProductQuantity(const StockProductQuantity &other);
    ~StockProductQuantity();

    StockProductQuantity &operator=(const StockProductQuantity &other);
    StockProductQuantity operator+(const StockProductQuantity& other) const;
    StockProductQuantity operator-(const StockProductQuantity& other) const;
    StockProductQuantity operator*(const StockProductQuantity& other) const;
    StockProductQuantity operator/(const StockProductQuantity& other) const;
    StockProductQuantity operator%(const StockProductQuantity& other) const;
    StockProductQuantity operator^(const StockProductQuantity& other) const;
    void operator+=(const StockProductQuantity& other);
    void operator-=(const StockProductQuantity& other);
    void operator*=(const StockProductQuantity& other);
    void operator/=(const StockProductQuantity& other);
    void operator^=(const StockProductQuantity& other);
    bool operator > (const StockProductQuantity& other) const;
    bool operator >= (const StockProductQuantity& other) const;
    bool operator == (const StockProductQuantity& other) const;
    bool operator != (const StockProductQuantity& other) const;
    bool operator < (const StockProductQuantity& other) const;
    bool operator <= (const StockProductQuantity& other) const;

    bool isZero() const;

    int productId() const;
    void setProductId(int productId);

    int toInt(const StockProductUnit & = StockProductUnit()) const;
    unsigned int toUInt(const StockProductUnit & = StockProductUnit()) const;
    qlonglong toLongLong(const StockProductUnit & = StockProductUnit()) const;
    qulonglong toULongLong(const StockProductUnit & = StockProductUnit()) const;
    long double toLongDouble(const StockProductUnit & = StockProductUnit()) const;
    double toDouble(const StockProductUnit & = StockProductUnit()) const;
    float toFloat(const StockProductUnit & = StockProductUnit()) const;
    QString toString() const;
    std::string toStdString() const;

    QString intPart() const;
    QString decPart() const;

    const BigDecimal &value() const;

    friend QDebug operator<<(QDebug debug, const StockProductQuantity &quantity)
    {
        debug.nospace() << "StockProductQuantity("
                        << quantity.toString()
                        << ")";

        return debug.nospace();
    }
private:
    BigDecimal m_value;
    int m_productId {0};

    static StockProductQuantity fromValue(const BigDecimal &num);
};
}
Q_DECLARE_TYPEINFO(Utility::StockProductQuantity, Q_PRIMITIVE_TYPE);
Q_DECLARE_LOGGING_CATEGORY(lcstockproductquantity);

#endif // STOCKQUANTITY_H
