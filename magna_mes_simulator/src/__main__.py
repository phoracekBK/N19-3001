from model.model import Model
from controler.controler import Controler
from view.view import MainWindow
from PyQt5 import QtWidgets
import sys


def main():
    app = QtWidgets.QApplication(sys.argv)
    model_ref = Model()
    controler_ref = Controler(model_ref)
    view_ref = MainWindow(model_ref, controler_ref)
    sys.exit(app.exec())



main()




