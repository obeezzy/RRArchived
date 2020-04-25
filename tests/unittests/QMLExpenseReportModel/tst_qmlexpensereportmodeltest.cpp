#include "qmlapi/qmlexpensereportmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLExpenseReportModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLExpenseReportModel *m_expenseReportModel;
    MockDatabaseThread *m_thread;
};

void QMLExpenseReportModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_expenseReportModel = new QMLExpenseReportModel(*m_thread, this);
}

void QMLExpenseReportModelTest::cleanup()
{
    m_expenseReportModel->deleteLater();
    m_thread->deleteLater();
}

void QMLExpenseReportModelTest::testModel()
{
    QAbstractItemModelTester(m_expenseReportModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_MAIN(QMLExpenseReportModelTest)

#include "tst_qmlexpensereportmodeltest.moc"
