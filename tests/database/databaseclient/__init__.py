#!venv/bin/python

#from mysql.connector import MySQLConnection, Error
from dbconfig import read_db_config
from subprocess import Popen, PIPE

import pymysql
import os
import logging

connection = None
__databaseNamePattern = "###DATABASENAME###"
__initSqlFile = "databaseclient/init.sql"
__updatedInitSqlFile = "databaseclient/init_updated.sql"
__procedureDir = "databaseclient/procedures"
__unixSocket = "/var/run/mysqld/mysqld.sock"

def __update_sql_file_database_name(old_filename, new_filename, db_config):
    global __databaseNamePattern
    f = open(old_filename, "r") 
    sql = f.read().replace(__databaseNamePattern, db_config["database"])
    g = open(new_filename, "w")
    g.write(sql)

def __execute_sql_file(filename, db_config):
    command = """mysql -u %s -p"%s" --host %s --port %s %s < %s""" % (db_config["user"], db_config["password"],
                db_config["host"], db_config["port"],
                db_config["database"], filename)
    os.system(command)
# process = Popen(['mysql', db_config["database"], '-u', db_config["user"], '-p', db_config["password"]],
#                 stdout=PIPE, stdin=PIPE)
# process.communicate('source ' + filename)[0]

def __create_tables(db_config):
    global __initSqlFile
    global __updatedInitSqlFile
    __update_sql_file_database_name(__initSqlFile, __updatedInitSqlFile, db_config)
    __execute_sql_file(__updatedInitSqlFile, db_config)

def __create_procedures(db_config):
    global __procedureDir
    for filename in os.listdir(__procedureDir):
        if filename.endswith(".sql"):
            __execute_sql_file(os.path.join(__procedureDir, filename), db_config)

def create_database():
    """ Connect to MySQL database """
    global connection
    global __unixSocket
    db_config = read_db_config()
 
    try:
        connection = pymysql.connect(host=db_config["host"], port=db_config["port"], 
                                        user=db_config["user"], password=db_config["password"], 
                                        database="mysql", unix_socket=__unixSocket)
 
        if connection.open:
            cursor = connection.cursor()
            cursor.execute("DROP DATABASE IF EXISTS " + db_config["database"])
            cursor.execute("CREATE DATABASE " + db_config["database"])
            logging.info("Creating database " + db_config["database"] + "...")
            __create_tables(db_config)
            __create_procedures(db_config)
            connection.close()
            
    except pymysql.OperationalError as error:
        logging.critical('Database creation failed: ' + str(error))
 
def connect():
    """ Connect to MySQL database """
    global connection
    global __unixSocket
    db_config = read_db_config()
 
    try:
        logging.info('Connecting to MySQL database...')
        connection = pymysql.connect(unix_socket = __unixSocket, **db_config)
 
    except pymysql.Error as error:
        logging.critical("Connection failed:" + str(error))

def disconnect():
    global connection
    connection.close()
    logging.info('Connection closed.')
 
 
if __name__ == '__main__':
    create_database()
    connect()