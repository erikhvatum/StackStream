from pathlib import Path
from PyQt5 import Qt
import sys



app = Qt.QApplication(sys.argv)
engine = Qt.QQmlApplicationEngine()

main_qml_fpath = Path(__file__).parent / 'main.qml'
engine.load(Qt.QUrl(str(main_qml_fpath)))
app.exec()
