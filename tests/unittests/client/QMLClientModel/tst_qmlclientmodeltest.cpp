#include "qmlapi/client/qmlclientmodel.h"
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
    const QVariantMap client {
        { "client_id", 1 },
        { "preferred_name", "Preferred" },
        { "phone_number", "123456789" }
    };
    auto databaseWillReturn = [this](const QVariantList &clients) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap {
                                          { "clients", clients },
                                          { "record_count", 1 }
                                      });
    };

    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);
    QSignalSpy busyChangedSpy(m_clientModel, &QMLClientModel::busyChanged);

    QCOMPARE(m_clientModel->rowCount(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturn({ });
    m_clientModel->componentComplete();

    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(m_clientModel->rowCount(), 0);

    databaseWillReturn({ client });
    m_clientModel->componentComplete();

    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(),
             client["client_id"].toInt());
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(),
             client["preferred_name"].toString());
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(),
             client["phone_number"].toString());
}

void QMLClientModelTest::testFilterByPreferredName()
{
    QVariantMap client1 {
        { "client_id", 1 },
        { "preferred_name", "Preferred" },
        { "phone_number", "123456789" }
    };
    QVariantMap client2 {
        { "client_id", 2 },
        { "preferred_name", "Preferred again" },
        { "phone_number", "987654321" }
    };
    auto databaseWillReturn = [this](const QVariantList &clients) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap {
                                          { "clients", clients },
                                          { "record_count", clients.count() }
                                      });
    };

    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);

    databaseWillReturn({ });

    m_clientModel->setFilterColumn(QMLClientModel::PreferredNameColumn);
    m_clientModel->setFilterText(QStringLiteral("A"));
    QCOMPARE(successSpy.count(), 2);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 0);

    databaseWillReturn({ client1, client2 });

    m_clientModel->setFilterText(QStringLiteral("P"));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 2);

    databaseWillReturn({ client2 });

    m_clientModel->setFilterText(QStringLiteral("Preferred again"));
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(),
             client2["client_id"].toInt());
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(),
             client2["preferred_name"].toString());
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(),
             client2["phone_number"].toString());
}

void QMLClientModelTest::testFilterByPhoneNumber()
{
    QVariantMap client1 {
        { "client_id", 1 },
        { "preferred_name", "Preferred" },
        { "phone_number", "123456789" }
    };
    QVariantMap client2 {
        { "client_id", 2 },
        { "preferred_name", "Preferred again" },
        { "phone_number", "987654321" }
    };
    auto databaseWillReturn = [this](const QVariantList &clients) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap {
                                          { "clients", clients },
                                          { "record_count", clients.count() }
                                      });
    };

    QSignalSpy successSpy(m_clientModel, &QMLClientModel::success);
    QSignalSpy errorSpy(m_clientModel, &QMLClientModel::error);

    databaseWillReturn({ });

    m_clientModel->setFilterColumn(QMLClientModel::PhoneNumberColumn);
    m_clientModel->setFilterText(QStringLiteral("0"));
    QCOMPARE(successSpy.count(), 2);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 0);

    databaseWillReturn({ client1, client2 });

    m_clientModel->setFilterText("1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 2);

    databaseWillReturn({ client2 });

    m_clientModel->setFilterText(client2["phone_number"].toString());
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_clientModel->rowCount(), 1);

    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::ClientIdRole).toInt(),
             client2["client_id"].toInt());
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PreferredNameRole).toString(),
             client2["preferred_name"].toString());
    QCOMPARE(m_clientModel->index(0).data(QMLClientModel::PhoneNumberRole).toString(),
             client2["phone_number"].toString());
}

QTEST_GUILESS_MAIN(QMLClientModelTest)

#include "tst_qmlclientmodeltest.moc"
