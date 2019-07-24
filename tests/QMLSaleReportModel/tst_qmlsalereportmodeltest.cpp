#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlsalereportmodel.h"
#include "mockdatabasethread.h"

class QMLSaleReportModelTest : public QObject
{
    Q_OBJECT

public:
    QMLSaleReportModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLSaleReportModel *m_saleReportModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLSaleReportModelTest::QMLSaleReportModelTest() :
    m_thread(&m_result)
{
    m_saleReportModel = new QMLSaleReportModel(m_thread, this);
}

void QMLSaleReportModelTest::init()
{

}

void QMLSaleReportModelTest::cleanup()
{

}

void QMLSaleReportModelTest::test_case1()
{

}

QTEST_MAIN(QMLSaleReportModelTest)

#include "tst_qmlsalereportmodeltest.moc"
