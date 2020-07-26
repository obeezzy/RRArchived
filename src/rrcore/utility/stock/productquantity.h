#ifndef PRODUCTQUANTITY_H
#define PRODUCTQUANTITY_H

#include "productunit.h"
#include "../../3rdparty/BigDecimal-CPP/BigDecimal.h"
#include <QDebug>
#include <QLoggingCategory>
#include <QString>

namespace Utility {
    namespace Stock {
    class ProductQuantity
    {
    public:
        explicit ProductQuantity();
        explicit ProductQuantity(const QString &num);
        explicit ProductQuantity(int num);
        explicit ProductQuantity(qlonglong num);
        explicit ProductQuantity(qulonglong num);
        explicit ProductQuantity(float num);
        explicit ProductQuantity(double num);
        explicit ProductQuantity(long double num);
        explicit ProductQuantity(const QVariantMap &map);
        explicit ProductQuantity(const ProductQuantity &other);
        ~ProductQuantity();

        ProductQuantity &operator=(const ProductQuantity &other);
        ProductQuantity operator+(const ProductQuantity& other) const;
        ProductQuantity operator-(const ProductQuantity& other) const;
        ProductQuantity operator*(const ProductQuantity& other) const;
        ProductQuantity operator/(const ProductQuantity& other) const;
        ProductQuantity operator%(const ProductQuantity& other) const;
        ProductQuantity operator^(const ProductQuantity& other) const;
        void operator+=(const ProductQuantity& other);
        void operator-=(const ProductQuantity& other);
        void operator*=(const ProductQuantity& other);
        void operator/=(const ProductQuantity& other);
        void operator^=(const ProductQuantity& other);
        bool operator > (const ProductQuantity& other) const;
        bool operator >= (const ProductQuantity& other) const;
        bool operator == (const ProductQuantity& other) const;
        bool operator != (const ProductQuantity& other) const;
        bool operator < (const ProductQuantity& other) const;
        bool operator <= (const ProductQuantity& other) const;

        bool isZero() const;

        int productId() const;
        void setProductId(int productId);

        int toInt(const ProductUnit & = ProductUnit()) const;
        unsigned int toUInt(const ProductUnit & = ProductUnit()) const;
        qlonglong toLongLong(const ProductUnit & = ProductUnit()) const;
        qulonglong toULongLong(const ProductUnit & = ProductUnit()) const;
        long double toLongDouble(const ProductUnit & = ProductUnit()) const;
        double toDouble(const ProductUnit & = ProductUnit()) const;
        float toFloat(const ProductUnit & = ProductUnit()) const;
        QString toString() const;
        std::string toStdString() const;

        QString intPart() const;
        QString decPart() const;

        const BigDecimal &value() const;

        friend QDebug operator<<(QDebug debug, const ProductQuantity &quantity)
        {
            debug.nospace() << "ProductQuantity("
                            << quantity.toString()
                            << ")";

            return debug.nospace();
        }
    private:
        BigDecimal m_value;
        int m_productId {0};

        static ProductQuantity fromValue(const BigDecimal &num);
    };
}
}
Q_DECLARE_TYPEINFO(Utility::Stock::ProductQuantity, Q_PRIMITIVE_TYPE);
Q_DECLARE_LOGGING_CATEGORY(lcstockproductquantity);

#endif // PRODUCTQUANTITY_H
