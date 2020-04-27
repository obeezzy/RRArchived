#include "qmlapi/qmlpurchasehomemodel.h"
#include "database/databasethread.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLPurchaseHomeModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLPurchaseHomeModel *m_purchaseHomeModel;
    MockDatabaseThread *m_thread;
};

void QMLPurchaseHomeModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_purchaseHomeModel = new QMLPurchaseHomeModel(*m_thread, this);
}

void QMLPurchaseHomeModelTest::cleanup()
{
    m_purchaseHomeModel->deleteLater();
    m_thread->deleteLater();
}

void QMLPurchaseHomeModelTest::testModel()
{
    QAbstractItemModelTester(m_purchaseHomeModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_GUILESS_MAIN(QMLPurchaseHomeModelTest)

#include "tst_qmlpurchasehomemodeltest.moc"
