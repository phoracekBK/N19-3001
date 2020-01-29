import snap7
from snap7 import client
import threading

class PLC:
    __plc_client: client
    __ip_address: str
    __slot_index: int
    __rack_index: int
    __db_index: int
    __fail_counter: int

    def __init__(self, ip_address, slot_index, rack_index, db_index):
        self.__ip_address = ip_address
        self.__slot_index = slot_index
        self.__rack_index = rack_index
        self.__db_index = db_index
        self.__fail_counter = 0

        self.__plc_client = snap7.client.Client()

    def plc_connect(self):
        if not self.__plc_client.get_connected():
            try:
                self.__plc_client.connect(self.__ip_address, self.__rack_index, self.__slot_index)
            except:
                print("PLC is not reacheble")

    def plc_diconnect(self):
        if self.__plc_client.get_connected():
            return self.__plc_client.disconnect()
        return False

    def plc_read_db(self, base, db_size):
        if self.__plc_client.get_connected():
            self.__fail_counter = 0
            try:
                db = self.__plc_client.db_read(self.__db_index, base, db_size)
                return db
            except:
                print("Bad PLC DB format")
                return None
        else:
            if self.__fail_counter < 3:
                self.__fail_counter = self.__fail_counter + 1
                self.plc_connect()
                self.plc_connect()
                return self.plc_read_db(base, db_size)
            else:
                self.__fail_counter = 0
                return None

    def plc_write_db(self, db, base):
        if self.__plc_client.get_connected():
            self.__fail_counter = 0
            write = self.__plc_client.db_write(self.__db_index, base, db)
            return write
        else:
            if self.__fail_counter < 3:
                self.__fail_counter = self.__fail_counter + 1
                self.plc_diconnect()
                self.plc_connect()
                return self.plc_write_db(db, base)
            else:
                self.__fail_counter = 0
                return None



