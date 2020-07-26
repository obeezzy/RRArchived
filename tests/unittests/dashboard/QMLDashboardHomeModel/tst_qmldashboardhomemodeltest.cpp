#include <QCoreApplication>
#include <QtTest>
#include "mockdatabasethread.h"
#include "qmlapi/dashboard/qmldashboardhomemodel.h"

class QMLDashboardHomeModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();

private:
    QMLDashboardHomeModel* m_dashboardHomeModel;
    MockDatabaseThread* m_thread;
};

void QMLDashboardHomeModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_dashboardHomeModel = new QMLDashboardHomeModel(*m_thread, this);
}

void QMLDashboardHomeModelTest::cleanup()
{
    m_dashboardHomeModel->deleteLater();
    m_thread->deleteLater();
}

void QMLDashboardHomeModelTest::testModel()
{
    QAbstractItemModelTester(
        m_dashboardHomeModel,
        QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

QTEST_GUILESS_MAIN(QMLDashboardHomeModelTest)

#include "tst_qmldashboardhomemodeltest.moc"
