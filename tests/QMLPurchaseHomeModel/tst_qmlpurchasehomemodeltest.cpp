#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlpurchasehomemodel.h"
#include "mockdatabasethread.h"

class QMLPurchaseHomeModelTest : public QObject
{
    Q_OBJECT

public:
    QMLPurchaseHomeModelTest();
    ~QMLPurchaseHomeModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLPurchaseHomeModel *m_purchaseHomeModel;
    QueryResult m_result;
    MockDatabaseThread m_thread;
};

QMLPurchaseHomeModelTest::QMLPurchaseHomeModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

QMLPurchaseHomeModelTest::~QMLPurchaseHomeModelTest()
{

}

void QMLPurchaseHomeModelTest::init()
{
    m_purchaseHomeModel = new QMLPurchaseHomeModel(m_thread);
}

void QMLPurchaseHomeModelTest::cleanup()
{
    m_purchaseHomeModel->deleteLater();
}

void QMLPurchaseHomeModelTest::test_case1()
{

}

QTEST_MAIN(QMLPurchaseHomeModelTest)

#include "tst_qmlpurchasehomemodeltest.moc"
