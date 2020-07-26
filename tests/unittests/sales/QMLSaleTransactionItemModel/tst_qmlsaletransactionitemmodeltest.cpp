#include "qmlapi/sales/qmlsaletransactionitemmodel.h"
#include "mockdatabasethread.h"
#include <QtTest>
#include <QCoreApplication>

class QMLSaleTransactionItemModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
    void testSetTransactionId();
private:
    QMLSaleTransactionItemModel *m_saleTransactionItemModel;
    MockDatabaseThread *m_thread;
};

void QMLSaleTransactionItemModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_saleTransactionItemModel = new QMLSaleTransactionItemModel(*m_thread, this);
}

void QMLSaleTransactionItemModelTest::cleanup()
{
    m_saleTransactionItemModel->deleteLater();
    m_thread->deleteLater();
}

void QMLSaleTransactionItemModelTest::testModel()
{
    QAbstractItemModelTester(m_saleTransactionItemModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLSaleTransactionItemModelTest::testSetTransactionId()
{
    const auto &currentDateTime = QDateTime::currentDateTime();
    const QVariantList products {
        QVariantMap {
            { "id", 1 },
            { "product_category_id", 1 },
            { "product_category", QStringLiteral("Category") },
            { "product_id", 1 },
            { "product", QStringLiteral("Product") },
            { "unit_price", 1.10 },
            { "quantity", 8.5 },
            { "product_unit_id", 1 },
            { "product_unit", QStringLiteral("Unit") },
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
    auto databaseWillReturn = [this](const QVariantList &products) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariantMap { { "products", products } });
    };

    QSignalSpy transactionIdChangedSpy(m_saleTransactionItemModel, &QMLSaleTransactionItemModel::transactionIdChanged);
    QSignalSpy successSpy(m_saleTransactionItemModel, &QMLSaleTransactionItemModel::success);
    QSignalSpy errorSpy(m_saleTransactionItemModel, &QMLSaleTransactionItemModel::error);

    m_saleTransactionItemModel->componentComplete();
    QCOMPARE(transactionIdChangedSpy.count(), 0);
    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);

    QCOMPARE(m_saleTransactionItemModel->transactionId(), 0);
    QCOMPARE(m_saleTransactionItemModel->rowCount(), 0);

    databaseWillReturn(products);

    m_saleTransactionItemModel->setTransactionId(1);
    QCOMPARE(transactionIdChangedSpy.count(), 1);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().value<ModelResult>().code(), 0);
    QCOMPARE(products.isEmpty(), false);
    QCOMPARE(m_saleTransactionItemModel->rowCount(), 1);
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CategoryIdRole).toInt(),
             products.first().toMap()["product_category_id"].toInt());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CategoryRole).toString(),
             products.first().toMap()["product_category"].toString());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::ProductIdRole).toInt(),
             products.first().toMap()["product_id"].toInt());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::ProductRole).toString(),
             products.first().toMap()["product"].toString());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UnitPriceRole).toDouble(),
             products.first().toMap()["unit_price"].toDouble());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::QuantityRole).toDouble(),
             products.first().toMap()["quantity"].toDouble());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UnitIdRole).toInt(),
             products.first().toMap()["product_unit_id"].toInt());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UnitRole).toString(),
             products.first().toMap()["product_unit"].toString());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CostRole).toDouble(),
             products.first().toMap()["cost"].toDouble());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::DiscountRole).toDouble(),
             products.first().toMap()["discount"].toDouble());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::NoteIdRole).toInt(),
             products.first().toMap()["note_id"].toInt());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::NoteRole).toString(),
             products.first().toMap()["note"].toString());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::SuspendedRole).toBool(),
             products.first().toMap()["suspended"].toBool());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::ArchivedRole).toBool(),
             products.first().toMap()["archived"].toBool());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::CreatedRole).toDateTime(),
             products.first().toMap()["created"].toDateTime());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::LastEditedRole).toDateTime(),
             products.first().toMap()["last_edited"].toDateTime());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UserIdRole).toInt(),
             products.first().toMap()["user_id"].toInt());
    QCOMPARE(m_saleTransactionItemModel->index(0, 0).data(QMLSaleTransactionItemModel::UserRole).toString(),
             products.first().toMap()["user"].toString());
}

QTEST_GUILESS_MAIN(QMLSaleTransactionItemModelTest)

#include "tst_qmlsaletransactionitemmodeltest.moc"
