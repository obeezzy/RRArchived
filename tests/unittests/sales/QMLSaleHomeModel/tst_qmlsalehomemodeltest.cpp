#include <QCoreApplication>
#include <QtTest>

#include "mockdatabasethread.h"
#include "qmlapi/sales/qmlsalehomemodel.h"

class QMLSaleHomeModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();

private:
    QMLSaleHomeModel* m_saleHomeModel;
    MockDatabaseThread* m_thread;
};

void QMLSaleHomeModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_saleHomeModel = new QMLSaleHomeModel(*m_thread, this);
}

void QMLSaleHomeModelTest::cleanup()
{
    m_saleHomeModel->deleteLater();
    m_thread->deleteLater();
}

void QMLSaleHomeModelTest::testModel() {}

QTEST_GUILESS_MAIN(QMLSaleHomeModelTest)

#include "tst_qmlsalehomemodeltest.moc"
