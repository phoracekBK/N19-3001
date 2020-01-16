import snap7
from snap7 import util
from snap7 import client
import time


# for install snap7 library to windows system it's necessarily to copy snap7.dll
# library to %systemroot%\System32 and %systemroot%\SysWoW64 and type WIN+R and
# run command C:\Windows\SysWOW64\REGSVR32 snap7.dll


class GlassInfo:
    vehicleNumber: str
    vehicleModel: str
    rearWindowType: str

    def __init__(self, vehicleNumber, vehicleModel, rearWindowType):
        self.vehicleNumber = vehicleNumber
        self.vehicleModel = vehicleModel
        self.rearWindowType = rearWindowType


class PLC:
    plcClient: client
    rackIndex: int
    slotIndex: int
    ipAddress: str
    dbIndex: int
    connectionTry: int

    def __init__(self, ipAddress, rackIndex, slotIndex, dbIndex):
        self.rackIndex = rackIndex
        self.slotIndex = slotIndex
        self.ipAddress = ipAddress
        self.dbIndex = dbIndex
        self.connectionTry = 0

        self.plcClient = snap7.client.Client()

    def connect(self):
        return self.plcClient.connect(self.ipAddress, self.rackIndex, self.slotIndex)

    def write(self, db):
        if self.plcClient.get_connected():
            self.connectionTry = 0
            return self.plcClient.db_write(self.dbIndex, 0, db)
        else:
            if self.connectionTry < 3:
                self.connect()
                self.connectionTry = self.connectionTry + 1
                return self.write(db)
            else:
                self.connectionTry = 0
                return False

    def read(self, dbSize):
        if self.plcClient.get_connected():
            self.connectionTry = 0
            return self.plcClient.db_read(self.dbIndex, 0, dbSize)
        else:
            if self.connectionTry < 3:
                self.connect()
                self.connectionTry = self.connectionTry + 1
                return self.read(dbSize)
            else:
                self.connectionTry = 0
                return False


def read_from_file(path):
    try:
        out = open(path, "r")
        csv = out.read()
        out.close()
        return csv
    except IOError:
        return ""


def write_to_file(path, csv):
    out = open(path, "w")
    out.write(csv)
    out.close()


class PCQueue:
    plc: PLC
    queue: list
    dbSize: int
    backupFilePath: str

    def __init__(self, dbSize):
        self.plc = PLC("192.168.0.1", 0, 1, 5)
        self.queue = []
        self.dbSize = dbSize
        self.backupFilePath = "queue.csv"

        self.parse_csv(read_from_file(self.backupFilePath))

    def generate_csv(self, index, csv):
        if index < len(self.queue):
            csv = csv + self.queue[index].vehicleNumber + ";" + self.queue[index].vehicleModel + ";" + self.queue[
                index].rearWindowType + "\n"
            return self.generate_csv(index + 1, csv)
        else:
            return csv

    def to_csv(self):
        return self.generate_csv(0, "")

    def parse_csv_line(self, csv_line, index):
        if index < len(csv_line):
            glass_struct = csv_line[index].split(sep=';')

            if len(glass_struct) == 3:
                self.queue.append(GlassInfo(glass_struct[0], glass_struct[1], glass_struct[2]))
                self.parse_csv_line(csv_line, index + 1)

    def parse_csv(self, csv):
        csv_lines = csv.split(sep='\n')
        self.parse_csv_line(csv_lines, 0)

    def copy_firest_n_glass_elements_to_visual_queue(self, db, index):
        if index < 10:
            if index < len(self.queue):
                util.set_string(db, 208 + (206 * index), self.queue[index].vehicleNumber, 100)
                util.set_string(db, 208 + (206 * index) + 102, self.queue[index].vehicleModel, 50)
                util.set_string(db, 208 + (206 * index) + 154, self.queue[index].rearWindowType, 50)
            else:
                util.set_string(db, 208 + (206 * index), "", 100)
                util.set_string(db, 208 + (206 * index) + 102, "", 50)
                util.set_string(db, 208 + (206 * index) + 154, "", 50)

            return self.copy_firest_n_glass_elements_to_visual_queue(db, index + 1)

        return db

    def synchronize_visual_queue(self, db):
        return self.copy_firest_n_glass_elements_to_visual_queue(db, 0)

    def event_add_glass(self, db):
        self.queue.append(
            GlassInfo(util.get_string(db, 0, 100), util.get_string(db, 102, 50), util.get_string(db, 154, 50)))
        write_to_file(self.backupFilePath, self.to_csv())
        return self.synchronize_visual_queue(db)

    def event_add_priority_glass(self, db):
        self.queue.insert(0, GlassInfo(util.get_string(db, 0, 100), util.get_string(db, 102, 50),
                                       util.get_string(db, 154, 50)))
        write_to_file(self.backupFilePath, self.to_csv())
        return self.synchronize_visual_queue(db)

    def find_and_delete_glass_in_queue(self, vehicleNumber, index):
        if index < len(self.queue):
            if self.queue[index].vehicleNumber == vehicleNumber:
                del self.queue[index]
            else:
                self.find_and_delete_glass_in_queue(vehicleNumber, index + 1)

    def event_delete_glass(self, db):
        self.find_and_delete_glass_in_queue(util.get_string(db, 0, 100), 0)
        write_to_file(self.backupFilePath, self.to_csv())
        return self.synchronize_visual_queue(db)

    def event_bus_init(self):
        db = self.plc.read(self.dbSize)
        db = self.synchronize_visual_queue(db)
        self.plc.write(db)

    def event_bus_main(self):
        self.event_bus_init()

        while (True):
            db = self.plc.read(self.dbSize)

            if util.get_bool(db, 206, 0):
                db = self.event_add_glass(db)
                util.set_bool(db, 2268, 0, True)
                print("add glass")
                self.plc.write(db)
            elif util.get_bool(db, 206, 1):
                db = self.event_add_priority_glass(db)
                util.set_bool(db, 2268, 0, True)
                print("add priority glass")
                self.plc.write(db)
            elif util.get_bool(db, 206, 2):
                db = self.event_delete_glass(db)
                util.set_bool(db, 2268, 0, True)
                print("delete glass")
                self.plc.write(db)

            time.sleep(0.3)


def main_():
    pc_queue = PCQueue(2269)

    pc_queue.event_bus_main()


main_()
