#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlpurchasecartmodel.h"
#include "mockdatabasethread.h"

class QMLPurchaseCartModelTest : public QObject
{
    Q_OBJECT

public:
    QMLPurchaseCartModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLPurchaseCartModel *m_purchaseCartModel;
    QueryResult m_result;
    MockDatabaseThread m_thread;
};

QMLPurchaseCartModelTest::QMLPurchaseCartModelTest() :
    m_thread(&m_result)
{
    m_purchaseCartModel = new QMLPurchaseCartModel(m_thread, this);
}

void QMLPurchaseCartModelTest::init()
{

}

void QMLPurchaseCartModelTest::cleanup()
{

}

void QMLPurchaseCartModelTest::test_case1()
{

}

QTEST_MAIN(QMLPurchaseCartModelTest)

#include "tst_qmlpurchasecartmodeltest.moc"
