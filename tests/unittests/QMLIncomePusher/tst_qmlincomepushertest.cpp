#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlincomepusher.h"
#include "mockdatabasethread.h"

class QMLIncomePusherTest : public QObject
{
    Q_OBJECT
public:
    QMLIncomePusherTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLIncomePusher *m_incomePusher;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLIncomePusherTest::QMLIncomePusherTest() :
    m_thread(&m_result)
{
    m_incomePusher = new QMLIncomePusher(m_thread, this);
}

void QMLIncomePusherTest::init()
{

}

void QMLIncomePusherTest::cleanup()
{

}

void QMLIncomePusherTest::test_case1()
{

}

QTEST_MAIN(QMLIncomePusherTest)

#include "tst_qmlincomepushertest.moc"
