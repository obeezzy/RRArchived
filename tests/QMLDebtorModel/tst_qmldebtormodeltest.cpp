#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmldebtormodel.h"
#include "qmlapi/qmldebtorpusher.h"
#include "databaseclient.h"

class QMLDebtorModelTest : public QObject
{
    Q_OBJECT

public:
    QMLDebtorModelTest();

private Q_SLOTS:
    void init();
    void cleanup();
    void testCase1();

private:
    QMLDebtorPusher *m_debtorPusher;
    QMLDebtorModel *m_debtorModel;
    DatabaseClient *m_client;
};

QMLDebtorModelTest::QMLDebtorModelTest()
{
}

void QMLDebtorModelTest::init()
{
}

void QMLDebtorModelTest::cleanup()
{
}

void QMLDebtorModelTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_MAIN(QMLDebtorModelTest)

#include "tst_qmldebtormodeltest.moc"
