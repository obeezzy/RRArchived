#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlexpensereportmodel.h"
#include "mockdatabasethread.h"

class QMLExpenseReportModelTest : public QObject
{
    Q_OBJECT

public:
    QMLExpenseReportModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLExpenseReportModel *m_expenseReportModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLExpenseReportModelTest::QMLExpenseReportModelTest() :
    m_thread(&m_result)
{
    m_expenseReportModel = new QMLExpenseReportModel(m_thread, this);
}

void QMLExpenseReportModelTest::init()
{

}

void QMLExpenseReportModelTest::cleanup()
{

}

void QMLExpenseReportModelTest::test_case1()
{

}

QTEST_MAIN(QMLExpenseReportModelTest)

#include "tst_qmlexpensereportmodeltest.moc"
