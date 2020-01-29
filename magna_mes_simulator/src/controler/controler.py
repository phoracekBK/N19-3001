
from model.model import Model
from model.glass_info import GlassInfo

import time
import threading


class Controler:
    __model_ref: Model
    __automatic_assembly_confirmation: bool

    __assembling_process: bool
    __moving_glass_to_production_progress: bool
    __deleting_glass_from_production_progress: bool
    __sending_glass_to_gluing_progress: bool

    __moving_glass_to_production_finished: bool
    __deleting_glass_finished: bool
    __sending_glass_to_gluing_finish: bool
    __assembling_finished: bool

    def __init__(self, model_ref):
        self.__automatic_assembly_confirmation = True

        self.__assembling_process = False
        self.__moving_glass_to_production_progress = False
        self.__deleting_glass_from_production_progress = False
        self.__sending_glass_to_gluing_progress = False

        self.__sending_glass_to_gluing_finish = False
        self.__deleting_glass_finished = False
        self.__moving_glass_to_production_finished = False
        self.__assembling_finished = False

        self.__model_ref = model_ref
        self.__initialize_stepper_threads()
        self.enable_automatic_assembly_confirmation()

    def enable_automatic_assembly_confirmation(self):
        self.__automatic_assembly_confirmation = True

    def disable_automatic_assembly_confirmation(self):
        self.__automatic_assembly_confirmation = False

    def get_automatic_assembly_confirmation_status(self):
        return self.__automatic_assembly_confirmation

    def __initialize_stepper_threads(self):
        self.__run_controler_thread(method=self.__output_sequence_loop, params=None)

    def get_operation_progress_status(self):
        return (self.__sending_glass_to_gluing_progress or
                self.__deleting_glass_from_production_progress or
                self.__moving_glass_to_production_progress or
                self.__assembling_process)

    def get_sending_glass_to_gluing_process_status(self):
        if self.__sending_glass_to_gluing_finish:
            self.__sending_glass_to_gluing_finish = False
            return True
        else:
            return False

    def get_deleting_glass_from_production_process_status(self):
        if self.__deleting_glass_finished:
            self.__deleting_glass_finished = False
            return True
        else:
            return False

    def get_moving_glass_to_production_process_status(self):
        if self.__moving_glass_to_production_finished:
            self.__moving_glass_to_production_finished = False
            return True
        else:
            return False

    def get_assembling_process_status(self):
        if self.__assembling_finished:
            self.__assembling_finished = False
            return True
        else:
            return False

    def __run_controler_thread(self, method, params):

        if params is None:
            t = threading.Thread(target=method)
        else:
            t = threading.Thread(target=method, args=(params,))

        t.daemon = True
        t.start()
        return t

    def mes_move_to_production(self):
        self.__run_controler_thread(method=self.__move_glass_to_production_loop, params=None)

    def mes_delete(self, glass: GlassInfo):
        self.__run_controler_thread(method=self.__delete_glass_loop, params=glass)

    def mes_gluing(self):
        self.__run_controler_thread(method=self.__gluing_glass_loop, params=None)

    def __move_glass_to_production_loop(self):
        self.__moving_glass_to_production_progress = True
        step = 0
        fail_counter = 0
        glass = self.__model_ref.database_get_next_glass_to_production()

        while step < 2 and fail_counter < 30:
            if step == 0:
                if not self.__model_ref.plc_get_flag_1():

                    self.__model_ref.plc_set_vehicle_no_1(glass.vehicle_number)
                    self.__model_ref.plc_set_rear_window_type_1(glass.rear_window_type)
                    self.__model_ref.plc_set_flag_1(True)
                    step = 1
            if step == 1:
                if not self.__model_ref.plc_get_flag_1():
                    self.__model_ref.database_move_glass_to_production()
                    step = 2

            fail_counter = fail_counter + 1
            time.sleep(1)

        self.__moving_glass_to_production_progress = False
        self.__moving_glass_to_production_finished = True

    def __delete_glass_loop(self, glass: GlassInfo):
        print("Hello world" + glass.vehicle_number)

        self.__deleting_glass_from_production_progress = True
        step = 0
        fail_counter = 0

        while step < 2 and fail_counter < 30:
            if step == 0:
                if not self.__model_ref.plc_get_flag_2():
                    self.__model_ref.plc_set_vehicle_no_2(glass.vehicle_number)
                    self.__model_ref.plc_set_rear_window_type_2(glass.rear_window_type)
                    self.__model_ref.plc_set_flag_2(True)
                    step = 1
            elif step == 1:
                if not self.__model_ref.plc_get_flag_2():
                    self.__model_ref.database_delete_glass_from_production(glass.record_index)
                    step = 2

            fail_counter = fail_counter + 1
            time.sleep(1)

        self.__deleting_glass_from_production_progress = False
        self.__deleting_glass_finished = True

    def __gluing_glass_loop(self):
        self.__sending_glass_to_gluing_progress = True
        step = 0
        fail_counter = 0
        glass = self.__model_ref.database_get_next_glass_to_gluing()

        while step < 2 and fail_counter < 30:
            if step == 0:
                if not self.__model_ref.plc_get_flag_3():
                    self.__model_ref.plc_set_vehicle_no_3(glass.vehicle_number)
                    self.__model_ref.plc_set_rear_window_type_3(glass.rear_window_type)
                    self.__model_ref.plc_set_flag_3(True)
                    step = 1
            elif step == 1:
                if not self.__model_ref.plc_get_flag_3():
                    step = 2

            fail_counter = fail_counter + 1
            time.sleep(1)

        self.__sending_glass_to_gluing_progress = False
        self.__sending_glass_to_gluing_finish = True

    def __output_sequence_loop(self):
        while True:
            if self.__model_ref.plc_get_flag_4():
                self.__assembling_process = True

                glass_id = self.__model_ref.plc_get_id_of_glass_panel()
                self.__model_ref.database_set_glass_id(glass_id)

                self.__assembling_finished = True
                self.__assembling_process = False

                self.__model_ref.plc_set_flag_4(False)

            if self.__model_ref.plc_get_glass_panel_taken_out():
                self.__assembling_process = True
                self.__model_ref.plc_set_glass_panel_taken_out(False)

                # after 5 seconds sends automatic confirmation of glass assembling

                time.sleep(5)
                self.__assembling_process = False

                if self.__automatic_assembly_confirmation:
                    self.__model_ref.plc_set_glass_panel_is_in_tailgate(True)
                    self.__model_ref.database_set_glass_assembled()
                    self.__assembling_finished = True

            time.sleep(1)

