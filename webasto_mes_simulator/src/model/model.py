
from snap7 import util
from .plc import PLC
from .glass_info import GlassInfo
from .database import Database
from .db_record import DbRecord
import threading
import ctypes
import array


class Model:
    plc: PLC
    db: Database
    __lock_database: threading.Semaphore
    __lock_plc: threading.Semaphore

    def __init__(self):
        self.__lock_database = threading.Semaphore()
        self.__lock_plc = threading.Semaphore()

        self.plc = PLC("192.168.0.1", 1, 0, 6)
        self.db = Database("mes.db")

    def plc_get_flag_1(self):
        return util.get_bool(self.__plc_read_data(3, 1), 0, 0)

    def plc_set_flag_1(self, value):
        buffer_value = [value]
        self.__plc_write_data(3, bytearray(buffer_value))

    def plc_get_flag_2(self):
        return util.get_bool(self.__plc_read_data(37, 1), 0, 0)

    def plc_set_flag_2(self, value):
        buffer_value = [value]
        self.__plc_write_data(37, bytearray(buffer_value))

    def plc_get_flag_3(self):
        return util.get_bool(self.__plc_read_data(70, 1), 0, 0)

    def plc_set_flag_3(self, value):
        buffer_value = [value]
        self.__plc_write_data(70, bytearray(buffer_value))

    def plc_get_flag_4(self):
        return util.get_bool(self.__plc_read_data(104, 1), 0, 0)

    def plc_set_flag_4(self, value):
        buffer_value = [value]
        self.__plc_write_data(104, bytearray(buffer_value))

    def plc_get_glass_panel_taken_out(self):
        return util.get_bool(self.__plc_read_data(116, 1), 0, 0)

    def plc_set_glass_panel_taken_out(self, value):
        buffer_value = [value]
        self.__plc_write_data(116, bytearray(buffer_value))

    def plc_get_glass_panel_is_in_tailgate(self):
        return util.get_bool(self.__plc_read_data(117, 1), 0, 0)

    def plc_set_glass_panel_is_in_tailgate(self, value):
        buffer_value = [value]
        self.__plc_write_data(117, bytearray(buffer_value))

    def plc_get_id_of_glass_panel(self):
        return util.get_dword(self.__plc_read_data(106, 4), 0)

    def plc_set_id_of_glass_panel(self, glass_id: ctypes.c_uint32):
        buffer_glass_id = [glass_id]
        self.__plc_write_data(106, bytearray(buffer_glass_id))

    def plc_get_versionsnummer(self):
        return util.get_int(self.__plc_read_data(0, 2), 0)

    def plc_set_versionsnummer(self, glass_id: ctypes.c_uint16):
        buffer_glass_id = [glass_id]
        self.__plc_write_data(0, bytearray(buffer_glass_id))

    def plc_get_teiletyp(self):
        return util.get_string(self.__plc_read_data(2, 1), 0, 1)

    def plc_set_teiletyp(self, value: ctypes.c_uint8):
        buffer_value = [value]
        self.__plc_write_data(2, bytearray(buffer_value))

    def plc_get_modell(self):
        model = self.__plc_read_data(36, 1)
        return model[0]

    def plc_set_modell(self, model: ctypes.c_uint8):
        buffer_model = [model]
        self.__plc_write_data(36, bytearray(buffer_model))

    def plc_get_vehicle_no_1(self):
        return util.get_string(self.__plc_read_data(4, 13), 0, 13)

    def plc_set_vehicle_no_1(self, number: str):
        self.__plc_write_data(4, bytearray(number, 'utf-8'))

    def plc_get_vehicle_no_2(self):
        return util.get_string(self.__plc_read_data(38, 13), 0, 13)

    def plc_set_vehicle_no_2(self, number: str):
        self.__plc_write_data(38, bytearray(number, 'utf-8'))

    def plc_get_vehicle_no_3(self):
        return util.get_string(self.__plc_read_data(72, 13), 0, 13)

    def plc_set_vehicle_no_3(self, number: str):
        self.__plc_write_data(72, bytearray(number, 'utf-8'))

    def plc_get_rear_window_type_1(self):
        return util.get_string(self.__plc_read_data(18, 18), 0, 18)

    def plc_set_rear_window_type_1(self, window_type: str):
        self.__plc_write_data(18, bytearray(window_type, 'utf-8'))

    def plc_get_rear_window_type_2(self):
        return util.get_string(self.__plc_read_data(52, 18), 0, 18)

    def plc_set_rear_window_type_2(self, window_type: str):
        self.__plc_write_data(52, bytearray(window_type,'utf-8'))

    def plc_get_rear_window_type_3(self):
        return util.get_string(self.__plc_read_data(86, 18), 0, 18)

    def plc_set_rear_window_type_3(self, window_type: str):
        self.__plc_write_data(86, bytearray(window_type, 'utf-8'))

    def __plc_read_data(self, base, size):
        self.__lock_plc.acquire()
        db = self.plc.plc_read_db(base, size)
        self.__lock_plc.release()
        return db

    def __plc_write_data(self, base, db):
        self.__lock_plc.acquire()
        self.plc.plc_write_db(db, base)
        self.__lock_plc.release()

    def database_add_new_glass(self):
        self.__lock_database.acquire()
        values = "'xxxxxxxxxxxxx', 'xxxxxxxxxxxxxxxxxx', '7', 0"
        self.db.insert("INSERT INTO glassdatabase (vehiclenumber, rearwindowtype, vehiclemodel, ID) VALUES (" + values + ");");
        self.__lock_database.release()

    def database_delete_glass_from_production(self, record_index):
        self.__lock_database.acquire()
        self.db.insert("DELETE FROM glassinproduction WHERE record_index='" + str(record_index) + "';")
        self.__lock_database.release()

    def database_move_glass_to_production(self):
        self.__lock_database.acquire()

        self.db.insert("INSERT INTO glassinproduction SELECT * FROM glassdatabase order by record_index limit 1;")
        self.db.insert("DELETE FROM glassdatabase WHERE record_index=(SELECT record_index FROM glassdatabase order by record_index limit 1);")

        self.__lock_database.release()

    def database_get_next_glass_to_production(self):
        self.__lock_database.acquire()

        db_record = self.db.select("SELECT * FROM glassdatabase order by record_index limit 1")
        fetch = db_record.fetchall()

        record_index = fetch[0][0]
        vehicle_number = fetch[0][1]
        rear_window_type = fetch[0][2]
        vehicle_model = fetch[0][3]
        glass = GlassInfo(record_index, vehicle_number, vehicle_model, rear_window_type, 0)

        self.__lock_database.release()

        return glass

    def database_get_next_glass_to_gluing(self):
        self.__lock_database.acquire()

        db_record = self.db.select("SELECT * FROM glassinproduction order by record_index limit 1")

        fetch = db_record.fetchall()

        record_index = fetch[0][0]
        vehicle_number = fetch[0][1]
        rear_window_type = fetch[0][2]
        vehicle_model = fetch[0][3]

        glass = GlassInfo(record_index, vehicle_number, vehicle_model, rear_window_type, 0)

        self.__lock_database.release()

        return glass

    def database_set_glass_assembled(self):
        self.__lock_database.acquire()

        self.db.insert("INSERT INTO assembledglass SELECT * FROM glassinproduction order by record_index limit 1;")
        self.db.insert("DELETE FROM glassinproduction WHERE record_index=(SELECT record_index FROM glassinproduction order by record_index limit 1);")
        self.__lock_database.release()

    def database_set_glass_id(self, id_value=ctypes.c_uint32):
        self.__lock_database.acquire()
        self.db.insert("UPDATE glassinproduction SET ID=" + str(id_value) + " WHERE record_index=(SELECT record_index FROM glassinproduction order by record_index limit 1);")
        self.__lock_database.release()

    def database_set_glass_vehicle_number(self, index, vehicle_number):
        self.__lock_database.acquire()
        query = "UPDATE glassdatabase SET vehiclenumber='" + vehicle_number + "' WHERE record_index = " + str(index) + ";"
        print(query)
        self.db.insert(query)
        self.__lock_database.release()

    def database_set_glass_rear_window_type(self, index, rear_window_type):
        self.__lock_database.acquire()
        self.db.insert("UPDATE glassdatabase SET rearwindowtype='"+rear_window_type+"' WHERE record_index = " + str(index) + ";")
        self.__lock_database.release()

    def database_set_glass_vehicle_model(self, index, vehicle_model):
        self.__lock_database.acquire()
        self.db.insert("UPDATE glassdatabase SET vehiclemodel='"+vehicle_model+"' WHERE record_index = " + str(index) + ";")
        self.__lock_database.release()

    def database_get_glassdatabase_content(self):
        self.__lock_database.acquire()
        db_table = self.db.select("SELECT * FROM glassdatabase")
        self.__lock_database.release()

        return db_table

    def database_get_glassinproduction_content(self):
        self.__lock_database.acquire()
        db_table = self.db.select("SELECT * FROM glassinproduction")
        self.__lock_database.release()

        return db_table

    def database_get_assembledglass_content(self):
        self.__lock_database
        db_table = self.db.select("SELECT * FROM assembledglass")

        self.__lock_database.release()

        return db_table

