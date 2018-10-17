#!venv/bin/python

import unittest
import os
import pymysql

import sys
sys.path.append('.')
import databaseclient

# source venv/bin/activate
class ClientTest(unittest.TestCase):
    def setUp(self):
        databaseclient.create_database()
        databaseclient.connect()

    def tearDown(self):
        databaseclient.disconnect()

    def test_ViewClients(self):
        clientId = 1
        preferredName = "Preferred"
        phoneNumber = "123456789"
        cursor = databaseclient.connection.cursor(pymysql.cursors.DictCursor)
        cursor.execute("""INSERT INTO client VALUES (%s, 'First', 'Last', %s, %s, 'Address', NULL, 0, CURRENT_TIMESTAMP(), CURRENT_TIMESTAMP(), 1)""",
                        [clientId, preferredName, phoneNumber])
        databaseclient.connection.commit()
        cursor.callproc("ViewClients", [0])
        for result in cursor:
            self.assertEqual(result["client_id"], clientId)
            self.assertEqual(result["preferred_name"], preferredName)
            self.assertEqual(result["phone_number"], phoneNumber)

if __name__ == '__main__':
    unittest.main()