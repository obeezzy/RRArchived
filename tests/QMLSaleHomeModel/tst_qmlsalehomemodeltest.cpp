#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlsalehomemodel.h"
#include "mockdatabasethread.h"

class QMLSaleHomeModelTest : public QObject
{
    Q_OBJECT

public:
    QMLSaleHomeModelTest();
    ~QMLSaleHomeModelTest();

private slots:
    void init();
    void cleanup();
    void test_case1();

private:
    QMLSaleHomeModel *m_saleHomeModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLSaleHomeModelTest::QMLSaleHomeModelTest() :
    m_thread(&m_result)
{
    QLoggingCategory::setFilterRules(QStringLiteral("*.info=false"));
}

QMLSaleHomeModelTest::~QMLSaleHomeModelTest()
{

}

void QMLSaleHomeModelTest::init()
{
    m_saleHomeModel = new QMLSaleHomeModel(m_thread);
}

void QMLSaleHomeModelTest::cleanup()
{
    m_saleHomeModel->deleteLater();
}

void QMLSaleHomeModelTest::test_case1()
{

}

QTEST_MAIN(QMLSaleHomeModelTest)

#include "tst_qmlsalehomemodeltest.moc"
