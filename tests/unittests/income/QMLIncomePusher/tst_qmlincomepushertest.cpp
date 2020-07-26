#include "qmlapi/income/qmlincomepusher.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLIncomePusherTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
private:
    QMLIncomePusher *m_incomePusher;
    MockDatabaseThread *m_thread;
};

void QMLIncomePusherTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_incomePusher = new QMLIncomePusher(*m_thread, this);
}

void QMLIncomePusherTest::cleanup()
{
    m_incomePusher->deleteLater();
    m_thread->deleteLater();
}

QTEST_GUILESS_MAIN(QMLIncomePusherTest)

#include "tst_qmlincomepushertest.moc"
