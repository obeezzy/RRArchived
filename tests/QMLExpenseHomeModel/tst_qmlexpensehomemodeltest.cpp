#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlexpensehomemodel.h"
#include "mockdatabasethread.h"

class QMLExpenseHomeModelTest : public QObject
{
    Q_OBJECT

public:
    QMLExpenseHomeModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLExpenseHomeModel *m_expenseHomeModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLExpenseHomeModelTest::QMLExpenseHomeModelTest() :
    m_thread(&m_result)
{
    m_expenseHomeModel = new QMLExpenseHomeModel(m_thread, this);
}

void QMLExpenseHomeModelTest::init()
{

}

void QMLExpenseHomeModelTest::cleanup()
{

}

void QMLExpenseHomeModelTest::test_case1()
{

}

QTEST_MAIN(QMLExpenseHomeModelTest)

#include "tst_qmlexpensehomemodeltest.moc"
