#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmldebtormodel.h"
#include "qmlapi/qmldebttransactionmodel.h"
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

    // Long-running tests
    void testAddDebtThenAddPaymentThenViewDebtors();
private:
    QMLDebtTransactionModel *m_debtTransactionModel;
    QMLDebtorModel *m_debtorModel;
    DatabaseClient *m_client;
};

QMLDebtorModelTest::QMLDebtorModelTest()
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLDebtorModelTest::init()
{
    m_debtTransactionModel = new QMLDebtTransactionModel(this);
    m_debtorModel = new QMLDebtorModel(this);
    m_client = new DatabaseClient;
}

void QMLDebtorModelTest::cleanup()
{
    m_debtTransactionModel->deleteLater();
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
    m_debtTransactionModel->setImageSource("image/source");
    m_debtTransactionModel->setFirstName("First name");
    m_debtTransactionModel->setLastName("Last name");
    m_debtTransactionModel->setPreferredName("Preferred name");
    m_debtTransactionModel->setPrimaryPhoneNumber("1234567890");
    m_debtTransactionModel->addDebt(1234.56, dueDateTime);
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));

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
    m_debtTransactionModel->setImageSource("image/source");
    m_debtTransactionModel->setFirstName("First name");
    m_debtTransactionModel->setLastName("Last name");
    m_debtTransactionModel->setPreferredName("Preferred name");
    m_debtTransactionModel->setPrimaryPhoneNumber("1234567890");
    m_debtTransactionModel->addDebt(1234.56, dueDateTime);
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));

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
    m_debtTransactionModel->setImageSource("image/source");
    m_debtTransactionModel->setFirstName("First name");
    m_debtTransactionModel->setLastName("Last name");
    m_debtTransactionModel->setPreferredName("Preferred name");
    m_debtTransactionModel->setPrimaryPhoneNumber("1234567890");
    m_debtTransactionModel->setNote("Note");
    m_debtTransactionModel->addDebt(1234.56, dueDateTime);
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));

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

/*
  Feature: Guess the word

  # The first example has two steps
  Scenario: Maker starts a game
    When the Maker starts a game
    Then the Maker waits for a Breaker to join

  # The second example has three steps
  Scenario: Breaker joins a game
    Given the Maker has started a game with the word "silky"
    When the Breaker joins the Maker's game
    Then the Breaker must guess a word with 5 characters
*/
void QMLDebtorModelTest::testAddDebtThenAddPaymentThenViewDebtors()
{
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    const QDateTime dueDateTime(QDateTime::currentDateTime().addDays(1));

    QVERIFY(m_client->initialize());

    // STEP: Add a new debtor and new debt.
    m_debtTransactionModel->setImageSource("image/source");
    m_debtTransactionModel->setFirstName("First name");
    m_debtTransactionModel->setLastName("Last name");
    m_debtTransactionModel->setPreferredName("Preferred name");
    m_debtTransactionModel->setPrimaryPhoneNumber("1234567890");
    m_debtTransactionModel->addDebt(1234.56, dueDateTime);
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);

    // STEP: Add payment
    m_debtTransactionModel->setDebtorId(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));
    QCOMPARE(m_debtTransactionModel->rowCount(), 1);
    m_debtTransactionModel->addPayment(0, 1000.0);
    QCOMPARE(m_debtTransactionModel->index(0).data(QMLDebtTransactionModel::CurrentBalanceRole).toDouble(), 234.56);
    QVERIFY(m_debtTransactionModel->submit());
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtTransactionModel->isBusy(); }, 2000));
    QCOMPARE(m_debtTransactionModel->rowCount(), 0);

    // STEP: View debtor.
    m_debtorModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtorModel->rowCount(), 1);
}

QTEST_MAIN(QMLDebtorModelTest)

#include "tst_qmldebtormodeltest.moc"
