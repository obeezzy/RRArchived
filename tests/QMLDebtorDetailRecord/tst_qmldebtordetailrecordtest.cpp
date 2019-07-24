#include <QtTest>
#include <QCoreApplication>

#include "mockdatabasethread.h"
#include "qmlapi/qmldebtordetailrecord.h"

class QMLDebtorDetailRecordTest : public QObject
{
    Q_OBJECT

public:
    QMLDebtorDetailRecordTest();

private slots:
    void init();
    void cleanup();
    void testViewDebtorDetails();

private:
    QMLDebtorDetailRecord *m_debtorDetailRecord;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLDebtorDetailRecordTest::QMLDebtorDetailRecordTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLDebtorDetailRecordTest::init()
{
    m_debtorDetailRecord = new QMLDebtorDetailRecord(m_thread, this);
}

void QMLDebtorDetailRecordTest::cleanup()
{
}

void QMLDebtorDetailRecordTest::testViewDebtorDetails()
{
    const QDateTime &currentDateTime(QDateTime::currentDateTime());
    auto databaseWillReturnSingleDebtor = [this, currentDateTime]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
        const QVariantMap debtor {
            {
                "debtor", QVariantMap {
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
                    { "user", QStringLiteral("user")}
                }
            }
        };
        m_result.setOutcome(debtor);
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
    const auto addresses = QStringList { QStringLiteral("Address1"), QStringLiteral("Address2") };
    const auto emails = QStringList { QStringLiteral("Email1"), QStringLiteral("Email2") };

    m_debtorDetailRecord->componentComplete();

    QCOMPARE(m_debtorDetailRecord->debtorId(), -1);
    QCOMPARE(m_debtorDetailRecord->preferredName(), QString());
    QCOMPARE(m_debtorDetailRecord->firstName(), QString());
    QCOMPARE(m_debtorDetailRecord->lastName(), QString());
    QCOMPARE(m_debtorDetailRecord->phoneNumber(), QString());
    QCOMPARE(m_debtorDetailRecord->addressModel(), QStringList());
    QCOMPARE(m_debtorDetailRecord->emailModel(), QStringList());
    QCOMPARE(m_debtorDetailRecord->created(), QDateTime());
    QCOMPARE(m_debtorDetailRecord->lastEdited(), QDateTime());
    QCOMPARE(m_debtorDetailRecord->userId(), -1);
    QCOMPARE(m_debtorDetailRecord->user(), QString());

    databaseWillReturnSingleDebtor();

    m_debtorDetailRecord->setDebtorId(1);

    QCOMPARE(m_debtorDetailRecord->debtorId(), 1);
    QCOMPARE(m_debtorDetailRecord->preferredName(), QStringLiteral("Preferred name"));
    QCOMPARE(m_debtorDetailRecord->firstName(), QStringLiteral("First name"));
    QCOMPARE(m_debtorDetailRecord->lastName(), QStringLiteral("Last name"));
    QCOMPARE(m_debtorDetailRecord->phoneNumber(), QStringLiteral("123456789"));
    QCOMPARE(m_debtorDetailRecord->addressModel(), addresses);
    QCOMPARE(m_debtorDetailRecord->emailModel(), emails);
    QCOMPARE(m_debtorDetailRecord->created(), currentDateTime);
    QCOMPARE(m_debtorDetailRecord->lastEdited(), currentDateTime);
    QCOMPARE(m_debtorDetailRecord->userId(), 1);
    QCOMPARE(m_debtorDetailRecord->user(), QStringLiteral("user"));
}

QTEST_MAIN(QMLDebtorDetailRecordTest)

#include "tst_qmldebtordetailrecordtest.moc"
