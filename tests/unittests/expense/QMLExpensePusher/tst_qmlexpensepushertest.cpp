#include <QCoreApplication>
#include <QtTest>
#include "mockdatabasethread.h"
#include "qmlapi/expense/qmlexpensepusher.h"

class QMLExpensePusherTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

private:
    QMLExpensePusher* m_expensePusher;
    MockDatabaseThread* m_thread;
};

void QMLExpensePusherTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_expensePusher = new QMLExpensePusher(*m_thread, this);
}

void QMLExpensePusherTest::cleanup()
{
    m_expensePusher->deleteLater();
    m_thread->deleteLater();
}

QTEST_GUILESS_MAIN(QMLExpensePusherTest)

#include "tst_qmlexpensepushertest.moc"
