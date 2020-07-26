#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/stock/qmlproductcategorymodel.h"
#include "mockdatabasethread.h"

class QMLProductCategoryModelTest : public QObject
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
    QMLProductCategoryModel *m_stockProductCategoryModel;
    MockDatabaseThread *m_thread;
};

void QMLProductCategoryModelTest::init()
{
    m_thread = new MockDatabaseThread(this);
    m_stockProductCategoryModel = new QMLProductCategoryModel(*m_thread, this);
}

void QMLProductCategoryModelTest::cleanup()
{
    m_stockProductCategoryModel->deleteLater();
    m_thread->deleteLater();
}

void QMLProductCategoryModelTest::testModel()
{
    QAbstractItemModelTester(m_stockProductCategoryModel,
                             QAbstractItemModelTester::FailureReportingMode::Fatal,
                             this);
}

void QMLProductCategoryModelTest::testViewStockCategories()
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
    QSignalSpy successSpy(m_stockProductCategoryModel, &QMLProductCategoryModel::success);
    QSignalSpy errorSpy(m_stockProductCategoryModel, &QMLProductCategoryModel::error);

    QCOMPARE(successSpy.count(), 0);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 0);

    databaseWillReturn(categories);

    m_stockProductCategoryModel->componentComplete();

    QCOMPARE(successSpy.count(), 1);
    QCOMPARE(errorSpy.count(), 0);
    QCOMPARE(m_stockProductCategoryModel->rowCount(), 1);
    QCOMPARE(categories.isEmpty(), false);
    QCOMPARE(m_stockProductCategoryModel->index(0).data(QMLProductCategoryModel::ProductCategoryIdRole).toInt(),
             categories.first().toMap()["product_category_id"].toInt());
    QCOMPARE(m_stockProductCategoryModel->index(0).data(QMLProductCategoryModel::CategoryRole).toString(),
             categories.first().toMap()["product_category"].toString());
}

void QMLProductCategoryModelTest::testFilterCategory()
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
    QSignalSpy successSpy(m_stockProductCategoryModel, &QMLProductCategoryModel::success);

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

void QMLProductCategoryModelTest::testFilterProduct()
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
    QSignalSpy successSpy(m_stockProductCategoryModel, &QMLProductCategoryModel::success);

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

QTEST_GUILESS_MAIN(QMLProductCategoryModelTest)

#include "tst_qmlproductcategorymodeltest.moc"
