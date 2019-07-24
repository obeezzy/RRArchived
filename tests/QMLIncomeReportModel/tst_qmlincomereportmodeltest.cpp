#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlincomereportmodel.h"
#include "mockdatabasethread.h"

class QMLIncomeReportModelTest : public QObject
{
    Q_OBJECT

public:
    QMLIncomeReportModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLIncomeReportModel *m_incomeReportModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLIncomeReportModelTest::QMLIncomeReportModelTest() :
    m_thread(&m_result)
{
    m_incomeReportModel = new QMLIncomeReportModel(m_thread, this);
}

void QMLIncomeReportModelTest::init()
{

}

void QMLIncomeReportModelTest::cleanup()
{

}

void QMLIncomeReportModelTest::test_case1()
{

}

QTEST_MAIN(QMLIncomeReportModelTest)

#include "tst_qmlincomereportmodeltest.moc"
