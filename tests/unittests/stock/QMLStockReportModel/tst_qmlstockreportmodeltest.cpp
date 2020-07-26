#include "qmlapi/stock/qmlstockreportmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLStockReportModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLStockReportModel *m_stockReportModel;
    MockDatabaseThread *m_thread;
};

void QMLStockReportModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_stockReportModel = new QMLStockReportModel(*m_thread, this);
}

void QMLStockReportModelTest::cleanup()
{
    m_stockReportModel->deleteLater();
    m_thread->deleteLater();
}

void QMLStockReportModelTest::testModel()
{
    QAbstractItemModelTester(m_stockReportModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_GUILESS_MAIN(QMLStockReportModelTest)

#include "tst_qmlstockreportmodeltest.moc"
