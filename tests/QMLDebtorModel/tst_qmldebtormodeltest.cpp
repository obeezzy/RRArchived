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

    void testRemoveDebtor();
    void testUndoRemoveDebtor();

private:
    QMLDebtorPusher *m_debtorPusher;
    QMLDebtorModel *m_debtorModel;
    DatabaseClient *m_client;
};

QMLDebtorModelTest::QMLDebtorModelTest()
{
    //QLoggingCategory::setFilterRules(QStringLiteral("*.debug=false"));
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

void QMLDebtorModelTest::testRemoveDebtor()
{
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    const QDateTime dueDateTime(QDateTime::currentDateTime().addDays(1));

    QVERIFY(m_client->initialize());

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

    m_debtorModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(m_debtorModel->rowCount(), 1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();

    m_debtorModel->removeDebtor(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_debtorModel->rowCount(), 0);
    successSpy.clear();

    m_debtorModel->refresh();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_debtorModel->rowCount(), 0);
}

void QMLDebtorModelTest::testUndoRemoveDebtor()
{
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    const QDateTime dueDateTime(QDateTime::currentDateTime().addDays(1));

    QVERIFY(m_client->initialize());

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

    m_debtorModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(m_debtorModel->rowCount(), 1);

    m_debtorModel->removeDebtor(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(m_debtorModel->rowCount(), 0);
    successSpy.clear();

    m_debtorModel->undoLastCommit();
    QVERIFY(QTest::qWaitFor([&]() { return !m_debtorModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_debtorModel->rowCount(), 1);
}

QTEST_MAIN(QMLDebtorModelTest)

#include "tst_qmldebtormodeltest.moc"
