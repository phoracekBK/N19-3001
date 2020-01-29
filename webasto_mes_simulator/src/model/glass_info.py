import ctypes


class GlassInfo:
    record_index: int
    vehicle_number: str
    vehicle_model: ctypes.c_uint8
    rear_window_type: str
    ID: ctypes.c_uint32

    def __init__(self, record_index, vehicle_number, vehicle_model, rear_window_type, ID):
        self.record_index = record_index
        self.vehicle_model = vehicle_model
        self.vehicle_number = vehicle_number
        self.rear_window_type = rear_window_type
        self.ID = ID
