#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlincomehomemodel.h"
#include "mockdatabasethread.h"

class QMLIncomeHomeModelTest : public QObject
{
    Q_OBJECT

public:
    QMLIncomeHomeModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLIncomeHomeModel *m_incomeHomeModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLIncomeHomeModelTest::QMLIncomeHomeModelTest() :
    m_thread(&m_result)
{
    m_incomeHomeModel = new QMLIncomeHomeModel(m_thread, this);
}

void QMLIncomeHomeModelTest::init()
{

}

void QMLIncomeHomeModelTest::cleanup()
{

}

void QMLIncomeHomeModelTest::test_case1()
{

}

QTEST_MAIN(QMLIncomeHomeModelTest)

#include "tst_qmlincomehomemodeltest.moc"
