#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlexpensepusher.h"
#include "mockdatabasethread.h"

class QMLExpensePusherTest : public QObject
{
    Q_OBJECT

public:
    QMLExpensePusherTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLExpensePusher *m_expensePusher;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLExpensePusherTest::QMLExpensePusherTest() :
    m_thread(&m_result)
{
    m_expensePusher = new QMLExpensePusher(m_thread, this);
}

void QMLExpensePusherTest::init()
{

}

void QMLExpensePusherTest::cleanup()
{

}

void QMLExpensePusherTest::test_case1()
{

}

QTEST_MAIN(QMLExpensePusherTest)

#include "tst_qmlexpensepushertest.moc"
