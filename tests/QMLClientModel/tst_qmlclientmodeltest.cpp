#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QSqlQuery>

#include "qmlapi/qmlclientmodel.h"
#include "databaseclient.h"

class QMLClientModelTest : public QObject
{
    Q_OBJECT
public:
    QMLClientModelTest();
    ~QMLClientModelTest();
private slots:
    void init();
    void cleanup();

    // Long-running tests
    void testViewClients();
    void testFilterByPreferredName();
    void testFilterByPhoneNumber();
private:
    QMLClientModel *m_clientModel;
    DatabaseClient *m_client;
};

QMLClientModelTest::QMLClientModelTest()
{
    //QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

QMLClientModelTest::~QMLClientModelTest()
{

}

void QMLClientModelTest::init()
{
    m_clientModel = new QMLClientModel(this);
    m_client = new DatabaseClient;
}

void QMLClientModelTest::cleanup()
{
    m_clientModel->deleteLater();
    delete m_client;
}

void QMLClientModelTest::testViewClients()
{
    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);
    QSignalSpy busyChangedSpy(m_clientModel, &QMLClientModel::busyChanged);

    QVERIFY(m_client->initialize());

    QCOMPARE(m_clientModel->rowCount(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);
    m_clientModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(m_clientModel->rowCount(), 0);

    // BUG: DON'T KNOW WHY THIS HAS TO BE HERE BUT IT DOESN'T WORK WITHOUT IT!
    m_client->connection().close();
    m_client->connection().open();

    QSqlQuery q(m_client->connection());
    q.prepare("INSERT INTO client VALUES (1, 'First', 'Last', 'Preferred', "
              "'123456789', 'Address', NULL, 0, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), 1)");
    QVERIFY(q.exec());

    m_clientModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(), 1);
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(), QStringLiteral("Preferred"));
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(), QStringLiteral("123456789"));
}

void QMLClientModelTest::testFilterByPreferredName()
{
    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);

    QVERIFY(m_client->initialize());

    // BUG: DON'T KNOW WHY THIS HAS TO BE HERE BUT IT DOESN'T WORK WITHOUT IT!
    m_client->connection().close();
    m_client->connection().open();

    QSqlQuery q(m_client->connection());
    q.prepare("INSERT INTO client VALUES (1, 'First', 'Last', 'Preferred', "
              "'123456789', 'Address', NULL, 0, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), 1)");
    QVERIFY(q.exec());
    q.prepare("INSERT INTO client VALUES (2, 'First again', 'Last again', 'Preferred again', "
              "'987654321', 'Address again', NULL, 0, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), 1)");
    QVERIFY(q.exec());

    m_clientModel->setFilterColumn(QMLClientModel::PreferredNameColumn);
    m_clientModel->setFilterText("A");
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 2);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 0);

    m_clientModel->setFilterText("P");
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 2);

    m_clientModel->setFilterText("Preferred again");
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(), 1);
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(), QStringLiteral("Preferred again"));
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(), QStringLiteral("987654321"));
}

void QMLClientModelTest::testFilterByPhoneNumber()
{
    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);

    QVERIFY(m_client->initialize());

    QSqlQuery q(m_client->connection());
    q.prepare("INSERT INTO client VALUES (1, 'First', 'Last', 'Preferred', "
              "'123456789', 'Address', NULL, 0, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), 1)");
    QVERIFY(q.exec());
    q.prepare("INSERT INTO client VALUES (2, 'First again', 'Last again', 'Preferred again', "
              "'987654321', 'Address again', NULL, 0, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), 1)");
    QVERIFY(q.exec());

    m_clientModel->setFilterColumn(QMLClientModel::PhoneNumberColumn);
    m_clientModel->setFilterText("0");
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 2);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 0);

    m_clientModel->setFilterText("1");
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 2);

    m_clientModel->setFilterText("987654321");
    QVERIFY(QTest::qWaitFor([&]() { return !m_clientModel->isBusy(); }, 2000));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(), 1);
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(), QStringLiteral("Preferred again"));
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(), QStringLiteral("987654321"));
}

QTEST_MAIN(QMLClientModelTest)

#include "tst_qmlclientmodeltest.moc"
