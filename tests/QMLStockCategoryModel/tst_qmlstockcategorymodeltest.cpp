#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockcategorymodel.h"
#include "mockdatabasethread.h"

class QMLStockCategoryModelTest : public QObject
{
    Q_OBJECT

public:
    QMLStockCategoryModelTest();

private slots:
    void init();
    void cleanup();
    void testViewStockCategories();
    void testFilterCategory();
    void testFilterItem();

private:
    QMLStockCategoryModel *m_stockCategoryModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLStockCategoryModelTest::QMLStockCategoryModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

void QMLStockCategoryModelTest::init()
{
    m_stockCategoryModel = new QMLStockCategoryModel(m_thread, this);
}

void QMLStockCategoryModelTest::cleanup()
{
    m_stockCategoryModel->deleteLater();
}

void QMLStockCategoryModelTest::testViewStockCategories()
{
    auto databsaeWillReturnSingleCategory = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
        const QVariantList categories {
            QVariantMap {
                { "category_id", 1 },
                { "category", QStringLiteral("Category") }
            }
        };

        m_result.setOutcome(QVariantMap { { "categories", categories } });
    };
    QSignalSpy successSpy(m_stockCategoryModel, &QMLStockCategoryModel::success);
    QSignalSpy errorSpy(m_stockCategoryModel, &QMLStockCategoryModel::error);

    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockCategoryModel->rowCount(), 0);

    databsaeWillReturnSingleCategory();

    m_stockCategoryModel->componentComplete();

    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockCategoryModel->rowCount(), 1);
    QCOMPARE(m_stockCategoryModel->index(0).data(QMLStockCategoryModel::CategoryIdRole).toInt(), 1);
    QCOMPARE(m_stockCategoryModel->index(0).data(QMLStockCategoryModel::CategoryRole).toString(), QStringLiteral("Category"));
}

void QMLStockCategoryModelTest::testFilterCategory()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnSingleItem = [this]() {
        m_result.setSuccessful(true);
        const QVariantMap categoryInfo {
            { "category_id", 1 },
            { "category", QStringLiteral("Category1") }
        };

        const QVariantList categories { categoryInfo };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockCategoryModel, &QMLStockCategoryModel::success);

    databaseWillReturnSingleItem();

    m_stockCategoryModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 1);

    m_stockCategoryModel->setFilterText("Category1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockCategoryModel->setFilterText("Category");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockCategoryModel->setFilterText("Category2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 0);

}

void QMLStockCategoryModelTest::testFilterItem()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_result.setSuccessful(true);
        m_result.setOutcome(QVariant());
    };
    auto databaseWillReturnSingleItem = [this]() {
        m_result.setSuccessful(true);
        const QVariantMap categoryInfo {
            { "category_id", 1 },
            { "category", QStringLiteral("Category") }
        };

        const QVariantList categories {
            categoryInfo
        };

        m_result.setOutcome(QVariantMap {
                                { "categories", categories },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockCategoryModel, &QMLStockCategoryModel::success);

    databaseWillReturnSingleItem();

    m_stockCategoryModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 1);

    m_stockCategoryModel->setItemFilterText("Category1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 1);

    databaseWillReturnSingleItem();

    m_stockCategoryModel->setFilterText("Category");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockCategoryModel->setFilterText("Category2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockCategoryModel->rowCount(), 0);
}

QTEST_MAIN(QMLStockCategoryModelTest)

#include "tst_qmlstockcategorymodeltest.moc"
