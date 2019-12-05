#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmldebtormodel.h"
#include "mockdatabasethread.h"

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
    QMLDebtorModel *m_debtorModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLDebtorModelTest::QMLDebtorModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLDebtorModelTest::init()
{
    m_debtorModel = new QMLDebtorModel(m_thread, this);
}

void QMLDebtorModelTest::cleanup()
{
    m_debtorModel->deleteLater();
}

void QMLDebtorModelTest::testViewDebtors()
{
    auto databaseWillReturnSingleDebtor = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

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

        m_result.setOutcome(QVariantMap {
                                { "debtors", debtors },
                                { "record_count", debtors.count() }
                            });
    };
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);

    databaseWillReturnSingleDebtor();

    // STEP: Instantiate model in QML and ensure that debtors are fetched from the database.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtorModel->rowCount(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::DebtorIdRole).toInt(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::ClientIdRole).toInt(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::ImageUrlRole).toString(), QStringLiteral("image/source"));
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::PreferredNameRole).toString(), QStringLiteral("Preferred name"));
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::TotalDebtRole).toDouble(), 1234.56);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::NoteRole).toString(), QStringLiteral("Note"));
}

void QMLDebtorModelTest::testRemoveDebtor()
{
    auto databaseWillReturnSingleDebtor = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

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

        m_result.setOutcome(QVariantMap {
                                { "debtors", debtors },
                                { "record_count", debtors.count() }
                            });
    };
    auto databaseWillReturnRemovedDebtor = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariantMap {
                                { "debtor_id", 1 },
                                { "debtor_row", 1 }
                            });
    };
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);

    // STEP: Add a debtor to the database.
    databaseWillReturnSingleDebtor();

    // STEP: Instantiate model in QML and ensure that debtors are fetched from the database.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_debtorModel->rowCount(), 1);

    databaseWillReturnRemovedDebtor();

    // STEP: Remove a debtor from the database.
    m_debtorModel->removeDebtor(1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::RemoveDebtorSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 0);

    databaseWillReturnEmptyResult();

    // STEP: Ensure debtor is removed even after model is re-populated.
    m_debtorModel->refresh();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    QCOMPARE(m_debtorModel->rowCount(), 0);
}

void QMLDebtorModelTest::testUndoRemoveDebtor()
{
    auto databaseWillReturnSingleDebtor = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

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

        m_result.setOutcome(QVariantMap {
                                { "debtors", debtors },
                                { "record_count", debtors.count() }
                            });
    };
    auto databaseWillReturnRemovedDebtor = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariantMap {
                                { "debtor_id", 1 },
                                { "debtor_row", 1 }
                            });
    };
    auto databaseWillReturnUndoRemovedDebtor = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        QVariantMap debtor {
            { "client_id", 1 },
            { "debtor_id", 1 },
            { "image_url", QStringLiteral("image/source") },
            { "preferred_name", QStringLiteral("Preferred name") },
            { "total_debt", 1234.56 },
            { "note", QStringLiteral("Note") }
        };

        m_result.setOutcome(QVariantMap {
                                { "debtor", debtor },
                                { "debtor_row", 1 },
                                { "debtor_id", 1 },
                                { "record_count", 1 }
                            });
    };

    QSignalSpy successSpy(m_debtorModel, &QMLDebtorModel::success);
    QSignalSpy errorSpy(m_debtorModel, &QMLDebtorModel::error);
    QSignalSpy busyChangedSpy(m_debtorModel, &QMLDebtorModel::busyChanged);

    databaseWillReturnSingleDebtor();

    // STEP: Instantiate model in QML and check if debtors are fetched.
    m_debtorModel->componentComplete();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::ViewDebtorsSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 1);

    databaseWillReturnRemovedDebtor();

    // STEP: Remove a debtor.
    m_debtorModel->removeDebtor(1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    busyChangedSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::RemoveDebtorSuccess);
    successSpy.clear();
    QCOMPARE(m_debtorModel->rowCount(), 0);

    databaseWillReturnUndoRemovedDebtor();

    // Undo the last removal.
    m_debtorModel->undoLastCommit();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLDebtorModel::SuccessCode>(), QMLDebtorModel::UndoRemoveDebtorSuccess);
    QCOMPARE(m_debtorModel->rowCount(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::ClientIdRole).toInt(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::DebtorIdRole).toInt(), 1);
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::PreferredNameRole).toString(), QStringLiteral("Preferred name"));
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::NoteRole).toString(), QStringLiteral("Note"));
    QCOMPARE(m_debtorModel->index(0).data(QMLDebtorModel::TotalDebtRole).toDouble(), 1234.56);
}

QTEST_MAIN(QMLDebtorModelTest)

#include "tst_qmldebtormodeltest.moc"
