#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmldashboardhomemodel.h"
#include "mockdatabasethread.h"

class QMLDashboardHomeModelTest : public QObject
{
    Q_OBJECT

public:
    QMLDashboardHomeModelTest();

private slots:
    void init();
    void cleanup();
    void test_case1();

private:
    QMLDashboardHomeModel *m_dashboardHomeModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLDashboardHomeModelTest::QMLDashboardHomeModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLDashboardHomeModelTest::init()
{
    m_dashboardHomeModel = new QMLDashboardHomeModel(m_thread, this);
}

void QMLDashboardHomeModelTest::cleanup()
{
}

void QMLDashboardHomeModelTest::test_case1()
{

}

QTEST_MAIN(QMLDashboardHomeModelTest)

#include "tst_qmldashboardhomemodeltest.moc"
