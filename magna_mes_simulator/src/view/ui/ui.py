from PyQt5 import QtWidgets, QtGui
import sys
from .ui_glass_database import UIGlassDatabase
from .ui_assembled_glass import UIAssembledGlass
from .ui_glass_in_productions import UIGlassInProduction


class UIStruct:
# public widgets
    glass_database_widget: UIGlassDatabase
    glass_in_production_widget: UIGlassInProduction
    assembled_glass_widget: UIAssembledGlass
    loading_icon: QtWidgets.QLabel
    main_container: QtWidgets.QWidget
    tab_container: QtWidgets.QTabWidget

    __vbox_layout: QtWidgets.QVBoxLayout
    __loading_movie: QtGui.QMovie
    __hbox_layout: QtWidgets.QHBoxLayout
    __loading_widget: QtWidgets.QWidget

# private widgets

    def __init__(self):
        self.__build_widgets()
        self.__pack()

    def __pack(self):
        self.tab_container.addTab(self.glass_database_widget, "Glass database")
        self.tab_container.addTab(self.glass_in_production_widget, "Glass in production")
        self.tab_container.addTab(self.assembled_glass_widget, "Assembled glass")

        self.hbox_layout.addStretch()
        self.hbox_layout.addWidget(self.loading_icon)
        self.hbox_layout.addStretch()

        self.__vbox_layout.addWidget(self.tab_container)
        self.__vbox_layout.addWidget(self.__loading_widget)
        self.main_container.setLayout(self.__vbox_layout)

    def __build_widgets(self):
        self.__init_tab_container()
        self.glass_database_widget = UIGlassDatabase()
        self.glass_in_production_widget = UIGlassInProduction()
        self.assembled_glass_widget = UIAssembledGlass()

        self.__vbox_layout = QtWidgets.QVBoxLayout()
        self.loading_icon = QtWidgets.QLabel()
        self.__loading_movie = QtGui.QMovie("img/ajax-loader.gif")
        self.loading_icon.setMovie(self.__loading_movie)
        self.__loading_movie.start()
        self.main_container = QtWidgets.QWidget()
        self.hbox_layout = QtWidgets.QHBoxLayout()
        self.__loading_widget = QtWidgets.QWidget()
        self.__loading_widget.setLayout(self.hbox_layout)
        self.__loading_widget.setMinimumHeight(40)

    def __init_tab_container(self):
        self.tab_container = QtWidgets.QTabWidget()

