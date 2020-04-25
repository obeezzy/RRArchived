#include "qmlapi/qmldoublevalidator.h"
#include <QtTest>
#include <QCoreApplication>

class QMLDoubleValidatorTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testRandomInput_data();
    void testRandomInput();
    void testDisallowNegativeInput();
private:
    QMLDoubleValidator *m_doubleValidator;
};
Q_DECLARE_METATYPE(QValidator::State);

void QMLDoubleValidatorTest::init()
{
    m_doubleValidator = new QMLDoubleValidator(1.0, 100.0, 4, this);
}

void QMLDoubleValidatorTest::cleanup()
{
    m_doubleValidator->deleteLater();
}

void QMLDoubleValidatorTest::testRandomInput_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<int>("pos");
    QTest::addColumn<QValidator::State>("state");

    QTest::newRow("Number within range") << "50" << 0 << QValidator::Acceptable;
    QTest::newRow("Number with 2 decimals") << "40.44" << 0 << QValidator::Acceptable;
    QTest::newRow("Number with 4 decimals") << "10.3823" << 0 << QValidator::Acceptable;
    QTest::newRow("Minimum") << "1.0000" << 0 << QValidator::Acceptable;
    QTest::newRow("Maximum") << "100.0000" << 0 << QValidator::Acceptable;

    QTest::newRow("Null string") << QString() << 0 << QValidator::Intermediate;
    QTest::newRow("Empty string") << "" << 0 << QValidator::Intermediate;

    QTest::newRow("Negative number") << "-10" << 0 << QValidator::Invalid;
    QTest::newRow("Number below range") << "0.3" << 0 << QValidator::Invalid;
    QTest::newRow("Number above range") << "101" << 0 << QValidator::Invalid;
    QTest::newRow("Number with 5 decimals") << "2.35382" << 0 << QValidator::Invalid;
    QTest::newRow("Alphabetic characters") << "Not gonna work" << 0 << QValidator::Invalid;
    QTest::newRow("Alphanumeric characters") << "9ja" << 0 << QValidator::Invalid;
    QTest::newRow("Random characters") << "@!$8($" << 0 << QValidator::Invalid;
}

void QMLDoubleValidatorTest::testRandomInput()
{
    QFETCH(QString, input);
    QFETCH(int, pos);
    QFETCH(QValidator::State, state);

    QCOMPARE(m_doubleValidator->validate(input, pos), state);
}

void QMLDoubleValidatorTest::testDisallowNegativeInput()
{
    QMLDoubleValidator validator(-11.0, 100.0, 4, this);
    QCOMPARE(validator.bottom(), 0.0);
}

QTEST_MAIN(QMLDoubleValidatorTest)

#include "tst_qmldoublevalidatortest.moc"
