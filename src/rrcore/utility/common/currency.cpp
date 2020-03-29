#include "currency.h"
#include "singletons/settings.h"
#include <QHash>
#include <QLocale>

Q_LOGGING_CATEGORY(lccurrency, "rrcore.utility.common.currency")

using namespace Utility;

inline static const QHash<Currency::Code, QString> isoCodeMap = {
    { Currency::Code::NGN, QStringLiteral("NGN") }
};

Currency::Currency(const QString &isoCode) :
    m_code(isoCodeMap.key(isoCode.isNull() ? DEFAULT_CURRENCY
                                           : isoCode,
                          Currency::Code::Unsupported))
{
    if (!isoCode.isNull() && m_code == Code::Unsupported)
        qCWarning(lccurrency) << "Invalid currency code:"
                              << isoCode
                              << ". Code unsupported.";
}

Currency::Currency(const Currency::Code &code) :
    m_code(code)
{}

Currency::Currency(const Currency &other) :
    m_code(other.code())
{}

Currency &Currency::operator=(const Currency &other)
{
    m_code = other.code();
    return *this;
}

Currency::Code Currency::code() const
{
    return m_code;
}

QString Currency::isoCode() const
{
    return Settings::instance().locale()
            .currencySymbol(QLocale::CurrencyIsoCode);
}

QString Currency::symbol() const
{
    return Settings::instance().locale()
            .currencySymbol(QLocale::CurrencySymbol);
}

QString Currency::displayName() const
{
    return Settings::instance().locale()
            .currencySymbol(QLocale::CurrencyDisplayName);
}

