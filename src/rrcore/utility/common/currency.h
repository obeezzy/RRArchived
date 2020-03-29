#ifndef CURRENCY_H
#define CURRENCY_H

#include <QString>
#include <QLocale>
#include <QLoggingCategory>

namespace Utility {
class Currency
{
public:
    enum Code {
        Unsupported,
        NGN
    };
    explicit Currency(const QString &isoCode = QString());
    explicit Currency(const Code &code);
    explicit Currency(const Currency &other);
    Currency &operator=(const Currency &other);
    ~Currency() = default;

    Code code() const;
    QString isoCode() const;
    QString symbol() const;
    QString displayName() const;

    static inline const QString DEFAULT_CURRENCY = QStringLiteral("NGN");
private:
    Code m_code;
};
}
Q_DECLARE_TYPEINFO(Utility::Currency, Q_PRIMITIVE_TYPE);
Q_DECLARE_LOGGING_CATEGORY(lccurrency)

#endif // CURRENCY_H
