#include "databaseclient.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QDateTime>

#include "database/databasecreator.h"
#include "database/databaseexception.h"
#include "database/databaseutils.h"

const QString USERNAME("root");
const QString PASSWORD("hello");
const int PORT(3306);
const QString DATABASENAME("rr_test");
const QString HOSTNAME("localhost");

const int USER_ID(1);
const QString RR_USERNAME("test");

DatabaseClient::DatabaseClient()
{
}

DatabaseClient::~DatabaseClient()
{
    disconnectFromTestDatabase();
}

bool DatabaseClient::initialize()
{
    return connectToTestDatabase() && createTables() && createUser();
}

bool DatabaseClient::connectToTestDatabase()
{
    try {
        if (!QSqlDatabase::contains())
            m_connection = QSqlDatabase::addDatabase("QMYSQL");
        else
            m_connection = QSqlDatabase::database();

        // Disconnect and connect to 'mysql'
        if (m_connection.isOpen())
            m_connection.close();

        m_connection.setDatabaseName("mysql");
        m_connection.setHostName(HOSTNAME);
        m_connection.setPort(PORT);
        m_connection.setUserName(USERNAME);
        m_connection.setPassword(PASSWORD);
        m_connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

        if (!m_connection.open())
            return false;

        DatabaseCreator creator(m_connection);
        creator.dropDatabase(DATABASENAME);
        creator.createDatabase(DATABASENAME);

        // Disconnect and connect to 'rr_test'
        if (m_connection.isOpen())
            m_connection.close();

        m_connection.setDatabaseName(DATABASENAME);
        m_connection.setHostName(HOSTNAME);
        m_connection.setPort(PORT);
        m_connection.setUserName(USERNAME);
        m_connection.setPassword(PASSWORD);
        m_connection.setConnectOptions("MYSQL_OPT_RECONNECT = 1");

        if (!m_connection.open())
            return false;
        if (!DatabaseUtils::connectToDatabase(USERNAME, PASSWORD, DATABASENAME))
            return false;

        creator.createTables();

        return true;
    } catch (DatabaseException &e) {
        qDebug() << "Exception caught:" << e.message();
    }

    return false;
}

bool DatabaseClient::disconnectFromTestDatabase()
{
    if (m_connection.isValid() && m_connection.isOpen())
        m_connection.close();
    else
        return false;

    return true;
}

bool DatabaseClient::createTables()
{
    try {
        DatabaseCreator(m_connection).createTables();

        return true;
    } catch (DatabaseException &e) {
        qDebug() << "Exception caught:" << e.message();
    }

    return false;
}

bool DatabaseClient::executeSql(const QString &sql)
{
    QSqlQuery q(m_connection);
    if (!q.exec(sql))
        return false;

    return true;
}

int DatabaseClient::userId() const
{
    return USER_ID;
}

QString DatabaseClient::userName() const
{
    return RR_USERNAME;
}

QSqlDatabase DatabaseClient::connection()
{
    return m_connection;
}

bool DatabaseClient::createUser()
{
    try {
        DatabaseCreator(m_connection).createRRUser("test");

        return true;
    } catch (DatabaseException &e) {
        qDebug() << "Exception caught:" << e.message();
    }

    return false;
}

bool DatabaseClient::addDummyItems()
{
//    QDateTime currentDateTime = QDateTime::currentDateTime();

//    QSqlQuery q(m_connection);
//    q.prepare("INSERT INTO category (category, created, last_edited, user_id) "
//              "VALUES(:category, :created, :last_edited, :user_id)");
//    q.bindValue(":category", "Beans");
//    q.bindValue(":created", currentDateTime);
//    q.bindValue(":last_edited", currentDateTime);
//    q.bindValue("user_id", USER_ID);

//    if (!q.exec()) {
//        qDebug() << "Failed to add dummy item.";
//        return false;
//    }

//    QSqlQuery q(m_connection);
//    q.prepare("INSERT INTO category (category, created, last_edited, user_id) "
//              "VALUES(:category, :created, :last_edited, :user_id)");
//    q.bindValue(":category", "Noodles");
//    q.bindValue(":created", currentDateTime);
//    q.bindValue(":last_edited", currentDateTime);
//    q.bindValue("user_id", USER_ID);

//    if (!q.exec()) {
//        qDebug() << "Failed to add dummy item.";
//        return false;
//    }

//    QSqlQuery q(m_connection);
//    q.prepare("INSERT INTO category (category, created, last_edited, user_id) "
//              "VALUES(:category, :created, :last_edited, :user_id)");
//    q.bindValue(":category", "Oil");
//    q.bindValue(":created", currentDateTime);
//    q.bindValue(":last_edited", currentDateTime);
//    q.bindValue("user_id", USER_ID);

//    if (!q.exec()) {
//        qDebug() << "Failed to add dummy item.";
//        return false;
//    }

//    QSqlQuery q(m_connection);
//    q.prepare("INSERT INTO category (category, created, last_edited, user_id) "
//              "VALUES(:category, :created, :last_edited, :user_id)");
//    q.bindValue(":category", "Rice");
//    q.bindValue(":created", currentDateTime);
//    q.bindValue(":last_edited", currentDateTime);
//    q.bindValue("user_id", USER_ID);

//    if (!q.exec()) {
//        qDebug() << "Failed to add dummy item.";
//        return false;
//    }

//    QSqlQuery q(m_connection);
//    q.prepare("INSERT INTO category (category, created, last_edited, user_id) "
//              "VALUES(:category, :created, :last_edited, :user_id)");
//    q.bindValue(":category", "Tomato Paste");
//    q.bindValue(":created", currentDateTime);
//    q.bindValue(":last_edited", currentDateTime);
//    q.bindValue("user_id", USER_ID);

//    if (!q.exec()) {
//        qDebug() << "Failed to add dummy item.";
//        return false;
//    }

    return true;
}
