#include "qmlapi/qmluserdetailrecord.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLUserDetailRecordTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
private:
    QMLUserDetailRecord *m_userDetailRecord;
    MockDatabaseThread *m_thread;
};

void QMLUserDetailRecordTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_userDetailRecord = new QMLUserDetailRecord(*m_thread, this);
}

void QMLUserDetailRecordTest::cleanup()
{
    m_userDetailRecord->deleteLater();
    m_thread->deleteLater();
}

QTEST_GUILESS_MAIN(QMLUserDetailRecordTest)

#include "tst_qmluserdetailrecordtest.moc"
