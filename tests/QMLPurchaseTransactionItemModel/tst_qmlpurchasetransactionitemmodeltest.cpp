#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlpurchasetransactionitemmodel.h"
#include "mockdatabasethread.h"

class QMLPurchaseTransactionItemModelTest : public QObject
{
    Q_OBJECT

public:
    QMLPurchaseTransactionItemModelTest();
    ~QMLPurchaseTransactionItemModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLPurchaseTransactionItemModel *m_purchaseTransactionItemModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLPurchaseTransactionItemModelTest::QMLPurchaseTransactionItemModelTest() :
    m_thread(&m_result)
{
    m_purchaseTransactionItemModel = new QMLPurchaseTransactionItemModel(m_thread);
}

QMLPurchaseTransactionItemModelTest::~QMLPurchaseTransactionItemModelTest()
{
    m_purchaseTransactionItemModel->deleteLater();
}

void QMLPurchaseTransactionItemModelTest::init()
{

}

void QMLPurchaseTransactionItemModelTest::cleanup()
{

}

void QMLPurchaseTransactionItemModelTest::test_case1()
{

}

QTEST_MAIN(QMLPurchaseTransactionItemModelTest)

#include "tst_qmlpurchasetransactionitemmodeltest.moc"
