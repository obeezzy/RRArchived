#include "qmldoublevalidator.h"
#include <QDebug>

QMLDoubleValidator::QMLDoubleValidator(QDoubleValidator *parent) :
    QDoubleValidator(parent)
{
    setNotation(QDoubleValidator::StandardNotation);
}

QMLDoubleValidator::QMLDoubleValidator(double bottom, double top, int decimals, QObject *parent) :
    QDoubleValidator(bottom, top, decimals, parent)
{
    setNotation(QDoubleValidator::StandardNotation);
}

QValidator::State QMLDoubleValidator::validate(QString &input, int &pos) const {
    Q_UNUSED(pos)
    if (input.isEmpty() || input.startsWith("-")) {
        // allow empty field or minus sign
        return QValidator::Intermediate;
    }
    // check length of decimal places
    QChar point = locale().decimalPoint();
    if (input.indexOf(point) != -1) {
        int lengthDecimals = input.length() - input.indexOf(point) - 1;
        if (lengthDecimals > decimals()) {
            return QValidator::Invalid;
        }
    }
    // check range of value
    bool isNumber;
    double value = locale().toDouble(input, &isNumber);
    if (isNumber && bottom() <= value && value <= top()) {
        return QValidator::Acceptable;
    }
    return QValidator::Invalid;
}
