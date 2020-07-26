#include <QCoreApplication>
#include <QtTest>
#include "mockdatabasethread.h"
#include "qmlapi/user/qmluserprivilegemodel.h"

class QMLUserPrivilegeModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();

private:
    QMLUserPrivilegeModel* m_userPrivilegeModel;
    MockDatabaseThread* m_thread;
};

void QMLUserPrivilegeModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_userPrivilegeModel = new QMLUserPrivilegeModel(*m_thread, this);
}

void QMLUserPrivilegeModelTest::cleanup()
{
    m_userPrivilegeModel->deleteLater();
    m_thread->deleteLater();
}

void QMLUserPrivilegeModelTest::testModel()
{
    QAbstractItemModelTester(
        m_userPrivilegeModel,
        QAbstractItemModelTester::FailureReportingMode::Fatal, this);
}

QTEST_GUILESS_MAIN(QMLUserPrivilegeModelTest)

#include "tst_qmluserprivilegemodeltest.moc"
