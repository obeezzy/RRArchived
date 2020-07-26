#include "qmlapi/user/qmlusermodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLUserModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testModel();
private:
    QMLUserModel *m_userModel;
    MockDatabaseThread *m_thread;
};

void QMLUserModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_userModel = new QMLUserModel(*m_thread, this);
}

void QMLUserModelTest::cleanup()
{
    m_userModel->deleteLater();
    m_thread->deleteLater();
}

void QMLUserModelTest::testModel()
{
    QAbstractItemModelTester(m_userModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_GUILESS_MAIN(QMLUserModelTest)

#include "tst_qmlusermodeltest.moc"
