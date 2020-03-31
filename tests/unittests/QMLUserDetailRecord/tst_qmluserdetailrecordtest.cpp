#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmluserdetailrecord.h"
#include "mockdatabasethread.h"

class QMLUserDetailRecordTest : public QObject
{
    Q_OBJECT

public:
    QMLUserDetailRecordTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLUserDetailRecord *m_userDetailRecord;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLUserDetailRecordTest::QMLUserDetailRecordTest() :
    m_thread(&m_result)
{
    m_userDetailRecord = new QMLUserDetailRecord(m_thread, this);
}

void QMLUserDetailRecordTest::init()
{

}

void QMLUserDetailRecordTest::cleanup()
{
}

void QMLUserDetailRecordTest::test_case1()
{

}

QTEST_MAIN(QMLUserDetailRecordTest)

#include "tst_qmluserdetailrecordtest.moc"
