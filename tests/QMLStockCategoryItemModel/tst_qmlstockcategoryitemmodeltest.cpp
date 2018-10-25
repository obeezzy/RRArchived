#include <QString>
#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockcategoryitemmodel.h"
#include "models/stockitemmodel.h"
#include "mockdatabasethread.h"

class QMLStockCategoryItemModelTest : public QObject
{
    Q_OBJECT

public:
    QMLStockCategoryItemModelTest();

private Q_SLOTS:
    void init();
    void cleanup();

    void testViewStockItems();
    void testRefresh();
    void testRemoveItem();
    void testUndoRemoveItem();
    void testFilterCategory();
    void testFilterItem();
private:
    QMLStockCategoryItemModel *m_stockCategoryItemModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLStockCategoryItemModelTest::QMLStockCategoryItemModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLStockCategoryItemModelTest::init()
{
    m_stockCategoryItemModel = new QMLStockCategoryItemModel(m_thread);
}

void QMLStockCategoryItemModelTest::cleanup()
{
    m_stockCategoryItemModel->deleteLater();
}

void QMLStockCategoryItemModelTest::testViewStockItems()
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

        const QVariantMap itemInfo3 {
            { "category_id", 2 },
            { "category", "Category2" },
            { "item_id", 3 },
            { "item", "Item3" },
            { "description", "Description3" },
            { "quantity", 1.0 },
            { "unit_id", 3 },
            { "unit", "Unit3" },
            { "cost_price", 11.0 },
            { "retail_price", 10.0 },
            { "unit_price", 13.0 },
            { "available_quantity", 10.0 }
        };

        const QVariantList categories {
            { "Category1" },
            { "Category2" }
        };

        const QVariantList category1Items {
            itemInfo1,
            itemInfo2
        };

        const QVariantList category2Items {
            { itemInfo3 }
        };

        const QVariantList itemGroups {
            category1Items, category2Items
        };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "item_groups", itemGroups }
                            });
    };
    QSignalSpy successSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::success);
    QSignalSpy busyChangedSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::busyChanged);

    databaseWillReturnThreeItems();

    m_stockCategoryItemModel->componentComplete(); // Trigger auto-query
    QCOMPARE(busyChangedSpy.count(), 2);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(successSpy.takeFirst().first().toInt(), QMLStockCategoryItemModel::ViewItemsSuccess);
    successSpy.clear();

    QCOMPARE(m_stockCategoryItemModel->rowCount(), 2);

    QCOMPARE(m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(0), QMLStockCategoryItemModel::CategoryRole).toString(),
             QStringLiteral("Category1"));
    QCOMPARE(m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(1), QMLStockCategoryItemModel::CategoryRole).toString(),
             QStringLiteral("Category2"));

    StockItemModel *model1 = m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(0), QMLStockCategoryItemModel::ItemModelRole)
            .value<StockItemModel *>();
    QVERIFY(model1 != nullptr);
    QCOMPARE(model1->rowCount(), 2);
    QCOMPARE(model1->data(model1->index(0), StockItemModel::ItemRole).toString(), QStringLiteral("Item1"));
    QCOMPARE(model1->data(model1->index(0), StockItemModel::DescriptionRole).toString(), QStringLiteral("Description1"));
    QCOMPARE(model1->data(model1->index(0), StockItemModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(model1->data(model1->index(0), StockItemModel::UnitRole).toString(), QStringLiteral("Unit1"));
    QCOMPARE(model1->data(model1->index(0), StockItemModel::CostPriceRole).toDouble(), 11.0);
    QCOMPARE(model1->data(model1->index(0), StockItemModel::RetailPriceRole).toDouble(), 10.0);

    QCOMPARE(model1->data(model1->index(1), StockItemModel::ItemRole).toString(), QStringLiteral("Item2"));
    QCOMPARE(model1->data(model1->index(1), StockItemModel::DescriptionRole).toString(), QStringLiteral("Description2"));
    QCOMPARE(model1->data(model1->index(1), StockItemModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(model1->data(model1->index(1), StockItemModel::UnitRole).toString(), QStringLiteral("Unit2"));
    QCOMPARE(model1->data(model1->index(1), StockItemModel::CostPriceRole).toDouble(), 11.0);
    QCOMPARE(model1->data(model1->index(1), StockItemModel::RetailPriceRole).toDouble(), 10.0);

    StockItemModel *model2 = m_stockCategoryItemModel->data(m_stockCategoryItemModel->index(1), QMLStockCategoryItemModel::ItemModelRole)
            .value<StockItemModel *>();
    QVERIFY(model2 != nullptr);
    QCOMPARE(model2->rowCount(), 1);
    QCOMPARE(model2->data(model1->index(0), StockItemModel::ItemRole).toString(), QStringLiteral("Item3"));
    QCOMPARE(model2->data(model1->index(0), StockItemModel::DescriptionRole).toString(), QStringLiteral("Description3"));
    QCOMPARE(model2->data(model1->index(0), StockItemModel::QuantityRole).toDouble(), 1.0);
    QCOMPARE(model2->data(model1->index(0), StockItemModel::UnitRole).toString(), QStringLiteral("Unit3"));
    QCOMPARE(model2->data(model1->index(0), StockItemModel::CostPriceRole).toDouble(), 11.0);
    QCOMPARE(model2->data(model1->index(0), StockItemModel::RetailPriceRole).toDouble(), 10.0);
}

void QMLStockCategoryItemModelTest::testRefresh()
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

        const QVariantList categories {
            { "Category1" }
        };

        const QVariantList itemGroups {
            QVariantList {
                { itemInfo }
            }
        };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "item_groups", itemGroups },
                                { "record_count", 1 }
                            });
    };

    databaseWillReturnEmptyResult();

    m_stockCategoryItemModel->componentComplete();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->refresh();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    StockItemModel *stockItemModel = m_stockCategoryItemModel->index(0).data(QMLStockCategoryItemModel::ItemModelRole)
            .value<StockItemModel *>();
    QVERIFY(stockItemModel != nullptr);
    QCOMPARE(stockItemModel->rowCount(), 1);
}

