#include <QtTest>
#include <QCoreApplication>

#include "qmlapi/qmlusermodel.h"
#include "mockdatabasethread.h"

class QMLUserModelTest : public QObject
{
    Q_OBJECT

public:
    QMLUserModelTest();
    ~QMLUserModelTest();
private slots:
    void init();
    void cleanup();
    void test_case1();
private:
    QMLUserModel *m_userModel;
    MockDatabaseThread m_thread;
    QueryResult m_result;
};

QMLUserModelTest::QMLUserModelTest() :
    m_thread(&m_result)
{
    m_userModel = new QMLUserModel(m_thread);
}

QMLUserModelTest::~QMLUserModelTest()
{
    m_userModel->deleteLater();
}

void QMLUserModelTest::init()
{

}

void QMLUserModelTest::cleanup()
{

}

void QMLUserModelTest::test_case1()
{

}

QTEST_MAIN(QMLUserModelTest)

#include "tst_qmlusermodeltest.moc"
