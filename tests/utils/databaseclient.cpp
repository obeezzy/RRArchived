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
const QString CONNECTION_NAME("database_client");

const int USER_ID(1);
const QString RR_USERNAME("test");

DatabaseClient::DatabaseClient()
{
    qputenv("TEST_GUI_ENABLED", "1");
}

DatabaseClient::~DatabaseClient()
{
    try {
        if (m_connection.isOpen()) {
            closeAllOtherConnections();
            DatabaseCreator(m_connection).dropDatabase(DATABASENAME);
            disconnectFromTestDatabase();
        }
    } catch (DatabaseException &e) {
        qCritical() << "DatabaseException caught in DatabaseClient:" << e.message() << e.userMessage();
    }
}

bool DatabaseClient::initialize()
{
    return connectToTestDatabase() && createTables() && createUser();
}

bool DatabaseClient::connectToTestDatabase()
{
    try {
        if (!QSqlDatabase::contains(CONNECTION_NAME))
            m_connection = QSqlDatabase::addDatabase("QMYSQL", CONNECTION_NAME);
        else
            m_connection = QSqlDatabase::database(CONNECTION_NAME);

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

        if (!DatabaseUtils::connectToDatabase(USERNAME, PASSWORD, DATABASENAME, CONNECTION_NAME))
            return false;

        return true;
    } catch (DatabaseException &e) {
        qCritical() << "Exception caught:" << e.message() << e.userMessage();
    }

    return false;
}

bool DatabaseClient::disconnectFromTestDatabase()
{
    if (m_connection.isValid() && m_connection.isOpen()) {
        m_connection.close();
        m_connection.setDatabaseName("mysql");
    } else {
        return false;
    }

    return true;
}

bool DatabaseClient::createTables()
{
    try {
        DatabaseCreator(m_connection).createTables();
        return true;
    } catch (DatabaseException &e) {
        qCritical() << "DatabaseException caught in DatabaseClient:" << e.message() << e.userMessage();
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

void DatabaseClient::closeAllOtherConnections()
{
    for (const QString &connectionName : QSqlDatabase::connectionNames()) {
        if (m_connection.connectionName() != connectionName)
            QSqlDatabase::database(connectionName).close();
    }
}

bool DatabaseClient::createUser()
{
    try {
        DatabaseCreator(m_connection).createRRUser("test");
        return true;
    } catch (DatabaseException &e) {
        qCritical() << "DatabaseException caught in DatabaseClient:" << e.message();
    }

    return false;
}