void QMLStockCategoryItemModelTest::testRemoveItem()
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

        const QVariantList categories {
            { "Category1" }
        };

        const QVariantList itemGroups {
            QVariantList {
                { itemInfo }
            }
        };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "item_groups", itemGroups },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::success);
    QSignalSpy errorSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::error);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnRemovedItem();

    m_stockCategoryItemModel->removeItem(1);
    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);
    QCOMPARE(successSpy.takeFirst().first().value<QMLStockCategoryItemModel::SuccessCode>(), QMLStockCategoryItemModel::RemoveItemSuccess);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);

    databaseWillReturnEmptyResult();

    m_stockCategoryItemModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);
}

void QMLStockCategoryItemModelTest::testUndoRemoveItem()
{
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

        const QVariantList categories {
            { "Category1" }
        };

        const QVariantList itemGroups {
            QVariantList {
                { itemInfo }
            }
        };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "item_groups", itemGroups },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::success);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnRemovedItem();

    m_stockCategoryItemModel->removeItem(1);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->undoLastCommit();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->refresh();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);
}

void QMLStockCategoryItemModelTest::testFilterCategory()
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

        const QVariantList categories {
            { "Category1" }
        };

        const QVariantList itemGroups {
            QVariantList {
                { itemInfo }
            }
        };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "item_groups", itemGroups },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::success);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->setFilterColumn(QMLStockCategoryItemModel::CategoryColumn);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    m_stockCategoryItemModel->setFilterText("Category1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->setFilterText("Category");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockCategoryItemModel->setFilterText("Category2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);
}

void QMLStockCategoryItemModelTest::testFilterItem()
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

        const QVariantList categories {
            { "Category1" }
        };

        const QVariantList itemGroups {
            QVariantList {
                { itemInfo }
            }
        };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "item_groups", itemGroups },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockCategoryItemModel, &QMLStockCategoryItemModel::success);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->setFilterColumn(QMLStockCategoryItemModel::ItemColumn);
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    m_stockCategoryItemModel->setFilterText("Item1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockCategoryItemModel->setFilterText("Item");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockCategoryItemModel->setFilterText("Item2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryItemModel->rowCount(), 0);
}

QTEST_MAIN(QMLStockCategoryItemModelTest)

#include "tst_qmlstockcategoryitemmodeltest.moc"
