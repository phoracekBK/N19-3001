import sqlite3


class DbRecord:
    def __init__(self, record):
        self.__record = record

    def row_count(self):
        print("count: "+self.arraysize())
        return self.arraysize()

    def get_ceel(self, row, column):
        return self.__record[row][column]

