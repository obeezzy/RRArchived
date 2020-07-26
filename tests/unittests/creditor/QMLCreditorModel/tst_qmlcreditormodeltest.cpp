#include "qmlapi/creditor/qmlcreditormodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLCreditorModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
private:
    QMLCreditorModel *m_creditorModel;
    MockDatabaseThread *m_thread;
};

void QMLCreditorModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_creditorModel = new QMLCreditorModel(*m_thread, this);
}

void QMLCreditorModelTest::cleanup()
{
    m_creditorModel->deleteLater();
    m_thread->deleteLater();
}

void QMLCreditorModelTest::testModel()
{
    QAbstractItemModelTester(m_creditorModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

QTEST_GUILESS_MAIN(QMLCreditorModelTest)

#include "tst_qmlcreditormodeltest.moc"
