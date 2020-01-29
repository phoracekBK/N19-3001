from PyQt5 import QtWidgets, QtCore
from src.view.ui.ui import UIStruct
from model.model import Model
from controler.controler import Controler
from model.glass_info import GlassInfo
import sqlite3


class MainWindow(QtWidgets.QMainWindow):
    ui: UIStruct
    controler_ref: Controler
    model_ref: Model
    __timer: QtCore.QTimer

    def __init__(self, model_ref, controler_ref, **kwargs):
        super(MainWindow, self).__init__(**kwargs)

        self.__init_timer()

        self.model_ref = model_ref
        self.controler_ref = controler_ref
        self.ui = UIStruct()
        self.event_connection()
        self.init_gui()

    def __init_timer(self):
        self.__timer = QtCore.QTimer(self)
        self.__timer.start(1000)

    def closeEvent(self, event):
        print("vynout")
        QtCore.QCoreApplication.quit()

    def init_gui(self):
        self.setWindowTitle("MES Simulator - 1.0.0")
        self.setCentralWidget(self.ui.main_container)
        self.showMaximized()
        self.update_assembled_glass_table()
        self.update_glass_in_production_table()
        self.update_glass_database_table()

    def event_connection(self):
        self.__timer.timeout.connect(self.timer_callback)

        self.ui.glass_database_widget.btn_refresh.clicked.connect(self.update_glass_database_table)
        self.ui.glass_database_widget.table.itemChanged.connect(self.glass_database_table_edit_callback)
        self.ui.glass_database_widget.btn_add_glass.clicked.connect(self.glass_database_btn_add_glass_callback)
        self.ui.glass_database_widget.btn_move_to_production.clicked.connect(self.glass_database_btn_move_to_production_callback)

        self.ui.glass_in_production_widget.btn_refresh.clicked.connect(self.glass_in_production_btn_refresh_callback)
        self.ui.glass_in_production_widget.btn_delete_glass.clicked.connect(self.glass_in_production_btn_delete_glass_callback)
        self.ui.glass_in_production_widget.btn_gluing_request.clicked.connect(self.glass_in_production_btn_gluing_request_callback)

        self.ui.assembled_glass_widget.btn_refresh.clicked.connect(self.assembled_glass_btn_refresh_callback)

    def timer_callback(self):
        if self.controler_ref.get_operation_progress_status():
            self.ui.tab_container.setEnabled(False)
            self.ui.loading_icon.setVisible(True)
        else:
            self.ui.tab_container.setEnabled(True)
            self.ui.loading_icon.setVisible(False)

        # if self.controler_ref.get_sending_glass_to_gluing_process_status():

        if self.controler_ref.get_deleting_glass_from_production_process_status():
            self.update_glass_in_production_table()

        if self.controler_ref.get_moving_glass_to_production_process_status():
            self.update_glass_database_table()
            self.update_glass_in_production_table()

        if self.controler_ref.get_assembling_process_status():
            self.update_glass_in_production_table()
            self.update_assembled_glass_table()

    def glass_database_btn_refresh_callback(self):
        self.update_glass_database_table()

    def glass_in_production_btn_refresh_callback(self):
        self.update_glass_in_production_table()

    def assembled_glass_btn_refresh_callback(self):
        self.update_assembled_glass_table()

    def glass_database_btn_add_glass_callback(self):
        self.model_ref.database_add_new_glass()
        self.update_glass_database_table()

    def glass_database_btn_move_to_production_callback(self):
        self.controler_ref.mes_move_to_production()

    def glass_in_production_btn_delete_glass_callback(self):
        try:
            select = self.ui.glass_in_production_widget.table.selectedItems()

            if len(select) > 0:
                glass = GlassInfo(int(select[0].text()), select[1].text(), select[2].text(), select[3].text(), 0)
                self.controler_ref.mes_delete(glass)
        except Exception as e:
            print("Exception: " + str(e))

    def glass_in_production_btn_gluing_request_callback(self):
        self.controler_ref.mes_gluing()

    def update_glass_database_table(self):
        db_table = self.model_ref.database_get_glassdatabase_content()
        self.update_table_content(self.ui.glass_database_widget.table, db_table, True)

    def update_glass_in_production_table(self):
        db_table = self.model_ref.database_get_glassinproduction_content()
        self.update_table_content(self.ui.glass_in_production_widget.table, db_table, False)

    def update_assembled_glass_table(self):
        db_table = self.model_ref.database_get_assembledglass_content()
        self.update_table_content(self.ui.assembled_glass_widget.table, db_table, False)

    def update_table_content(self, table: QtWidgets.QTableWidget, db_table, editable):
        try:
            table.blockSignals(True)
            table.clear()

            fetch = db_table.fetchall()
            if not fetch is None:
                table.setRowCount(len(fetch))
                table.setColumnCount(5)

            labels = ["index", "vehicle number", "rear window type", "vehicle model", "ID"]
            table.setHorizontalHeaderLabels(labels)

            i = 0
            for row in fetch:
                table.setItem(i, 0, self.get_table_widget_item(str(row[0]), False))
                table.setItem(i, 1, self.get_table_widget_item(row[1], editable))
                table.setItem(i, 2, self.get_table_widget_item(row[2], editable))
                table.setItem(i, 3, self.get_table_widget_item(row[3], editable))
                table.setItem(i, 4, self.get_table_widget_item(str(row[4]), False))
                i = i + 1

            table.blockSignals(False)
        except Exception as e:
            print("Exception: " + str(e))

    def get_table_widget_item(self, content, editable):
        item = QtWidgets.QTableWidgetItem(content)
        item.setTextAlignment(QtCore.Qt.AlignCenter)

        if not editable:
            item.setFlags(item.flags() ^ QtCore.Qt.ItemIsEditable)

        return item

    def glass_database_table_edit_callback(self, item):
        try:
            index = self.ui.glass_database_widget.table.item(item.row(), 0).text()

            if item.column() == 1:
                self.model_ref.database_set_glass_vehicle_number(index, item.text())
            elif item.column() == 2:
                self.model_ref.database_set_rear_window_type(index, item.text())
            elif item.column() == 3:
                self.model_ref.database_set_glass_vehicle_model(index, item.text())

        except Exception as e:
            print("Exception: " + str(e))
