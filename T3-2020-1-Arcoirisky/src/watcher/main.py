import sys

from PyQt5 import QtWidgets

from window import Window

if __name__ == "__main__":

  try:

    app = QtWidgets.QApplication([])

    height = int(sys.argv[1])
    width = int(sys.argv[2])

    max_size = int(sys.argv[3])

    window = Window(height, width, max_size)

    window.update()

    sys.exit(app.exec_())

  except KeyboardInterrupt:
    pass
