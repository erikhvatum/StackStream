from pathlib import Path
from PyQt5 import Qt
import sys
from ris_widget import om
from list_role_model import ListRoleModel

class OutlinePoint(Qt.QObject):
    x_Changed = Qt.pyqtSignal(float)
    y_Changed = Qt.pyqtSignal(float)

    def __init__(self, x=0, y=0, parent=None):
        super().__init__(parent)
        self._x = x
        self._y = y

    @Qt.pyqtProperty(float, notify=x_Changed)
    def x_(self):
        return self._x

    @x_.setter
    def setX_(self, x):
        x = float(x)
        if x != self._x:
            self._x = x
            self.x_Changed.emit(x)

    @Qt.pyqtProperty(float, notify=y_Changed)
    def y_(self):
        return self._y

    @y_.setter
    def setY_(self, y):
        y = float(y)
        if y != self._y:
            self._y = y
            self.y_Changed.emit(y)

class OutlinePointList(om.UniformSignalingList):
    def take_input_element(self, obj):
        if isinstance(obj, OutlinePoint):
            return obj
        elif isinstance(obj, (Qt.QPointF, Qt.QPoint)):
            return OutlinePoint(obj.x(), obj.y())
        else:
            i = iter(obj)
            return OutlinePoint(next(i), next(i))

class PointListModel(ListRoleModel):
    @Qt.pyqtSlot(float, float)
    def append(self, x, y):
        self.signaling_list.append(OutlinePoint(x, y))

    @Qt.pyqtSlot(int)
    def delAtIndex(self, idx):
        del self.signaling_list[idx]

if __name__ == "__main__":
    app = Qt.QApplication(sys.argv)
    Qt.qmlRegisterType(OutlinePoint)
    Qt.qmlRegisterType(ListRoleModel)
    point_list = OutlinePointList()
    point_list_model = PointListModel(('x_', 'y_'), point_list)
    quick_widget = Qt.QQuickWidget()
    quick_widget.setResizeMode(Qt.QQuickWidget.SizeRootObjectToView)
    quick_widget.rootContext().setContextProperty("pointListModel", point_list_model)
    Path(__file__).parent / 'PyQmlProof.py'
    url = Qt.QUrl(str(Path(__file__).parent / 'PyQmlProof.qml'))
    quick_widget.setSource(url)
    quick_widget.show()
    #
    app.exec_()
