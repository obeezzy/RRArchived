#include "qmlapi/qmlclientmodel.h"
#include "mockdatabasethread.h"
#include <QString>
#include <QtTest>
#include <QCoreApplication>

class QMLClientModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
    void testViewClients();
    void testFilterByPreferredName();
    void testFilterByPhoneNumber();
private:
    QMLClientModel *m_clientModel;
    MockDatabaseThread *m_thread;
};

void QMLClientModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_clientModel = new QMLClientModel(*m_thread, this);
}

void QMLClientModelTest::cleanup()
{
    m_clientModel->deleteLater();
    m_thread->deleteLater();
}

void QMLClientModelTest::testModel()
{
    QAbstractItemModelTester(m_clientModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLClientModelTest::testViewClients()
{
    auto databaseWillReturnEmptySet = [this]() {
        m_thread->result().setSuccessful(true);
    };
    auto databaseWillReturnSingleClient = [this]() {
        m_thread->result().setSuccessful(true);
        QVariantList clients;
        clients.append(QVariantMap {
                           { "client_id", 1 },
                           { "preferred_name", "Preferred" },
                           { "phone_number", "123456789" }
                       });
        m_thread->result().setOutcome(QVariantMap { { "clients", clients }, { "record_count", clients.count() } });
    };

    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);
    QSignalSpy busyChangedSpy(m_clientModel, &QMLClientModel::busyChanged);

    QCOMPARE(m_clientModel->rowCount(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturnEmptySet();
    m_clientModel->componentComplete();

    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(m_clientModel->rowCount(), 0);

    databaseWillReturnSingleClient();
    m_clientModel->componentComplete();

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
    auto databaseWillReturnEmptySet = [this]() {
        m_thread->result().setSuccessful(true);
    };
    auto databaseWillReturnSingleClient = [this]() {
        m_thread->result().setSuccessful(true);
        QVariantList clients;
        clients.append(QVariantMap {
                           { "client_id", 2 },
                           { "preferred_name", "Preferred again" },
                           { "phone_number", "987654321" }
                       });
        m_thread->result().setOutcome(QVariantMap { { "clients", clients }, { "record_count", clients.count() } });
    };
    auto databaseWillReturnTwoClients = [this]() {
        m_thread->result().setSuccessful(true);
        QVariantList clients;
        clients.append(QVariantMap {
                           { "client_id", 1 },
                           { "preferred_name", "Preferred" },
                           { "phone_number", "123456789" }
                       });

        clients.append(QVariantMap {
                           { "client_id", 2 },
                           { "preferred_name", "Preferred again" },
                           { "phone_number", "987654321" }
                       });
        m_thread->result().setOutcome(QVariantMap { { "clients", clients }, { "record_count", clients.count() } });
    };

    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);

    databaseWillReturnEmptySet();

    m_clientModel->setFilterColumn(QMLClientModel::PreferredNameColumn);
    m_clientModel->setFilterText(QStringLiteral("A"));
    QCOMPARE(successSpy.count(), 2);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 0);

    databaseWillReturnTwoClients();

    m_clientModel->setFilterText(QStringLiteral("P"));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 2);

    databaseWillReturnSingleClient();

    m_clientModel->setFilterText(QStringLiteral("Preferred again"));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(), 2);
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(), QStringLiteral("Preferred again"));
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(), QStringLiteral("987654321"));
}

void QMLClientModelTest::testFilterByPhoneNumber()
{
    auto databaseWillReturnEmptySet = [this]() {
        m_thread->result().setSuccessful(true);
    };
    auto databaseWillReturnSingleClient = [this]() {
        m_thread->result().setSuccessful(true);
        QVariantList clients;
        clients.append(QVariantMap {
                           { "client_id", 2 },
                           { "preferred_name", "Preferred again" },
                           { "phone_number", "987654321" }
                       });
        m_thread->result().setOutcome(QVariantMap { { "clients", clients }, { "record_count", clients.count() } });
    };
    auto databaseWillReturnTwoClients = [this]() {
        m_thread->result().setSuccessful(true);
        QVariantList clients;
        clients.append(QVariantMap {
                           { "client_id", 1 },
                           { "preferred_name", "Preferred" },
                           { "phone_number", "123456789" }
                       });

        clients.append(QVariantMap {
                           { "client_id", 2 },
                           { "preferred_name", "Preferred again" },
                           { "phone_number", "987654321" }
                       });
        m_thread->result().setOutcome(QVariantMap { { "clients", clients }, { "record_count", clients.count() } });
    };

    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);

    databaseWillReturnEmptySet();

    m_clientModel->setFilterColumn(QMLClientModel::PhoneNumberColumn);
    m_clientModel->setFilterText(QStringLiteral("0"));
    QCOMPARE(successSpy.count(), 2);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 0);

    databaseWillReturnTwoClients();

    m_clientModel->setFilterText("1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 2);

    databaseWillReturnSingleClient();

    m_clientModel->setFilterText(QStringLiteral("987654321"));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(), 2);
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(), QStringLiteral("Preferred again"));
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(), QStringLiteral("987654321"));
}

QTEST_MAIN(QMLClientModelTest)

#include "tst_qmlclientmodeltest.moc"
