#include <QCoreApplication>
#include <QtTest>
#include "mockdatabasethread.h"
#include "qmlapi/purchase/qmlpurchasecartmodel.h"

class QMLPurchaseCartModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();

private:
    QMLPurchaseCartModel* m_purchaseCartModel;
    MockDatabaseThread* m_thread;
};

void QMLPurchaseCartModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_purchaseCartModel = new QMLPurchaseCartModel(*m_thread, this);
}

void QMLPurchaseCartModelTest::cleanup()
{
    m_purchaseCartModel->deleteLater();
    m_thread->deleteLater();
}

void QMLPurchaseCartModelTest::testModel()
{
    QAbstractItemModelTester(
        m_purchaseCartModel,
        QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

QTEST_GUILESS_MAIN(QMLPurchaseCartModelTest)

#include "tst_qmlpurchasecartmodeltest.moc"
