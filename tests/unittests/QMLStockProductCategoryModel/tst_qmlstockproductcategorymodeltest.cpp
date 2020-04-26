#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlstockproductcategorymodel.h"
#include "mockdatabasethread.h"

class QMLStockProductCategoryModelTest : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testModel();
    void testViewStockCategories();
    void testFilterCategory();
    void testFilterProduct();

private:
    QMLStockProductCategoryModel *m_stockProductCategoryModel;
    MockDatabaseThread *m_thread;
};

void QMLStockProductCategoryModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_stockProductCategoryModel = new QMLStockProductCategoryModel(*m_thread, this);
}

void QMLStockProductCategoryModelTest::cleanup()
{
    m_stockProductCategoryModel->deleteLater();
    m_thread->deleteLater();
}

void QMLStockProductCategoryModelTest::testModel()
{
    QAbstractItemModelTester(m_stockProductCategoryModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLStockProductCategoryModelTest::testViewStockCategories()
{
    const QVariantList categories {
        QVariantMap {
            { "category_id", 1 },
            { "category", QStringLiteral("Category") }
        }
    };

    auto databaseWillReturn = [this](const QVariantList &categories) {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
        m_thread->result().setOutcome(QVariantMap { { "categories", categories } });
    };
    QSignalSpy successSpy(m_stockProductCategoryModel, &QMLStockProductCategoryModel::success);
    QSignalSpy errorSpy(m_stockProductCategoryModel, &QMLStockProductCategoryModel::error);

    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 0);

    databaseWillReturn(categories);

    m_stockProductCategoryModel->componentComplete();

    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);
    QCOMPARE(categories.isEmpty(), false);
    QCOMPARE(m_stockProductCategoryModel->index(0).data(QMLStockProductCategoryModel::ProductCategoryIdRole).toInt(),
             categories.first().toMap()["product_category_id"].toInt());
    QCOMPARE(m_stockProductCategoryModel->index(0).data(QMLStockProductCategoryModel::CategoryRole).toString(),
             categories.first().toMap()["product_category"].toString());
}

void QMLStockProductCategoryModelTest::testFilterCategory()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
    };
    auto databaseWillReturnSingleCategory = [this]() {
        m_thread->result().setSuccessful(true);
        const QVariantMap categoryInfo {
            { "category_id", 1 },
            { "category", QStringLiteral("Category1") }
        };

        const QVariantList categories { categoryInfo };

        m_thread->result().setOutcome(QVariantMap {
                                { "categories", categories },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockProductCategoryModel, &QMLStockProductCategoryModel::success);

    databaseWillReturnSingleCategory();

    m_stockProductCategoryModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);

    m_stockProductCategoryModel->setFilterText("Category1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);

    databaseWillReturnSingleCategory();

    m_stockProductCategoryModel->setFilterText("Category");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockProductCategoryModel->setFilterText("Category2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 0);

}

void QMLStockProductCategoryModelTest::testFilterProduct()
{
    auto databaseWillReturnEmptyResult = [this]() {
        m_thread->result().setSuccessful(true);
        m_thread->result().setOutcome(QVariant());
    };
    auto databaseWillReturnSingleProduct = [this]() {
        m_thread->result().setSuccessful(true);
        const QVariantMap categoryInfo {
            { "category_id", 1 },
            { "category", QStringLiteral("Category") }
        };

        const QVariantList categories {
            categoryInfo
        };

        m_thread->result().setOutcome(QVariantMap {
                                { "categories", categories },
                                { "record_count", 1 }
                            });
    };
    QSignalSpy successSpy(m_stockProductCategoryModel, &QMLStockProductCategoryModel::success);

    databaseWillReturnSingleProduct();

    m_stockProductCategoryModel->componentComplete();
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);

    m_stockProductCategoryModel->setProductFilterText("Category1");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);

    databaseWillReturnSingleProduct();

    m_stockProductCategoryModel->setFilterText("Category");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);

    databaseWillReturnEmptyResult();

    m_stockProductCategoryModel->setFilterText("Category2");
    QCOMPARE(successSpy.count(), 1);
    successSpy.clear();
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 0);
}

QTEST_MAIN(QMLStockProductCategoryModelTest)

#include "tst_qmlstockproductcategorymodeltest.moc"
