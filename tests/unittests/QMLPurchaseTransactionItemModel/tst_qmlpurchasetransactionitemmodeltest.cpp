#include "qmlapi/qmlpurchasetransactionitemmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLPurchaseTransactionItemModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLPurchaseTransactionItemModel *m_purchaseTransactionItemModel;
    MockDatabaseThread *m_thread;
};

void QMLPurchaseTransactionItemModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_purchaseTransactionItemModel = new QMLPurchaseTransactionItemModel(*m_thread, this);
}

void QMLPurchaseTransactionItemModelTest::cleanup()
{
    m_purchaseTransactionItemModel->deleteLater();
    m_thread->deleteLater();
}

void QMLPurchaseTransactionItemModelTest::testModel()
{
    QAbstractItemModelTester(m_purchaseTransactionItemModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_GUILESS_MAIN(QMLPurchaseTransactionItemModelTest)

#include "tst_qmlpurchasetransactionitemmodeltest.moc"
