import sqlite3
from .db_record import DbRecord


class Database:
    def __init__(self, db_file_name):
        self.__db_file_name = db_file_name

    def select(self, sql_request):
        try:
            connection = sqlite3.connect(self.__db_file_name)
            print(sql_request)
            db_table = connection.execute(sql_request)
            return db_table
        except Exception as e:
            print("sql processing error: " + str(e))
            return None

    def insert(self, sql_request):
        try:
            connection = sqlite3.connect(self.__db_file_name)
            print(sql_request)
            connection.execute(sql_request)
            connection.commit()

            changes = connection.total_changes

            return changes

        except Exception as e:
            print("sql processing error: " + str(e))
            return 0
