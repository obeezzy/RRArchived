#ifndef PAYMENTMETHOD_H
#define PAYMENTMETHOD_H

#include <QString>
#include <QLoggingCategory>

namespace Utility {
class PaymentMethod
{
public:
    enum Enum { Cash, DebitCard, CreditCard };

    explicit PaymentMethod() = default;
    PaymentMethod(Enum enumValue);
    explicit PaymentMethod(int intValue);
    explicit PaymentMethod(const QString &stringValue);
    explicit PaymentMethod(const PaymentMethod &other);
    PaymentMethod &operator=(const PaymentMethod &other);
    inline bool operator==(const PaymentMethod &other) const {
        return m_enumValue == other.enumValue();
    }

    Enum enumValue() const;
    QString toString() const;

    friend QDebug operator<<(QDebug debug, const PaymentMethod &method)
    {
        debug.nospace() << "PaymentMethod("
                        << method.toString()
                        << ")";

        return debug.nospace();
    }
private:
    Enum m_enumValue {Cash};
};
}
Q_DECLARE_TYPEINFO(Utility::PaymentMethod, Q_PRIMITIVE_TYPE);
Q_DECLARE_LOGGING_CATEGORY(lcpaymentmethod);

#endif // PAYMENTMETHOD_H
