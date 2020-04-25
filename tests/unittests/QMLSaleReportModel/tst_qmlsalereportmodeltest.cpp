#include "qmlapi/qmlsalereportmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLSaleReportModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLSaleReportModel *m_saleReportModel;
    MockDatabaseThread *m_thread;
};

void QMLSaleReportModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_saleReportModel = new QMLSaleReportModel(*m_thread, this);
}

void QMLSaleReportModelTest::cleanup()
{
    m_saleReportModel->deleteLater();
    m_thread->deleteLater();
}

void QMLSaleReportModelTest::testModel()
{
    QAbstractItemModelTester(m_saleReportModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_MAIN(QMLSaleReportModelTest)

#include "tst_qmlsalereportmodeltest.moc"
