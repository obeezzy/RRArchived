#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlpurchasereportmodel.h"
#include "mockdatabasethread.h"

class QMLPurchaseReportModelTest : public QObject
{
    Q_OBJECT

public:
    QMLPurchaseReportModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLPurchaseReportModel *m_purchaseReportModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLPurchaseReportModelTest::QMLPurchaseReportModelTest() :
    m_thread(&m_result)
{
    m_purchaseReportModel = new QMLPurchaseReportModel(m_thread, this);
}

void QMLPurchaseReportModelTest::init()
{

}

void QMLPurchaseReportModelTest::cleanup()
{

}

void QMLPurchaseReportModelTest::test_case1()
{

}

QTEST_MAIN(QMLPurchaseReportModelTest)

#include "tst_qmlpurchasereportmodeltest.moc"
