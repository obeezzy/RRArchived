#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlpurchasetransactionmodel.h"
#include "mockdatabasethread.h"

class QMLPurchaseTransactionModelTest : public QObject
{
    Q_OBJECT

public:
    QMLPurchaseTransactionModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLPurchaseTransactionModel *m_purchaseTransactionModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLPurchaseTransactionModelTest::QMLPurchaseTransactionModelTest() :
    m_thread(&m_result)
{
    m_purchaseTransactionModel = new QMLPurchaseTransactionModel(m_thread, this);
}

void QMLPurchaseTransactionModelTest::init()
{

}

void QMLPurchaseTransactionModelTest::cleanup()
{

}

void QMLPurchaseTransactionModelTest::test_case1()
{

}

QTEST_MAIN(QMLPurchaseTransactionModelTest)

#include "tst_qmlpurchasetransactionmodeltest.moc"
