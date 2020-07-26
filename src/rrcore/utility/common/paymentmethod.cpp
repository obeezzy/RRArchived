#include "paymentmethod.h"

using namespace Utility;

Q_LOGGING_CATEGORY(lcpaymentmethod, "rrcore.utility.common.paymentmethod");

PaymentMethod::PaymentMethod(Enum enumValue) : m_enumValue(enumValue) {}

PaymentMethod::PaymentMethod(int intValue)
{
    switch (intValue) {
        case PaymentMethod::DebitCard:
            m_enumValue = PaymentMethod::DebitCard;
            break;
        case PaymentMethod::CreditCard:
            m_enumValue = PaymentMethod::CreditCard;
            break;
        case PaymentMethod::Cash:
            m_enumValue = PaymentMethod::Cash;
            break;
        default:
            m_enumValue = PaymentMethod::Cash;
            qCWarning(lcpaymentmethod) << "Invalid integer value passed as "
                                          "PaymentMethod. Defaulting to "
                                          "PaymentMethod::Cash...";
    }
}

PaymentMethod::PaymentMethod(const QString& stringValue)
{
    if (stringValue == QStringLiteral("debit_card")) {
        m_enumValue = PaymentMethod::DebitCard;
    } else if (stringValue == QStringLiteral("credit_card")) {
        m_enumValue = PaymentMethod::CreditCard;
    } else if (stringValue == QStringLiteral("cash")) {
        m_enumValue = PaymentMethod::Cash;
    } else {
        m_enumValue = PaymentMethod::Cash;
        qCWarning(lcpaymentmethod)
            << "Invalid string value passed as PaymentMethod:" << stringValue
            << ". Defaulting to PaymentMethod::Cash...";
    }
}

PaymentMethod::PaymentMethod(const PaymentMethod& other)
    : m_enumValue(other.enumValue())
{}

PaymentMethod& PaymentMethod::operator=(const PaymentMethod& other)
{
    m_enumValue = other.enumValue();
    return *this;
}

PaymentMethod::Enum PaymentMethod::enumValue() const
{
    return m_enumValue;
}

QString PaymentMethod::toString() const
{
    switch (m_enumValue) {
        case PaymentMethod::DebitCard:
            return QStringLiteral("debit_card");
        case PaymentMethod::CreditCard:
            return QStringLiteral("credit_card");
        default:
            break;
    }

    return QStringLiteral("cash");
    ;
}
