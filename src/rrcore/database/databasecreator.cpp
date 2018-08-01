#include "databasecreator.h"
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlDriver>
#include <QRegularExpression>
#include <QDebug>
#include <QDateTime>

#include "database/databaseexception.h"
#include "database/databaseutils.h"

DatabaseCreator::DatabaseCreator(QSqlDatabase connection) :
    m_connection(connection)
{

}

void DatabaseCreator::runSqlOnTestDatabase(const QString &fileName)
{
    if (fileName.trimmed().isEmpty())
        return;

    QFile file(fileName);
    if (QFileInfo(fileName).suffix() != "sql")
        throw DatabaseException(DatabaseException::RRErrorCode::RunSqlOnTestDatabaseFailed, QString(),
                                QString("File '%1' is not a sql file").arg(fileName));
    if (QFileInfo(fileName).size() > 1024 * 50)
        throw DatabaseException(DatabaseException::RRErrorCode::RunSqlOnTestDatabaseFailed, QString(),
                                QString("File '%1' is too large (larger than 50MB).").arg(fileName));
    if (!file.open(QFile::ReadOnly))
        throw DatabaseException(DatabaseException::RRErrorCode::RunSqlOnTestDatabaseFailed, QString(),
                                QString("Failed to open '%1'").arg(fileName));

    QSqlDatabase connection;

    if (!QSqlDatabase::contains())
        connection = QSqlDatabase::addDatabase("QMYSQL");
    else
        connection = QSqlDatabase::database();

    QSqlQuery q(connection);
    QString sqlData = file.readAll();
    //Check if SQL Driver supports Transactions
    if(connection.driver()->hasFeature(QSqlDriver::Transactions)) {
        //Replace comments and tabs and new lines with space
        sqlData = sqlData.replace(QRegularExpression("(\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                     QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        //Remove waste spaces
        sqlData = sqlData.trimmed();

        //Extracting queries
        QStringList qList = sqlData.split(';', QString::SkipEmptyParts);

        //Initialize regular expression for detecting special queries (`begin transaction` and `commit`).
        //NOTE: I used new regular expression for Qt5 as recommended by Qt documentation.
        QRegularExpression re_transaction("\\bbegin.transaction.*", QRegularExpression::CaseInsensitiveOption);
        QRegularExpression re_commit("\\bcommit.*", QRegularExpression::CaseInsensitiveOption);

        //Check if query file is already wrapped with a transaction
        bool isStartedWithTransaction = re_transaction.match(qList.at(0)).hasMatch();
        if(!isStartedWithTransaction)
          connection.transaction();     //<=== not wrapped with a transaction, so we wrap it with a transaction.

        //Execute each individual queries
        foreach(const QString &s, qList) {
          if(re_transaction.match(s).hasMatch())    //<== detecting special query
              connection.transaction();
          else if(re_commit.match(s).hasMatch())    //<== detecting special query
              connection.commit();
          else {
              q.exec(s);                        //<== execute normal query
              if(q.lastError().type() != QSqlError::NoError) {
                  qInfo() << q.lastError().text();
                  connection.rollback();                    //<== rollback the transaction if there is any problem
              }
          }
        }
        if(!isStartedWithTransaction)
          connection.commit();          //<== ... completing of wrapping with transaction

    //Sql Driver doesn't supports transaction
    } else {
        //...so we need to remove special queries (`begin transaction` and `commit`)
        sqlData = sqlData.replace(QRegularExpression("(\\bbegin.transaction.*;|\\bcommit.*;|\\/\\*(.|\\n)*?\\*\\/|^--.*\\n|\\t|\\n)",
                                                     QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption), " ");
        sqlData = sqlData.trimmed();

        //Execute each individual queries
        QStringList qList = sqlData.split(';', QString::SkipEmptyParts);
        foreach(const QString &s, qList) {
          q.exec(s);
          if(q.lastError().type() != QSqlError::NoError) qInfo() << q.lastError().text();
        }
    }
}

void DatabaseCreator::createDatabase(const QString &databaseName)
{
    if (databaseName.isEmpty())
        return;

    QSqlQuery q(m_connection);
    if (!q.exec(QString("CREATE DATABASE IF NOT EXISTS %1").arg(databaseName)))
        throw DatabaseException(DatabaseException::RRErrorCode::ConnectToTestDatabaseFailed, q.lastError().text(),
                                QString("Failed to create database %1.").arg(databaseName));
}

// NOTE: You can't drop a database if there are "other" connections to it!
void DatabaseCreator::dropDatabase(const QString &databaseName)
{
    if (databaseName.isEmpty())
        return;

    QSqlQuery q(m_connection);
    if (!q.exec(QString("DROP DATABASE IF EXISTS %1").arg(databaseName)))
        throw DatabaseException(DatabaseException::RRErrorCode::ConnectToTestDatabaseFailed, q.lastError().text(),
                                QString("Failed to drop database %1.").arg(databaseName));
}

void DatabaseCreator::createTables()
{
    QSqlQuery q(m_connection);

    try {
        if (!DatabaseUtils::beginTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to start transation."));

        // Create business details table
        q.prepare("CREATE TABLE IF NOT EXISTS business_details ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "address VARCHAR(200) NOT NULL, "
                  "type VARCHAR(50) DEFAULT NULL, "
                  "establishment_year INT(11) NOT NULL, "
                  "phone_number VARCHAR(20) DEFAULT NULL, "
                  "logo BLOB, "
                  "extra_details VARCHAR(100) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create business details table."));

        // Create category table
        q.prepare("CREATE TABLE IF NOT EXISTS category ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "category VARCHAR(100) NOT NULL, "
                  "short_form VARCHAR(25) DEFAULT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id), "
                  "UNIQUE KEY category (category)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create category table."));

        // Create client table
        q.prepare("CREATE TABLE IF NOT EXISTS client ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "first_name VARCHAR(100) DEFAULT NULL, "
                  "last_name VARCHAR(100) DEFAULT NULL, "
                  "preferred_name VARCHAR(100) NOT NULL, "
                  "phone_number VARCHAR(20) NOT NULL, "
                  "address VARCHAR(100) DEFAULT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL DEFAULT '0', "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id), "
                  "UNIQUE KEY phone_number (phone_number)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create client table."));

        // Create credit payment table
        // TODO: Remove due_date
        q.prepare("CREATE TABLE IF NOT EXISTS credit_payment ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "credit_transaction_id INT(11) NOT NULL, "
                  "total_debt DECIMAL(19,2) NOT NULL, "
                  "amount_paid DECIMAL(19,2) NOT NULL, "
                  "balance DECIMAL(19,2) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "due_date DATETIME NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create credit payment table."));

        // Create creditor table
        // TODO: Add due_date
        q.prepare("CREATE TABLE IF NOT EXISTS creditor ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "client_id INT(11) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create creditor table."));

        // Create credit transaction table
        q.prepare("CREATE TABLE IF NOT EXISTS credit_transaction ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "creditor_id INT(11) NOT NULL, "
                  "transaction_table VARCHAR(20) NOT NULL, "
                  "transaction_id INT(11) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create credit transaction table."));

        // Create current quantity table
        q.prepare("CREATE TABLE IF NOT EXISTS current_quantity ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "item_id INT(11) DEFAULT NULL, "
                  "quantity DOUBLE NOT NULL, "
                  "unit_id INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id), "
                  "UNIQUE KEY item_id (item_id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create current quantity table."));

        // Create customer table
        q.prepare("CREATE TABLE IF NOT EXISTS customer ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "client_id INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id), "
                  "UNIQUE KEY client_id (client_id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create customer table."));

        // Create damaged quantity table
        q.prepare("CREATE TABLE IF NOT EXISTS damaged_quantity ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "item_id INT(11) DEFAULT NULL, "
                  "quantity DOUBLE NOT NULL, "
                  "unit_id INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id),"
                  "UNIQUE KEY item_id (item_id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create damaged quantity table."));

        // Create db info table
        q.prepare("CREATE TABLE IF NOT EXISTS db_info ("
                  "version VARCHAR(20) NOT NULL, "
                  "rack_id VARCHAR(40) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "PRIMARY KEY (version)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create db info table."));

        // Create debt payment table
        // TODO: Remove due_date
        q.prepare("CREATE TABLE IF NOT EXISTS debt_payment ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "debt_transaction_id INT(11) NOT NULL, "
                  "total_amount DECIMAL(19,2) NOT NULL, "
                  "amount_paid DECIMAL(19,2) NOT NULL, "
                  "balance DECIMAL(19,2) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "due_date DATETIME NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create debt payment table."));

        // Create debt transaction table
        // TODO: Add due_date
        q.prepare("CREATE TABLE IF NOT EXISTS debt_transaction ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "debtor_id INT(11) NOT NULL, "
                  "transaction_table VARCHAR(20) NOT NULL, "
                  "transaction_id INT(11) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create debt transaction table."));

        // Create debtor table
        q.prepare("CREATE TABLE IF NOT EXISTS debtor ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "client_id INT(11) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create debtor table."));

        // Create expense table
        q.prepare("CREATE TABLE IF NOT EXISTS expense ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "name VARCHAR(30) DEFAULT NULL, "
                  "client_id INT(11) DEFAULT '0', "
                  "purpose VARCHAR(100) NOT NULL, "
                  "total_amount DECIMAL(19,2) NOT NULL, "
                  "amount_paid DECIMAL(19,2) NOT NULL, "
                  "balance DECIMAL(19,2) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL DEFAULT '0', "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create expense table."));


        // Create expense payment table
        q.prepare("CREATE TABLE IF NOT EXISTS expense_payment ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "expense_id INT(11) NOT NULL, "
                  "amount INT(11) NOT NULL, "
                  "method VARCHAR(20) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create expense payment table."));

        // Create expense purpose table
        q.prepare("CREATE TABLE IF NOT EXISTS expense_purpose ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "purpose VARCHAR(100) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create expense purpose table."));

        // Create income table
        q.prepare("CREATE TABLE IF NOT EXISTS income ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "name VARCHAR(30) DEFAULT NULL, "
                  "client_id INT(11) DEFAULT '0', "
                  "purpose VARCHAR(100) NOT NULL, "
                  "total_amount DECIMAL(19,2) NOT NULL, "
                  "amount_paid DECIMAL(19,2) NOT NULL, "
                  "balance DECIMAL(19,2) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL DEFAULT '0',"
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create income table."));


        // Create income payment table
        q.prepare("CREATE TABLE IF NOT EXISTS income_payment ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "income_id INT(11) NOT NULL, "
                  "amount INT(11) NOT NULL, "
                  "method VARCHAR(20) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create income payment table."));

        // Create income purpose table
        q.prepare("CREATE TABLE IF NOT EXISTS income_purpose ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "purpose VARCHAR(100) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QString("Failed to create income purpose table."));

        // Create initial quantity table
        q.prepare("CREATE TABLE IF NOT EXISTS initial_quantity ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "item_id INT(11) NOT NULL, "
                  "quantity DOUBLE NOT NULL, "
                  "unit_id INT(11) NOT NULL, "
                  "reason VARCHAR(30) NOT NULL, "
                  "archived INT(11) NOT NULL DEFAULT 0, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create initial quantity table."));

        // Create item table
        q.prepare("CREATE TABLE IF NOT EXISTS item ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "category_id INT(11) NOT NULL, "
                  "item VARCHAR(200) NOT NULL, "
                  "short_form VARCHAR(10) DEFAULT NULL, "
                  "description VARCHAR(200) DEFAULT NULL, "
                  "barcode VARCHAR(70) DEFAULT NULL, "
                  "divisible int(11) DEFAULT 1, "
                  "image BLOB, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id), "
                  "UNIQUE KEY item (item), "
                  "UNIQUE KEY barcode (barcode)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create item table."));

        // Create last used date/time table
        q.prepare("CREATE TABLE IF NOT EXISTS last_used_date_time ("
                  "last_date_time DATETIME NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "PRIMARY KEY (last_date_time) "
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create last used date/time table."));

        // Create note table
        q.prepare("CREATE TABLE IF NOT EXISTS note ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "note VARCHAR(200) NOT NULL, "
                  "table_name VARCHAR(30) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create note table."));

        // Create purchase item table
        q.prepare("CREATE TABLE IF NOT EXISTS purchase_item ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "purchase_transaction_id INT(11) NOT NULL, "
                  "item_id INT(11) NOT NULL, "
                  "unit_price DECIMAL(19,2) NOT NULL, "
                  "quantity DOUBLE NOT NULL, "
                  "cost DOUBLE NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL DEFAULT '0', "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create purchase item table."));

        // Create purchase payment table
        q.prepare("CREATE TABLE IF NOT EXISTS purchase_payment ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "purchase_transaction_id INT(11) NOT NULL, "
                  "amount INT(11) NOT NULL, "
                  "method VARCHAR(20) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create purchase payment table."));

        // Create purchase transaction table
        q.prepare("CREATE TABLE IF NOT EXISTS purchase_transaction ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "name VARCHAR(50) NOT NULL, "
                  "client_id INT(11) DEFAULT NULL, "
                  "total_cost DECIMAL(19,2) NOT NULL, "
                  "amount_paid DECIMAL(19,2) NOT NULL, "
                  "balance DECIMAL(19,2) NOT NULL, "
                  "discount DECIMAL(19,2) NOT NULL, "
                  "suspended INT(11) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create purchase transaction table."));

        // Create sale item table
        q.prepare("CREATE TABLE IF NOT EXISTS sale_item ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "sale_transaction_id INT(11) NOT NULL, "
                  "item_id INT(11) NOT NULL, "
                  "unit_price DECIMAL(19,2) NOT NULL, "
                  "quantity DOUBLE NOT NULL, "
                  "unit_id INT(11) NOT NULL, "
                  "cost DECIMAL(19,2) NOT NULL, "
                  "discount DECIMAL(19,2) NOT NULL DEFAULT '0.00', "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create sale item table."));

        // Create sale payment table
        q.prepare("CREATE TABLE IF NOT EXISTS sale_payment ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "sale_transaction_id INT(11) NOT NULL, "
                  "amount INT(11) NOT NULL, "
                  "method VARCHAR(20) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create sale payment table."));

        // Create sale transaction table
        q.prepare("CREATE TABLE IF NOT EXISTS sale_transaction ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "name VARCHAR(50) NOT NULL, "
                  "client_id INT(11) DEFAULT NULL, "
                  "total_cost DECIMAL(19,2) NOT NULL, "
                  "amount_paid DECIMAL(19,2) NOT NULL, "
                  "balance DECIMAL(19,2) NOT NULL, "
                  "change_due DECIMAL(19,2) DEFAULT '0.00', "
                  "discount DECIMAL(19,2) NOT NULL, "
                  "suspended INT(11) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create sale transaction table."));

        // Create unit table
        q.prepare("CREATE TABLE IF NOT EXISTS unit ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "item_id INT(11) NOT NULL, "
                  "unit VARCHAR(30) NOT NULL, "
                  "short_form VARCHAR(10) DEFAULT NULL, "
                  "base_unit_equivalent INT(11) NOT NULL, "
                  "preferred INT(11) NOT NULL DEFAULT 0, "
                  "cost_price DECIMAL(19,2) NOT NULL, "
                  "retail_price DECIMAL(19,2) NOT NULL, "
                  "currency VARCHAR(4) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL DEFAULT '0', "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create unit table."));

        // Create unit relation table
        q.prepare("CREATE TABLE IF NOT EXISTS unit_relation ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "item_id INT(11) NOT NULL, "
                  "old_unit_quantity DOUBLE NOT NULL, "
                  "old_unit_id INT(11) NOT NULL, "
                  "new_unit_quantity DOUBLE NOT NULL, "
                  "new_unit_id INT(11) NOT NULL, "
                  "note_id INT(11) NOT NULL DEFAULT '0', "
                  "archived INT(11) NOT NULL DEFAULT '0', "
                  "created DATETIME NOT NULL, "
                  "last_edted DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create unit relation table."));

        // Create user table
        q.prepare("CREATE TABLE IF NOT EXISTS user ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "user VARCHAR(60) DEFAULT NULL, "
                  "photo BLOB DEFAULT NULL, "
                  "phone_number VARCHAR(20) DEFAULT NULL, "
                  "email_address VARCHAR(30) DEFAULT NULL, "
                  "active INT(11) DEFAULT '0', "
                  "pending INT(11) DEFAULT '0', "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "PRIMARY KEY (id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create user table."));

        // Create user privilege table
        q.prepare("CREATE TABLE IF NOT EXISTS user_privilege ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "user_id INT(11) NOT NULL, "
                  "privileges VARCHAR(1000) NOT NULL, "
                  "PRIMARY KEY (id), "
                  "UNIQUE KEY user_id (user_id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create user privilege table."));

        // Create vendor table
        q.prepare("CREATE TABLE IF NOT EXISTS vendor ("
                  "id INT(11) NOT NULL AUTO_INCREMENT, "
                  "client_id INT(11) NOT NULL, "
                  "note_id INT(11) DEFAULT NULL, "
                  "archived INT(11) NOT NULL, "
                  "created DATETIME NOT NULL, "
                  "last_edited DATETIME NOT NULL, "
                  "user_id INT(11) NOT NULL, "
                  "PRIMARY KEY (id), "
                  "UNIQUE KEY client_id (client_id)"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8");

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateTableFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create vendor table."));

        if (!DatabaseUtils::commitTransaction(q))
            throw DatabaseException(DatabaseException::RRErrorCode::BeginTransactionFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to start transation."));
    } catch (DatabaseException &) {
        DatabaseUtils::rollbackTransaction(q);
        throw;
    }
}

void DatabaseCreator::createRRUser(const QString &userName, bool active, bool pending)
{
    const QDateTime currentDateTime = QDateTime::currentDateTime();
    QSqlQuery q(m_connection);

    try {
        if (userName.trimmed().isEmpty())
            throw DatabaseException(DatabaseException::RRErrorCode::InvalidArguments,
                                    QString(),
                                    QStringLiteral("User name is empty."));
        q.prepare("INSERT INTO user (user, active, pending, created, last_edited) "
                  "VALUES(:user, :active, :pending, :created, :last_edited)");
        q.bindValue(":user", userName);
        q.bindValue(":active", active);
        q.bindValue(":pending", pending);
        q.bindValue(":created", currentDateTime);
        q.bindValue(":last_edited", currentDateTime);

        if (!q.exec())
            throw DatabaseException(DatabaseException::RRErrorCode::CreateUserFailed,
                                    q.lastError().text(),
                                    QStringLiteral("Failed to create RR user."));
    } catch (DatabaseException &) {
        throw;
    }
}
