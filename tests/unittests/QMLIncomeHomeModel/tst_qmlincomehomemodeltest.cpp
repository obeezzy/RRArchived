#include "qmlapi/qmlincomehomemodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLIncomeHomeModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLIncomeHomeModel *m_incomeHomeModel;
    MockDatabaseThread *m_thread;
};

void QMLIncomeHomeModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_incomeHomeModel = new QMLIncomeHomeModel(*m_thread, this);
}

void QMLIncomeHomeModelTest::cleanup()
{
    m_incomeHomeModel->deleteLater();
    m_thread->deleteLater();
}

void QMLIncomeHomeModelTest::testModel()
{
    QAbstractItemModelTester(m_incomeHomeModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_MAIN(QMLIncomeHomeModelTest)

#include "tst_qmlincomehomemodeltest.moc"
