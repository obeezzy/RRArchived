#ifndef QMLDOUBLEVALIDATOR_H
#define QMLDOUBLEVALIDATOR_H

#include <QObject>
#include <QDoubleValidator>

class QMLDoubleValidator : public QDoubleValidator
{
    Q_OBJECT
public:
    explicit QMLDoubleValidator(QObject *parent = nullptr);
    explicit QMLDoubleValidator(double bottom,
                                double top,
                                int decimals,
                                QObject *parent = nullptr);

    QValidator::State validate(QString &input, int &pos) const override final;
};

#endif // QMLDOUBLEVALIDATOR_H
