from PyQt5 import QtWidgets
import sys


class UIGlassDatabase(QtWidgets.QWidget):
#publicdd
    table: QtWidgets.QTableWidget
    btn_move_to_production: QtWidgets.QPushButton
    btn_add_glass: QtWidgets.QPushButton
    btn_refresh: QtWidgets.QPushButton

#private
    __table_scroll: QtWidgets.QScrollArea
    __vbox: QtWidgets.QVBoxLayout
    __hbox_1: QtWidgets.QHBoxLayout

    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.__build_widgets()
        self.__pack()

    def __pack(self):
        self.setLayout(self.__vbox)

        self.__hbox_1.addStretch()
        self.__hbox_1.addWidget(self.btn_move_to_production)
        self.__hbox_1.addWidget(self.btn_add_glass)
        self.__hbox_1.addWidget(self.btn_refresh)

        self.__table_scroll.setWidget(self.table)
        self.__table_scroll.setWidgetResizable(True)

        self.__vbox.addLayout(self.__hbox_1)
        self.__vbox.addWidget(self.__table_scroll)

    def __init_menu_bar(self):
        self.__hbox_1 = QtWidgets.QHBoxLayout()
        self.btn_refresh = QtWidgets.QPushButton("Obnovit zobrazení", self)
        self.btn_add_glass = QtWidgets.QPushButton("Přidat záznam", self)
        self.btn_move_to_production = QtWidgets.QPushButton("Odeslat do produkce", self)

    def __build_widgets(self):
        self.__init_table_widget()

        self.__table_scroll = QtWidgets.QScrollArea()
        self.__vbox = QtWidgets.QVBoxLayout()

        self.__init_menu_bar()

    def __init_table_widget(self):
        self.table = QtWidgets.QTableWidget()
        self.table.horizontalHeader().setSectionResizeMode(QtWidgets.QHeaderView.Stretch)
        self.table.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)
        self.table.setSelectionMode(QtWidgets.QAbstractItemView.SingleSelection)
