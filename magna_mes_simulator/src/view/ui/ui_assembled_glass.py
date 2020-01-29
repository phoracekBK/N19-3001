from PyQt5 import QtWidgets
import sys


class UIAssembledGlass(QtWidgets.QWidget):
    table: QtWidgets.QTableWidget
    btn_refresh: QtWidgets.QPushButton

    __table_scroll: QtWidgets.QScrollArea
    __vbox: QtWidgets.QVBoxLayout
    __hbox1: QtWidgets.QHBoxLayout
    __menu_bar_widget: QtWidgets.QWidget

    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.__build_widgets()
        self.__pack_widgets()

    def __build_widgets(self):
        self.__table_scroll = QtWidgets.QScrollArea()
        self.__vbox = QtWidgets.QVBoxLayout()
        self.__hbox1 = QtWidgets.QHBoxLayout()

        self.btn_refresh = QtWidgets.QPushButton("Obnovit zobrazení", self)

        self.__init_table_widget()

    def __init_table_widget(self):
        self.table = QtWidgets.QTableWidget()
        self.table.horizontalHeader().setSectionResizeMode(QtWidgets.QHeaderView.Stretch)
        self.table.setSelectionBehavior(QtWidgets.QAbstractItemView.SelectRows)
        self.table.setSelectionMode(QtWidgets.QAbstractItemView.SingleSelection)

    def __pack_widgets(self):
        self.setLayout(self.__vbox)

        self.__table_scroll.setWidget(self.table)
        self.__table_scroll.setWidgetResizable(True)

        self.__hbox1.addStretch()
        self.__hbox1.addWidget(self.btn_refresh)

        self.__vbox.addLayout(self.__hbox1)
        self.__vbox.addWidget(self.__table_scroll)
