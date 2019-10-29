#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockitemmodel.h"
#include "mockdatabasethread.h"

class QMLStockItemModelTest : public QObject
{
    Q_OBJECT

public:
    QMLStockItemModelTest();

private slots:
    void init();
    void cleanup();

    void testViewStockItems();
    void testRefresh();
    void testRemoveItem();
    void testUndoRemoveItem();
    void testFilterItem();
private:
    QMLStockItemModel *m_stockItemModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLStockItemModelTest::QMLStockItemModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLStockItemModelTest::init()
{
    m_stockItemModel = new QMLStockItemModel(m_thread, this);
}

void QMLStockItemModelTest::cleanup()
{
    m_stockItemModel->deleteLater();
}

void QMLStockItemModelTest::testViewStockItems()
{
    auto databaseWillReturnThreeItems = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        const QVariantMap itemInfo1 {
            { "category_id", 1 },
            { "category", "Category1" },
            { "item_id", 1 },
            { "item", "Item1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantMap itemInfo2 {
            { "category_id", 1 },
            { "category", "Category1" },
            { "item_id", 2 },
            { "item", "Item2" },
            { "description", "Description2" },
            { "quantity", 1.0 },
            { "unit_id", 2 },
            { "unit", "Unit2" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList items {
            itemInfo1,
            itemInfo2
        };

        m_result.setOutcome(QVariantMap {
                                { "items", items },
                                { "record_count", items.count() }
                            });
    };
    QSignalSpy successSpy(m_stockItemModel, &QMLStockItemModel::success);
    QSignalSpy busyChangedSpy(m_stockItemModel, &QMLStockItemModel::busyChanged);

    databaseWillReturnThreeItems();

    QCOMPARE(m_stockItemModel->rowCount(), 0);

    m_stockItemModel->setCategoryId(1);
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().toInt(), QMLStockItemModel::ViewStockItemsSuccess);
    successSpy.clear();

    QCOMPARE(m_stockItemModel->rowCount(), 2);
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(0, 0), QMLStockItemModel::CategoryRole).toString(), QStringLiteral("Category1"));
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(0, 0), QMLStockItemModel::ItemRole).toString(), QStringLiteral("Item1"));
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(0, 0), QMLStockItemModel::DescriptionRole).toString(), QStringLiteral("Description1"));
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(0, 0), QMLStockItemModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(0, 0), QMLStockItemModel::UnitRole).toString(), QStringLiteral("Unit1"));
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(0, 0), QMLStockItemModel::CostPriceRole).toDouble(), 11.0);
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(0, 0), QMLStockItemModel::RetailPriceRole).toDouble(), 10.0);

    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(1, 0), QMLStockItemModel::ItemRole).toString(), QStringLiteral("Item2"));
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(1, 0), QMLStockItemModel::DescriptionRole).toString(), QStringLiteral("Description2"));
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(1, 0), QMLStockItemModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(1, 0), QMLStockItemModel::UnitRole).toString(), QStringLiteral("Unit2"));
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(1, 0), QMLStockItemModel::CostPriceRole).toDouble(), 11.0);
    QCOMPARE(m_stockItemModel->data(m_stockItemModel->index(1, 0), QMLStockItemModel::RetailPriceRole).toDouble(), 10.0);
}

void QMLStockItemModelTest::testRefresh()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnSingleItem = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        const QVariantMap itemInfo {
            { "category_id", 1 },
            { "category", "Category1" },
            { "item_id", 1 },
            { "item", "Item1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList items { itemInfo };

        m_result.setOutcome(QVariantMap {
                                { "items", items },
                                { "record_count", items.count() }
                            });
    };

    databaseWillReturnEmptyResult();

    m_stockItemModel->setCategoryId(1);
    QCOMPARE(m_stockItemModel->rowCount(), 0);

    databaseWillReturnSingleItem();

    m_stockItemModel->refresh();
    QCOMPARE(m_stockItemModel->rowCount(), 1);
}

void QMLStockItemModelTest::testRemoveItem()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnRemovedItem = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        m_result.setOutcome(QVariantMap {
                                { "category_id", 1 },
                                { "item_id", 1 }
                            });
    };
    auto databaseWillReturnSingleItem = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        const QVariantMap itemInfo {
            { "category_id", 1 },
            { "category", "Category1" },
            { "item_id", 1 },
            { "item", "Item1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList items { itemInfo };

        m_result.setOutcome(QVariantMap {
                                { "items", items },
                                { "record_count", items.count() }
                            });
    };
    QSignalSpy successSpy(m_stockItemModel, &QMLStockItemModel::success);
    QSignalSpy errorSpy(m_stockItemModel, &QMLStockItemModel::error);

    databaseWillReturnSingleItem();

    m_stockItemModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockItemModel->rowCount(), 1);

    databaseWillReturnRemovedItem();

    m_stockItemModel->removeItem(0);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_stockItemModel->rowCount(), 0);
    QCOMPARE(successSpy.takeFirst().first().value<QMLStockItemModel::SuccessCode>(), QMLStockItemModel::RemoveItemSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturnEmptyResult();

    m_stockItemModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockItemModel->rowCount(), 0);
}

void QMLStockItemModelTest::testUndoRemoveItem()
{
    auto databaseWillReturnRemovedItem = [this]() {
        m_result.setSuccessful(true);

        m_result.setOutcome(QVariantMap {
                                { "category_id", 1 },
                                { "item_id", 1 }
                            });
    };
    auto databaseWillReturnSingleItem = [this]() {
        m_result.setSuccessful(true);

        const QVariantMap itemInfo {
            { "category_id", 1 },
            { "category", "Category1" },
            { "item_id", 1 },
            { "item", "Item1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList items { itemInfo };

        m_result.setOutcome(QVariantMap {
                                { "items", items },
                                { "record_count", items.count() }
                            });
    };
    QSignalSpy successSpy(m_stockItemModel, &QMLStockItemModel::success);

    databaseWillReturnSingleItem();

    m_stockItemModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 1);

    databaseWillReturnRemovedItem();

    m_stockItemModel->removeItem(0);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 0);

    databaseWillReturnSingleItem();

    m_stockItemModel->undoLastCommit();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 0);

    databaseWillReturnSingleItem();

    m_stockItemModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 1);
}

void QMLStockItemModelTest::testFilterItem()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnSingleItem = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());

        const QVariantMap itemInfo {
            { "category_id", 1 },
            { "category", "Category1" },
            { "item_id", 1 },
            { "item", "Item1" },
            { "description", "Description1" },
            { "quantity", 1.0 },
            { "unit_id", 1 },
            { "unit", "Unit1" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList items { itemInfo };

        m_result.setOutcome(QVariantMap {
                                { "items", items },
                                { "record_count", items.count() }
                            });
    };
    QSignalSpy successSpy(m_stockItemModel, &QMLStockItemModel::success);

    databaseWillReturnSingleItem();

    m_stockItemModel->setCategoryId(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockItemModel->setFilterColumn(QMLStockItemModel::ItemColumn);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    m_stockItemModel->setFilterText("Item1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockItemModel->setFilterText("Item");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockItemModel->setFilterText("Item2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockItemModel->rowCount(), 0);
}

QTEST_MAIN(QMLStockItemModelTest)

#include "tst_qmlstockitemmodeltest.moc"
