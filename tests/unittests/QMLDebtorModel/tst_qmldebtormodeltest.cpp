#include "mockdatabasethread.h"
#include "qmlapi/qmldebtormodel.h"
#include <QCoreApplication>
#include <QString>
#include <QtTest>

class QMLDebtorModelTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void init();
    void cleanup();

    void testModel();
    void testViewDebtors();
    void testRemoveDebtor();
    void testUndoRemoveDebtor();
private:
    QMLDebtorModel *m_debtorModel;
    MockDatabaseThread *m_thread;
};

void QMLDebtorModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_debtorModel = new QMLDebtorModel(*m_thread, this);
}

void QMLDebtorModelTest::cleanup()
{
    m_debtorModel->deleteLater();
    m_thread->deleteLater();
}

void QMLDebtorModelTest::testModel()
{
    QAbstractItemModelTester(m_debtorModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLDebtorModelTest::testViewDebtors()
{
    const QVariantList debtors {
        QVariantMap {
            { "client_id", 1 },
            { "debtor_id", 1 },
            { "image_url", QUrl("file:///image/source") },
            { "preferred_name", QStringLiteral("Preferred name") },
            { "total_debt", 1234.56 },
            { "note", QStringLiteral("Note") }
        }
    };

    auto databaseWillReturn = [this](const QVariantList &debtors) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap { { "debtors", debtors } });
    };

    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);

    databaseWillReturn(debtors);

    // STEP: Instantiate model in QML and ensure that debtors are fetched from the database.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtorModel::ViewDebtorsSuccess);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtorModel->rowCount(), 1);
    QCOMPARE(debtors.count(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::DebtorIdRole).toInt(),
             debtors.first().toMap()["debtor_id"].toInt());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::ClientIdRole).toInt(),
             debtors.first().toMap()["client_id"].toInt());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::ImageUrlRole).toUrl(),
             debtors.first().toMap()["image_url"].toUrl());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::PreferredNameRole).toString(),
             debtors.first().toMap()["preferred_name"].toString());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::TotalDebtRole).toDouble(),
             debtors.first().toMap()["total_debt"].toDouble());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::NoteRole).toString(),
             debtors.first().toMap()["note"].toString());
}

void QMLDebtorModelTest::testRemoveDebtor()
{
    QVariantList debtors {
        QVariantMap {
            { "client_id", 1 },
            { "debtor_id", 1 },
            { "image_url", QStringLiteral("image/source") },
            { "preferred_name", QStringLiteral("Preferred name") },
            { "total_debt", 1234.56 },
            { "note", QStringLiteral("Note") }
        }
    };

    auto databaseWillReturn = [this](const QVariantList &debtors) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap {
                                { "debtors", debtors },
                                { "record_count", debtors.count() }
                            });
    };
    auto databaseWillReturnRemovedDebtor = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap {
                                { "debtor_id", 1 },
                                { "row", 0 }
                            });
    };
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);

    // STEP: Add a debtor to the database.
    databaseWillReturn(debtors);

    // STEP: Instantiate model in QML and ensure that debtors are fetched from the database.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtorModel->rowCount(), 1);

    databaseWillReturnRemovedDebtor();

    // STEP: Remove a debtor from the database.
    m_debtorModel->removeDebtor(0);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtorModel::RemoveDebtorSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 0);

    databaseWillReturn({ });

    // STEP: Ensure debtor is removed even after model is re-populated.
    m_debtorModel->refresh();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtorModel::ViewDebtorsSuccess);
    QCOMPARE(m_debtorModel->rowCount(), 0);
}

void QMLDebtorModelTest::testUndoRemoveDebtor()
{
    const QVariantList debtors {
        QVariantMap {
            { "client_id", 1 },
            { "debtor_id", 1 },
            { "image_url", QStringLiteral("image/source") },
            { "preferred_name", QStringLiteral("Preferred name") },
            { "total_debt", 1234.56 },
            { "note", QStringLiteral("Note") },
            { "row", 0 }
        }
    };

    auto databaseWillReturn = [this](const QVariantList &debtors) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap{ { "debtors", debtors } });
    };
    auto databaseWillReturnRemovedDebtor = [this](const QVariantMap &debtor) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(debtor);
    };
    auto databaseWillReturnReaddedDebtor = [this](const QVariantMap &debtor) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(debtor);
    };

    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);

    databaseWillReturn(debtors);

    // STEP: Instantiate model in QML and check if debtors are fetched.
    m_debtorModel->componentComplete();
    QCOMPARE(debtors.isEmpty(), false);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 1);

    databaseWillReturnRemovedDebtor(debtors.first().toMap());

    // STEP: Remove a debtor.
    m_debtorModel->removeDebtor(debtors.first().toMap()["row"].toInt());
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtorModel::RemoveDebtorSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 0);

    databaseWillReturnReaddedDebtor(debtors.first().toMap());

    // Undo the last removal.
    m_debtorModel->undoLastCommit();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(),
             QMLDebtorModel::UndoRemoveDebtorSuccess);
    QCOMPARE(m_debtorModel->rowCount(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::ClientIdRole).toInt(),
             debtors.first().toMap()["client_id"].toInt());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::DebtorIdRole).toInt(),
             debtors.first().toMap()["debtor_id"].toInt());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::PreferredNameRole).toString(),
             debtors.first().toMap()["preferred_name"].toString());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::NoteRole).toString(),
             debtors.first().toMap()["note"].toString());
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::TotalDebtRole).toDouble(),
             debtors.first().toMap()["total_debt"].toDouble());
}

QTEST_GUILESS_MAIN(QMLDebtorModelTest)

#include "tst_qmldebtormodeltest.moc"
