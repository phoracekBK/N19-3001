from PyQt5 import QtWidgets
import sys


class UIGlassInProduction(QtWidgets.QWidget):
    table: QtWidgets.QTableWidget
    btn_gluing_request: QtWidgets.QPushButton
    btn_delete_glass: QtWidgets.QPushButton
    btn_refresh: QtWidgets.QPushButton

    __table_scroll: QtWidgets.QScrollArea
    __vbox: QtWidgets.QVBoxLayout
    __hbox1: QtWidgets.QHBoxLayout

    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.__build_widgets()
        self.__pack_widgets()

    def __build_widgets(self):
        self.__table_scroll = QtWidgets.QScrollArea()
        self.__vbox = QtWidgets.QVBoxLayout()
        self.__hbox1 = QtWidgets.QHBoxLayout()

        self.__init_table_widget()

        self.btn_delete_glass = QtWidgets.QPushButton("Smazat sklo", self)
        self.btn_gluing_request = QtWidgets.QPushButton("Nanést lepidlo", self)
        self.btn_refresh = QtWidgets.QPushButton("Obnovit zobrazení", self)

    def __init_menu_bar(self):
        self.__hbox1.addStretch()
        self.__hbox1.addWidget(self.btn_delete_glass)
        self.__hbox1.addWidget(self.btn_gluing_request)
        self.__hbox1.addWidget(self.btn_refresh)

    def __init_table_widget(self):
        self.table = QtWidgets.QTableWidget()
        self.table.horizontalHeader().setSectionResizeMode(QtWidgets.QHeaderView.Stretch)
        self.table.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)
        self.table.setSelectionMode(QtWidgets.QAbstractItemView.SingleSelection)

    def __pack_widgets(self):
        self.setLayout(self.__vbox)
        self.__init_menu_bar()
        self.__table_scroll.setWidget(self.table)
        self.__table_scroll.setWidgetResizable(True)

        self.__vbox.addLayout(self.__hbox1)
        self.__vbox.addWidget(self.__table_scroll)
