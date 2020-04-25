#include "qmlapi/qmlexpensehomemodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLExpenseHomeModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLExpenseHomeModel *m_expenseHomeModel;
    MockDatabaseThread *m_thread;
};

void QMLExpenseHomeModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_expenseHomeModel = new QMLExpenseHomeModel(*m_thread, this);
}

void QMLExpenseHomeModelTest::cleanup()
{
    m_expenseHomeModel->deleteLater();
    m_thread->deleteLater();
}

void QMLExpenseHomeModelTest::testModel()
{
    QAbstractItemModelTester(m_expenseHomeModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_MAIN(QMLExpenseHomeModelTest)

#include "tst_qmlexpensehomemodeltest.moc"
