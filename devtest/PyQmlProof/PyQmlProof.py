from pathlib import Path
from PyQt5 import Qt
import sys

class OutlinePath(Qt.QObject):
    pointCountChanged = Qt.pyqtSignal()

    def __init__(self, parent=None):
        super().__init__(parent)
        self._points = []

    @Qt.pyqtProperty(int, notify=pointCountChanged)
    def pointCount(self):
        return len(self._points)

    @Qt.pyqtSlot(Qt.QPoint)
    def appendPoint(self, point):
        self._points.append(point)
        self.pointCountChanged.emit()
        print(self._points)

    @Qt.pyqtSlot(int, result=Qt.QPoint)
    def pointAt(self, idx):
        return self._points[idx]

    @Qt.pyqtSlot(int, Qt.QPoint)
    def setPointAt(self, idx, p):
        self._points[idx] = p

    @Qt.pyqtSlot(int)
    def delPointAt(self, idx):
        del self._points[idx]
        self.pointCountChanged.emit()

if __name__ == "__main__":
    app = Qt.QApplication(sys.argv)
    Qt.qmlRegisterType(OutlinePath)
    quick_widget = Qt.QQuickWidget()
    quick_widget.setResizeMode(Qt.QQuickWidget.SizeRootObjectToView)
    outlinePath = OutlinePath()
    quick_widget.rootContext().setContextProperty("outlinePath", outlinePath)
    Path(__file__).parent / 'PyQmlProof.py'
    url = Qt.QUrl(str(Path(__file__).parent / 'PyQmlProof.qml'))
    quick_widget.setSource(url)
    quick_widget.show()
    #
    app.exec_()
