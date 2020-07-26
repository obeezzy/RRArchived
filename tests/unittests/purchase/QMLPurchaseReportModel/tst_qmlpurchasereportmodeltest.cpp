#include <QCoreApplication>
#include <QtTest>

#include "mockdatabasethread.h"
#include "qmlapi/purchase/qmlpurchasereportmodel.h"

class QMLPurchaseReportModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();

private:
    QMLPurchaseReportModel* m_purchaseReportModel;
    MockDatabaseThread* m_thread;
};

void QMLPurchaseReportModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_purchaseReportModel = new QMLPurchaseReportModel(*m_thread, this);
}

void QMLPurchaseReportModelTest::cleanup()
{
    m_purchaseReportModel->deleteLater();
    m_thread->deleteLater();
}

void QMLPurchaseReportModelTest::testModel()
{
    QAbstractItemModelTester(
        m_purchaseReportModel,
        QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

QTEST_GUILESS_MAIN(QMLPurchaseReportModelTest)

#include "tst_qmlpurchasereportmodeltest.moc"
