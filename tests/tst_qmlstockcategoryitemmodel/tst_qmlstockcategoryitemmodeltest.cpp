#include <QString>
#include <QtTest>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QLoggingCategory>

#include "qmlapi/qmlstockcategoryitemmodel.h"
#include "qmlapi/qmlstockitempusher.h"
#include "models/stockitemmodel.h"
#include "../utils/databaseclient.h"

class QMLStockCategoryItemModelTest : public QObject
{
    Q_OBJECT

public:
    QMLStockCategoryItemModelTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    // Long-running tests
    void testViewStockItems();
    void testRefresh();
    void testRemoveItem();
    void testUndoRemoveItem();
    void testFilter();
private:
    QMLStockCategoryItemModel *m_stockCategoryItemModel;
    QMLStockItemPusher *m_stockItemPusher;
    DatabaseClient *m_client;
};

QMLStockCategoryItemModelTest::QMLStockCategoryItemModelTest()
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.debug=false"));
}

void QMLStockCategoryItemModelTest::initTestCase()
{
    m_stockCategoryItemModel = new QMLStockCategoryItemModel(this);
    m_stockItemPusher = new QMLStockItemPusher(this);
    m_client = new DatabaseClient;
}

void QMLStockCategoryItemModelTest::cleanupTestCase()
{
    m_stockCategoryItemModel->deleteLater();
    m_stockItemPusher->deleteLater();
    delete m_client;
}

void QMLStockCategoryItemModelTest::testViewStockItems()
{
    QSignalSpy successSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::success);
    QSignalSpy busyChangedSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::busyChanged);

    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();

    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item2");
    m_stockItemPusher->setDescription("Description2");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(4.0);
    m_stockItemPusher->setUnit("Unit2");
    m_stockItemPusher->setCostPrice(5.0);
    m_stockItemPusher->setRetailPrice(6.0);
    m_stockItemPusher->push();

    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_stockItemPusher->setCategory("Category2");
    m_stockItemPusher->setItem("Item3");
    m_stockItemPusher->setDescription("Description3");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(7.0);
    m_stockItemPusher->setUnit("Unit3");
    m_stockItemPusher->setCostPrice(8.0);
    m_stockItemPusher->setRetailPrice(9.0);
    m_stockItemPusher->push();

    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_stockCategoryItemModel->componentComplete(); // Trigger auto-query
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().toInt(), QMLStockCategoryItemModel::ItemsFetched);
    successSpy.clear();

    QCOMPARE(m_stockCategoryItemModel->rowCount(), 2);

    QCOMPARE(m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(0), QMLStockCategoryItemModel::CategoryRole).toString(), "Category1");
    QCOMPARE(m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(1), QMLStockCategoryItemModel::CategoryRole).toString(), "Category2");

    StockItemModel *model1 = m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(0), QMLStockCategoryItemModel::ItemModelRole)
            .value<StockItemModel *>();
    QVERIFY(model1 != nullptr);
    QCOMPARE(model1->rowCount(), 2);
    QCOMPARE(model1->data(model1->index(0), StockItemModel::ItemRole).toString(), "Item1");
    QCOMPARE(model1->data(model1->index(0), StockItemModel::DescriptionRole).toString(), "Description1");
    QCOMPARE(model1->data(model1->index(0), StockItemModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(model1->data(model1->index(0), StockItemModel::UnitRole).toString(), "Unit1");
    QCOMPARE(model1->data(model1->index(0), StockItemModel::CostPriceRole).toDouble(), 2.0);
    QCOMPARE(model1->data(model1->index(0), StockItemModel::RetailPriceRole).toDouble(), 3.0);

    QCOMPARE(model1->data(model1->index(1), StockItemModel::ItemRole).toString(), "Item2");
    QCOMPARE(model1->data(model1->index(1), StockItemModel::DescriptionRole).toString(), "Description2");
    QCOMPARE(model1->data(model1->index(1), StockItemModel::QuantityRole).toDouble(), 4.0);
    QCOMPARE(model1->data(model1->index(1), StockItemModel::UnitRole).toString(), "Unit2");
    QCOMPARE(model1->data(model1->index(1), StockItemModel::CostPriceRole).toDouble(), 5.0);
    QCOMPARE(model1->data(model1->index(1), StockItemModel::RetailPriceRole).toDouble(), 6.0);

    StockItemModel *model2 = m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(1), QMLStockCategoryItemModel::ItemModelRole)
            .value<StockItemModel *>();
    QVERIFY(model2 != nullptr);
    QCOMPARE(model2->rowCount(), 1);
    QCOMPARE(model2->data(model1->index(0), StockItemModel::ItemRole).toString(), "Item3");
    QCOMPARE(model2->data(model1->index(0), StockItemModel::DescriptionRole).toString(), "Description3");
    QCOMPARE(model2->data(model1->index(0), StockItemModel::QuantityRole).toDouble(), 7.0);
    QCOMPARE(model2->data(model1->index(0), StockItemModel::UnitRole).toString(), "Unit3");
    QCOMPARE(model2->data(model1->index(0), StockItemModel::CostPriceRole).toDouble(), 8.0);
    QCOMPARE(model2->data(model1->index(0), StockItemModel::RetailPriceRole).toDouble(), 9.0);
}

void QMLStockCategoryItemModelTest::testRefresh()
{
    QVERIFY(m_client->initialize());

    m_stockCategoryItemModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);
    m_stockCategoryItemModel->refresh();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);
    StockItemModel *stockItemModel = m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(0), QMLStockCategoryItemModel::ItemModelRole)
            .value<StockItemModel *>();
    QCOMPARE(stockItemModel->rowCount(), 1);
}

void QMLStockCategoryItemModelTest::testRemoveItem()
{
    QSignalSpy successSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::success);

    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_stockCategoryItemModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    successSpy.clear();
    m_stockCategoryItemModel->removeItem(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);
    QCOMPARE(successSpy.takeFirst().first().toInt(), QMLStockCategoryItemModel::ItemRemoved);

    m_stockCategoryItemModel->refresh();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);
}

void QMLStockCategoryItemModelTest::testUndoRemoveItem()
{
    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_stockCategoryItemModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    m_stockCategoryItemModel->removeItem(1);
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);

    m_stockCategoryItemModel->undoLastCommit();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);
}

void QMLStockCategoryItemModelTest::testFilter()
{
    QVERIFY(m_client->initialize());

    // Push some items
    m_stockItemPusher->setCategory("Category1");
    m_stockItemPusher->setItem("Item1");
    m_stockItemPusher->setDescription("Description1");
    m_stockItemPusher->setDivisible(true);
    m_stockItemPusher->setQuantity(1.0);
    m_stockItemPusher->setUnit("Unit1");
    m_stockItemPusher->setCostPrice(2.0);
    m_stockItemPusher->setRetailPrice(3.0);
    m_stockItemPusher->push();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockItemPusher->isBusy(); }, 2000));

    m_stockCategoryItemModel->componentComplete();
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    m_stockCategoryItemModel->setFilterColumn(QMLStockCategoryItemModel::CategoryColumn);
    m_stockCategoryItemModel->setFilterText("Category1");
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    m_stockCategoryItemModel->setFilterText("Category2");
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);

    m_stockCategoryItemModel->setFilterColumn(QMLStockCategoryItemModel::ItemColumn);
    m_stockCategoryItemModel->setFilterText("Item1");
    QVERIFY(QTest::qWaitFor([&]() { return !m_stockCategoryItemModel->isBusy(); }, 2000));
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);
}

QTEST_MAIN(QMLStockCategoryItemModelTest)

#include "tst_qmlstockcategoryitemmodeltest.moc"
