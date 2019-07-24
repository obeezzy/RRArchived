#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmluserprivilegemodel.h"
#include "mockdatabasethread.h"

class QMLUserPrivilegeModelTest : public QObject
{
    Q_OBJECT
public:
    QMLUserPrivilegeModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLUserPrivilegeModel *m_userPrivilegeModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLUserPrivilegeModelTest::QMLUserPrivilegeModelTest() :
    m_thread(&m_result)
{
    m_userPrivilegeModel = new QMLUserPrivilegeModel(m_thread, this);
}

void QMLUserPrivilegeModelTest::init()
{

}

void QMLUserPrivilegeModelTest::cleanup()
{

}

void QMLUserPrivilegeModelTest::test_case1()
{

}

QTEST_MAIN(QMLUserPrivilegeModelTest)

#include "tst_qmluserprivilegemodeltest.moc"
