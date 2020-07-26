#include "qmlapi/sales/qmlsaletransactionmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLSaleTransactionModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
    void testViewSaleTransactions();
private:
    QMLSaleTransactionModel *m_saleTransactionModel;
    MockDatabaseThread *m_thread;
};

void QMLSaleTransactionModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_saleTransactionModel = new QMLSaleTransactionModel(*m_thread, this);
}

void QMLSaleTransactionModelTest::cleanup()
{
    m_saleTransactionModel->deleteLater();
    m_thread->deleteLater();
}

void QMLSaleTransactionModelTest::testModel()
{
    QAbstractItemModelTester(m_saleTransactionModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLSaleTransactionModelTest::testViewSaleTransactions()
{
    const QDateTime &currentDateTime(QDateTime::currentDateTime());
    const QVariantList transactions {
        QVariantMap {
            { "sale_transaction_id", 1 },
            { "customer_id", 1 },
            { "customer_name", QStringLiteral("Customer name") },
            { "total_cost", 1234.56 },
            { "amount_paid", 1000.56 },
            { "balance", 234.00 },
            { "discount", 1.89 },
            { "note_id", 1 },
            { "note", QStringLiteral("Note") },
            { "suspended", false },
            { "archived", false },
            { "created", currentDateTime },
            { "last_edited", currentDateTime },
            { "user_id", 1 }
        }
    };
    auto databaseWillReturn = [this](const QVariantList &transactions) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap { { "transactions", transactions } });
    };
    QSignalSpy successSpy(m_saleTransactionModel, &QMLSaleTransactionModel::success);
    QSignalSpy errorSpy(m_saleTransactionModel, &QMLSaleTransactionModel::error);

    databaseWillReturn(transactions);

    QCOMPARE(m_saleTransactionModel->rowCount(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);

    m_saleTransactionModel->componentComplete();

    QCOMPARE(m_saleTransactionModel->rowCount(), 1);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), QMLSaleTransactionModel::ViewTransactionSuccess);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::SaleTransactionIdRole).toInt(),
             transactions.first().toMap()["sale_transaction_id"].toInt());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::CustomerIdRole).toInt(),
             transactions.first().toMap()["customer_id"].toInt());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::CustomerNameRole).toString(),
             transactions.first().toMap()["customer_name"].toString());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::TotalCostRole).toDouble(),
             transactions.first().toMap()["total_cost"].toDouble());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::AmountPaidRole).toDouble(),
             transactions.first().toMap()["amount_paid"].toDouble());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::BalanceRole).toDouble(),
             transactions.first().toMap()["balance"].toDouble());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::DiscountRole).toDouble(),
             transactions.first().toMap()["discount"].toDouble());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::NoteIdRole).toInt(),
             transactions.first().toMap()["note_id"].toInt());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::NoteRole).toString(),
             transactions.first().toMap()["note"].toString());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::SuspendedRole).toBool(),
             transactions.first().toMap()["suspended"].toBool());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::ArchivedRole).toBool(),
             transactions.first().toMap()["archived"].toBool());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::CreatedRole).toDateTime(),
             transactions.first().toMap()["created"].toDateTime());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::LastEditedRole).toDateTime(),
             transactions.first().toMap()["last_edited"].toDateTime());
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::UserIdRole).toInt(),
             transactions.first().toMap()["user_id"].toInt());
}

QTEST_GUILESS_MAIN(QMLSaleTransactionModelTest)

#include "tst_qmlsaletransactionmodeltest.moc"
