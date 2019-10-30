#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlsaletransactionitemmodel.h"
#include "mockdatabasethread.h"

class QMLSaleTransactionItemModelTest : public QObject
{
    Q_OBJECT

public:
    QMLSaleTransactionItemModelTest();
    ~QMLSaleTransactionItemModelTest();

private slots:
    void init();
    void cleanup();

    void testSetTransactionId();

private:
    QMLSaleTransactionItemModel *m_saleTransactionItemModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLSaleTransactionItemModelTest::QMLSaleTransactionItemModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

QMLSaleTransactionItemModelTest::~QMLSaleTransactionItemModelTest()
{

}

void QMLSaleTransactionItemModelTest::init()
{
    m_saleTransactionItemModel = new QMLSaleTransactionItemModel(m_thread);
}

void QMLSaleTransactionItemModelTest::cleanup()
{
    m_saleTransactionItemModel->deleteLater();
}

void QMLSaleTransactionItemModelTest::testSetTransactionId()
{
    const QDateTime &currentDateTime(QDateTime::currentDateTime());
    auto databaseWillReturnSingleTransactionItem = [this, &currentDateTime]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
        const QVariantList items {
            QVariantMap {
                { "id", 1 },
                { "category_id", 1 },
                { "category", QStringLiteral("Category") },
                { "item_id", 1 },
                { "item", QStringLiteral("Item") },
                { "unit_price", 1.10 },
                { "quantity", 8.5 },
                { "unit_id", 1 },
                { "unit", QStringLiteral("Unit") },
                { "cost", 50.85 },
                { "discount", 50.55 },
                { "note_id", 1 },
                { "note", QStringLiteral("Note") },
                { "suspended", false },
                { "archived", false },
                { "created", currentDateTime },
                { "last_edited", currentDateTime },
                { "user_id", 1 },
                { "user", QStringLiteral("user") }
            }
        };
        m_result.setOutcome(QVariantMap { { "items", items } });
    };

    QSignalSpy transactionIdChangedSpy(m_saleTransactionItemModel, &QMLSaleTransactionItemModel::transactionIdChanged);
    QSignalSpy successSpy(m_saleTransactionItemModel, &QMLSaleTransactionItemModel::success);
    QSignalSpy errorSpy(m_saleTransactionItemModel, &QMLSaleTransactionItemModel::error);

    m_saleTransactionItemModel->componentComplete();
    QCOMPARE(transactionIdChangedSpy.count(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);

    QCOMPARE(m_saleTransactionItemModel->transactionId(), -1);
    QCOMPARE(m_saleTransactionItemModel->rowCount(), 0);

    databaseWillReturnSingleTransactionItem();

    m_saleTransactionItemModel->setTransactionId(1);
    QCOMPARE(transactionIdChangedSpy.count(), 1);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<QMLSaleTransactionItemModel::SuccessCode>(),
             QMLSaleTransactionItemModel::ViewSaleTransactionItemsSuccess);
    QCOMPARE(m_saleTransactionItemModel->rowCount(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::TransactionItemIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CategoryIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CategoryRole).toString(), QStringLiteral("Category"));
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::ItemIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::ItemRole).toString(), QStringLiteral("Item"));
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UnitPriceRole).toDouble(), 1.10);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::QuantityRole).toDouble(), 8.5);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UnitIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UnitRole).toString(), QStringLiteral("Unit"));
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CostRole).toDouble(), 50.85);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::DiscountRole).toDouble(), 50.55);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::NoteIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::NoteRole).toString(), QStringLiteral("Note"));
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::SuspendedRole).toBool(), false);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::ArchivedRole).toBool(), false);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CreatedRole).toDateTime(), currentDateTime);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::LastEditedRole).toDateTime(), currentDateTime);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UserIdRole).toInt(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UserRole).toString(), QStringLiteral("user"));
}

QTEST_MAIN(QMLSaleTransactionItemModelTest)

#include "tst_qmlsaletransactionitemmodeltest.moc"
