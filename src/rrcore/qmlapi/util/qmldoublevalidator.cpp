#include "qmldoublevalidator.h"
#include <QDebug>

QMLDoubleValidator::QMLDoubleValidator(QObject *parent) :
    QDoubleValidator(parent)
{
    setNotation(QDoubleValidator::StandardNotation);
}

QMLDoubleValidator::QMLDoubleValidator(double bottom, double top, int decimals, QObject *parent) :
    QDoubleValidator(qMax(0.0, bottom), top, decimals, parent)
{
    setNotation(QDoubleValidator::StandardNotation);
}

QValidator::State QMLDoubleValidator::validate(QString &input, int &pos) const {
    Q_UNUSED(pos)
    if (input.isEmpty())
        return QValidator::Intermediate;
    if (input.startsWith("-"))
        return QValidator::Invalid; // Don't allow negative numbers

    // check length of decimal places
    const QChar &point = locale().decimalPoint();
    if (input.contains(point)) {
        int lengthDecimals = input.length() - input.indexOf(point) - 1;
        if (lengthDecimals > decimals())
            return QValidator::Invalid;
    }

    bool isNumber;
    const double value = locale().toDouble(input, &isNumber);
    if (isNumber && bottom() <= value && value <= top())
        return QValidator::Acceptable;

    return QValidator::Invalid;
}
