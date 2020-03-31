#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlcreditormodel.h"
#include "mockdatabasethread.h"

class QMLCreditorModelTest : public QObject
{
    Q_OBJECT
public:
    QMLCreditorModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLCreditorModel *m_creditorModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLCreditorModelTest::QMLCreditorModelTest() :
    m_thread(&m_result)
{
    m_creditorModel = new QMLCreditorModel(m_thread, this);
}

void QMLCreditorModelTest::init()
{

}

void QMLCreditorModelTest::cleanup()
{

}

void QMLCreditorModelTest::test_case1()
{

}

QTEST_MAIN(QMLCreditorModelTest)

#include "tst_qmlcreditormodeltest.moc"
