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

QTEST_MAIN(QMLStockCategoryModelTest)

#include "tst_qmlstockcategorymodeltest.moc"
