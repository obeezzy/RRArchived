#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmldebtormodel.h"
#include "qmlapi/qmldebtorpusher.h"
#include "databaseclient.h"

class QMLDebtorModelTest : public QObject
{
    Q_OBJECT

public:
    QMLDebtorModelTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testViewDebtors();
    void testRemoveDebtor();
    void testUndoRemoveDebtor();

private:
    QMLDebtorPusher *m_debtorPusher;
    QMLDebtorModel *m_debtorModel;
    DatabaseClient *m_client;
};

QMLDebtorModelTest::QMLDebtorModelTest()
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLDebtorModelTest::init()
{
    m_debtorPusher = new QMLDebtorPusher(this);
    m_debtorModel = new QMLDebtorModel(this);
    m_client = new DatabaseClient;
}

void QMLDebtorModelTest::cleanup()
{
    m_debtorPusher->deleteLater();
    m_debtorModel->deleteLater();
    delete m_client;
}

void QMLDebtorModelTest::testViewDebtors()
{
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);
    const QDateTime dueDateTime(QDateTime::currentDateTime().addDays(1));

    QVERIFY(m_client->initialize());

    // STEP: Add a debtor to the database.
    m_debtorPusher->setImageSource("image/source");
    m_debtorPusher->setFirstName("First name");
    m_debtorPusher->setLastName("Last name");
    m_debtorPusher->setPreferredName("Preferred name");
    m_debtorPusher->setPhoneNumber("1234567890");
    m_debtorPusher->setAddress("1234 Address Street");
    m_debtorPusher->setNote("Note");
    m_debtorPusher->addDebt(1234.56, dueDateTime);
    m_debtorPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorPusher->isBusy(); }, 2000));

    // STEP: Instantiate model in QML and ensure that debtors are fetched from the database.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtorModel->rowCount(), 1);
}

void QMLDebtorModelTest::testRemoveDebtor()
{
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);
    const QDateTime dueDateTime(QDateTime::currentDateTime().addDays(1));

    QVERIFY(m_client->initialize());

    // STEP: Add a debtor to the database.
    m_debtorPusher->setImageSource("image/source");
    m_debtorPusher->setFirstName("First name");
    m_debtorPusher->setLastName("Last name");
    m_debtorPusher->setPreferredName("Preferred name");
    m_debtorPusher->setPhoneNumber("1234567890");
    m_debtorPusher->setAddress("1234 Address Street");
    m_debtorPusher->setNote("Note");
    m_debtorPusher->addDebt(1234.56, dueDateTime);
    m_debtorPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorPusher->isBusy(); }, 2000));

    // STEP: Instantiate model in QML and ensure that debtors are fetched from the database.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtorModel->rowCount(), 1);

    // STEP: Remove a debtor from the database.
    m_debtorModel->removeDebtor(1);
    QCOMPARE(errorSpy.count(), 0);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::RemoveDebtorSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 0);

    // STEP: Ensure debtor is removed even after model is re-populated.
    m_debtorModel->refresh();
    QCOMPARE(errorSpy.count(), 0);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    QCOMPARE(m_debtorModel->rowCount(), 0);
}

void QMLDebtorModelTest::testUndoRemoveDebtor()
{
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);
    const QDateTime dueDateTime(QDateTime::currentDateTime().addDays(1));

    QVERIFY(m_client->initialize());

    // STEP: Add a new debtor.
    m_debtorPusher->setImageSource("image/source");
    m_debtorPusher->setFirstName("First name");
    m_debtorPusher->setLastName("Last name");
    m_debtorPusher->setPreferredName("Preferred name");
    m_debtorPusher->setPhoneNumber("1234567890");
    m_debtorPusher->setAddress("1234 Address Street");
    m_debtorPusher->setNote("Note");
    m_debtorPusher->addDebt(1234.56, dueDateTime);
    m_debtorPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorPusher->isBusy(); }, 2000));

    // STEP: Instantiate model in QML and check if debtors are fetched.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 1);

    // STEP: Remove a debtor.
    m_debtorModel->removeDebtor(1);
    QCOMPARE(errorSpy.count(), 0);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::RemoveDebtorSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 0);

    // Undo the last removal.
    m_debtorModel->undoLastCommit();
    QCOMPARE(errorSpy.count(), 0);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::UndoRemoveDebtorSuccess);
    QCOMPARE(m_debtorModel->rowCount(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::ClientIdRole).toInt(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::DebtorIdRole).toInt(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::PreferredNameRole).toString(), "Preferred name");
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::NoteRole).toString(), "Note");
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::TotalDebtRole).toDouble(), 1234.56);
}

QTEST_MAIN(QMLDebtorModelTest)

#include "tst_qmldebtormodeltest.moc"
