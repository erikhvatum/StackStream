from pathlib import Path
from PyQt5 import Qt
import sys

class Layer(Qt.QObject):
    gammaChanged = Qt.pyqtSignal()
    tintChanged = Qt.pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)
        self._gamma = 1.0
        self._tint = Qt.QColor(Qt.Qt.white)

    @Qt.pyqtProperty(float, notify=gammaChanged)
    def gamma(self):
        return self._gamma

    @gamma.setter
    def gamma(self, v):
        v = float(v)
        self._gamma = v
        self.gammaChanged.emit()

    @Qt.pyqtProperty(Qt.QColor, notify=tintChanged)
    def tint(self):
        return self._tint

    @tint.setter
    def tint(self, v):
        self._tint = v
        self.tintChanged.emit()

class TestModel(Qt.QAbstractListModel):
    ROLES = ['gamma', 'tint']
    layersChanged = Qt.pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)
        self._layers = list()

    def rowCount(self, midx):
        return len(self._layers)

    def data(self, midx, role):
        if midx.isValid():
            print('data')
            return Qt.QVariant(getattr(self._layers[midx.row()], '_' + self.ROLES[role - Qt.Qt.UserRole]))
        return Qt.QVariant()

    def roleNames(self):
        return {i : v.encode('utf8') for i, v in enumerate(self.ROLES, Qt.Qt.UserRole)}

    @Qt.pyqtSlot(int, Qt.QVariant, str)
    def setProperty(self, idx, v, role):
        print('setProperty(self, {}, {}, {})'.format(idx, v, role))
        setattr(self._layers[idx], role, v)
        col = self.ROLES.index(role)
        midx = self.createIndex(idx, col)
        self.dataChanged.emit(midx, midx, [col + Qt.Qt.UserRole])

test_model=None
def make_engine():
    engine = Qt.QQmlApplicationEngine()
    Qt.qmlRegisterType(Layer, 'StackStream', 1, 0, 'Layer')
    Qt.qmlRegisterType(TestModel, 'StackStream', 1, 0, 'TestModel')
    global test_model
    test_model = TestModel()
    layer=Layer()
    layer.gamma = 0.8
    layer.tint = Qt.QColor(Qt.Qt.red)
    test_model._layers.append(layer)
    layer=Layer()
    layer.gamma = 0.6
    layer.tint = Qt.QColor(Qt.Qt.blue)
    test_model._layers.append(layer)
    engine.rootContext().setContextProperty("test_model", test_model)
    main_qml_fpath = Path(__file__).parent / 'main.qml'
    engine.load(Qt.QUrl(str(main_qml_fpath)))
    return engine

if __name__ == '__main__':
    app = Qt.QApplication(sys.argv)
    engine = make_engine()
    app.exec()
