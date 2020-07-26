#include <QtTest>
#include <QCoreApplication>

#include "mockdatabasethread.h"
#include "qmlapi/debtor/qmldebtordetailrecord.h"

class QMLDebtorDetailRecordTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testFetchDebtor();
private:
    QMLDebtorDetailRecord *m_debtorDetailRecord;
    MockDatabaseThread *m_thread;
};

void QMLDebtorDetailRecordTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_debtorDetailRecord = new QMLDebtorDetailRecord(*m_thread, this);
}

void QMLDebtorDetailRecordTest::cleanup()
{
    m_debtorDetailRecord->deleteLater();
    m_thread->deleteLater();
}

void QMLDebtorDetailRecordTest::testFetchDebtor()
{
    const QDateTime &currentDateTime(QDateTime::currentDateTime());
    const QVariantMap debtor {
        { "debtor_id", 1 },
        { "preferred_name", QStringLiteral("Preferred name") },
        { "first_name", QStringLiteral("First name") },
        { "last_name", QStringLiteral("Last name") },
        { "phone_number", QStringLiteral("123456789") },
        { "addresses", QStringList { QStringLiteral("Address1"), QStringLiteral("Address2") } },
        { "emails", QStringList { QStringLiteral("Email1"), QStringLiteral("Email2") } },
        { "created", currentDateTime },
        { "last_edited", currentDateTime },
        { "user_id", 1 },
        { "username", QStringLiteral("username")}
    };
    auto databaseWillReturn = [this](const QVariantMap &debtor) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(debtor);
    };
    QSignalSpy debtorIdChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::debtorIdChanged);
    QSignalSpy preferredNameChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::preferredNameChanged);
    QSignalSpy firstNameChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::firstNameChanged);
    QSignalSpy lastNameChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::lastNameChanged);
    QSignalSpy phoneNumberChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::phoneNumberChanged);
    QSignalSpy addressModelChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::addressModelChanged);
    QSignalSpy emailModelChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::emailModelChanged);
    QSignalSpy createdChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::createdChanged);
    QSignalSpy lastEditedChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::lastEditedChanged);
    QSignalSpy userIdChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::userIdChanged);
    QSignalSpy userChangedSpy(m_debtorDetailRecord, &QMLDebtorDetailRecord::userChanged);

    m_debtorDetailRecord->componentComplete();

    QCOMPARE(m_debtorDetailRecord->debtorId(), 0);
    QCOMPARE(m_debtorDetailRecord->preferredName(), QString());
    QCOMPARE(m_debtorDetailRecord->firstName(), QString());
    QCOMPARE(m_debtorDetailRecord->lastName(), QString());
    QCOMPARE(m_debtorDetailRecord->phoneNumber(), QString());
    QCOMPARE(m_debtorDetailRecord->addressModel(), QStringList());
    QCOMPARE(m_debtorDetailRecord->emailModel(), QStringList());
    QCOMPARE(m_debtorDetailRecord->created(), QDateTime());
    QCOMPARE(m_debtorDetailRecord->lastEdited(), QDateTime());
    QCOMPARE(m_debtorDetailRecord->userId(), 0);
    QCOMPARE(m_debtorDetailRecord->user(), QString());

    databaseWillReturn(debtor);

    m_debtorDetailRecord->setDebtorId(debtor["debtor_id"].toInt());

    QCOMPARE(m_debtorDetailRecord->debtorId(), debtor["debtor_id"].toInt());
    QCOMPARE(m_debtorDetailRecord->preferredName(), debtor["preferred_name"].toString());
    QCOMPARE(m_debtorDetailRecord->firstName(), debtor["first_name"].toString());
    QCOMPARE(m_debtorDetailRecord->lastName(), debtor["last_name"].toString());
    QCOMPARE(m_debtorDetailRecord->phoneNumber(), debtor["phone_number"].toString());
    QCOMPARE(m_debtorDetailRecord->created(), currentDateTime);
    QCOMPARE(m_debtorDetailRecord->lastEdited(), currentDateTime);
    QCOMPARE(m_debtorDetailRecord->userId(), debtor["user_id"].toInt());
    QCOMPARE(m_debtorDetailRecord->user(), debtor["username"].toString());
}

QTEST_GUILESS_MAIN(QMLDebtorDetailRecordTest)

#include "tst_qmldebtordetailrecordtest.moc"
