#include "mockdatabasethread.h"
#include "qmlapi/income/qmlincometransactionmodel.h"
#include <QtTest>
#include <QCoreApplication>

class QMLIncomeTransactionModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testViewIncomeTransactions();
    void testError();
private:
    QMLIncomeTransactionModel *m_incomeTransactionModel;
    MockDatabaseThread *m_thread;
};

void QMLIncomeTransactionModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_incomeTransactionModel = new QMLIncomeTransactionModel(*m_thread, this);
}

void QMLIncomeTransactionModelTest::cleanup()
{
    m_incomeTransactionModel->deleteLater();
    m_thread->deleteLater();
}

void QMLIncomeTransactionModelTest::testViewIncomeTransactions()
{
    const QVariantList transactions {
        QVariantMap {
            { "client_id", 1 },
            { "preferred_name", QStringLiteral("Ama Mosieri") },
            { "purpose", QStringLiteral("Dodger tickets") },
            { "amount", 420.00 },
            { "payment_method", "cash" }
        },
        QVariantMap {
            { "client_id", 2 },
            { "preferred_name", QStringLiteral("Bob Martin") },
            { "purpose", QStringLiteral("Clean code and TDD!") },
            { "amount", 420.00 },
            { "payment_method", "cash" }
        }
    };

    auto databaseWillReturn = [this](const QVariantList &transactions) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap { { "transactions", transactions } });
    };
    QSignalSpy successSpy(m_incomeTransactionModel, &QMLIncomeTransactionModel::success);
    QSignalSpy errorSpy(m_incomeTransactionModel, &QMLIncomeTransactionModel::error);
    QSignalSpy busyChangedSpy(m_incomeTransactionModel, &QMLIncomeTransactionModel::busyChanged);

    QCOMPARE(m_incomeTransactionModel->rowCount(), 0);

    databaseWillReturn(transactions);

    m_incomeTransactionModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(busyChangedSpy.count(), 2);

    QCOMPARE(m_incomeTransactionModel->rowCount(), 2);
    QCOMPARE(m_incomeTransactionModel->data(m_incomeTransactionModel->index(0, QMLIncomeTransactionModel::TransactionIdColumn),
                                             QMLIncomeTransactionModel::TransactionIdRole).toInt(),
             transactions[0].toMap()["income_transaction_id"].toInt());
    QCOMPARE(m_incomeTransactionModel->data(m_incomeTransactionModel->index(0, 0),
                                             QMLIncomeTransactionModel::ClientIdRole).toInt(),
             transactions[0].toMap()["client_id"].toInt());
    QCOMPARE(m_incomeTransactionModel->data(m_incomeTransactionModel->index(0, QMLIncomeTransactionModel::ClientNameColumn),
                                             QMLIncomeTransactionModel::ClientNameRole).toInt(),
             transactions[0].toMap()["preferred_name"].toInt());
    QCOMPARE(m_incomeTransactionModel->data(m_incomeTransactionModel->index(0, QMLIncomeTransactionModel::AmountColumn),
                                             QMLIncomeTransactionModel::AmountRole).toInt(),
             transactions[0].toMap()["amount"].toInt());
}

void QMLIncomeTransactionModelTest::testError()
{
    auto databaseWillReturnError = [this]() {
        m_thread->result().setSuccessful(false);
    };
    QSignalSpy successSpy(m_incomeTransactionModel, &QMLIncomeTransactionModel::success);
    QSignalSpy errorSpy(m_incomeTransactionModel, &QMLIncomeTransactionModel::error);
    QSignalSpy busyChangedSpy(m_incomeTransactionModel, &QMLIncomeTransactionModel::busyChanged);

    QCOMPARE(m_incomeTransactionModel->rowCount(), 0);

    databaseWillReturnError();

    m_incomeTransactionModel->componentComplete();
    QCOMPARE(m_incomeTransactionModel->rowCount(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 1);
    QCOMPARE(busyChangedSpy.count(), 2);
}

QTEST_GUILESS_MAIN(QMLIncomeTransactionModelTest)

#include "tst_qmlincometransactionmodeltest.moc"
