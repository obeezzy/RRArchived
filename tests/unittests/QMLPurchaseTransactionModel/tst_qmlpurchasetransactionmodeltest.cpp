#include "qmlapi/qmlpurchasetransactionmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLPurchaseTransactionModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLPurchaseTransactionModel *m_purchaseTransactionModel;
    MockDatabaseThread *m_thread;
};

void QMLPurchaseTransactionModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_purchaseTransactionModel = new QMLPurchaseTransactionModel(*m_thread, this);
}

void QMLPurchaseTransactionModelTest::cleanup()
{
    m_purchaseTransactionModel->deleteLater();
    m_thread->deleteLater();
}

void QMLPurchaseTransactionModelTest::testModel()
{
    QAbstractItemModelTester(m_purchaseTransactionModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_MAIN(QMLPurchaseTransactionModelTest)

#include "tst_qmlpurchasetransactionmodeltest.moc"
