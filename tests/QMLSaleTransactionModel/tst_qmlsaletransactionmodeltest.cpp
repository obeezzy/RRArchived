#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlsaletransactionmodel.h"
#include "mockdatabasethread.h"

class QMLSaleTransactionModelTest : public QObject
{
    Q_OBJECT

public:
    QMLSaleTransactionModelTest();

private slots:
    void init();
    void cleanup();
    void testViewSaleTransactions();

private:
    QMLSaleTransactionModel *m_saleTransactionModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLSaleTransactionModelTest::QMLSaleTransactionModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLSaleTransactionModelTest::init()
{
    m_saleTransactionModel = new QMLSaleTransactionModel(m_thread, this);
}

void QMLSaleTransactionModelTest::cleanup()
{
}

void QMLSaleTransactionModelTest::testViewSaleTransactions()
{
    const QDateTime &currentDateTime(QDateTime::currentDateTime());
    auto databaseWillReturnSingleTransaction = [this, &currentDateTime]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
        const QVariantList transactions {
            QVariantMap {
                { "transaction_id", 1 },
                { "client_id", 1 },
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

        m_result.setOutcome(QVariantMap { { "transactions", transactions } });
    };
    QSignalSpy successSpy(m_saleTransactionModel, &QMLSaleTransactionModel::success);
    QSignalSpy errorSpy(m_saleTransactionModel, &QMLSaleTransactionModel::error);

    databaseWillReturnSingleTransaction();

    QCOMPARE(m_saleTransactionModel->rowCount(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);

    m_saleTransactionModel->componentComplete();

    QCOMPARE(m_saleTransactionModel->rowCount(), 1);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleTransactionModel::SuccessCode>(),
             QMLSaleTransactionModel::ViewTransactionSuccess);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::TransactionIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::ClientIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::CustomerNameRole).toString(), QStringLiteral("Customer name"));
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::TotalCostRole).toDouble(), 1234.56);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::AmountPaidRole).toDouble(), 1000.56);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::BalanceRole).toDouble(), 234.00);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::DiscountRole).toDouble(), 1.89);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::NoteIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::NoteRole).toString(), QStringLiteral("Note"));
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::SuspendedRole).toBool(), false);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::ArchivedRole).toBool(), false);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::CreatedRole).toDateTime(), currentDateTime);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::LastEditedRole).toDateTime(), currentDateTime);
    QCOMPARE(m_saleTransactionModel->index(0, 0).data(QMLSaleTransactionModel::UserIdRole).toInt(), 1);
}

QTEST_MAIN(QMLSaleTransactionModelTest)

#include "tst_qmlsaletransactionmodeltest.moc"
