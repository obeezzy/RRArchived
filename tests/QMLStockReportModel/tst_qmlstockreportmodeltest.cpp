#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockreportmodel.h"
#include "mockdatabasethread.h"

class QMLStockReportModelTest : public QObject
{
    Q_OBJECT

public:
    QMLStockReportModelTest();
    ~QMLStockReportModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLStockReportModel *m_stockReportModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLStockReportModelTest::QMLStockReportModelTest() :
    m_thread(&m_result)
{
    m_stockReportModel = new QMLStockReportModel(m_thread);
}

QMLStockReportModelTest::~QMLStockReportModelTest()
{
    m_stockReportModel->deleteLater();
}

void QMLStockReportModelTest::init()
{

}

void QMLStockReportModelTest::cleanup()
{

}

void QMLStockReportModelTest::test_case1()
{

}

QTEST_MAIN(QMLStockReportModelTest)

#include "tst_qmlstockreportmodeltest.moc"
