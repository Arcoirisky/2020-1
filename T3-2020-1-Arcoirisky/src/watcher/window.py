from PyQt5 import QtWidgets, QtGui

from canvas import Canvas
from updater import UpdateThread

import math

from typing import List, Tuple

class Window(QtWidgets.QMainWindow):
  """
  Ventana que visualiza el problema
  
  Parámetros:
  -----------

  aquariums: Matriz de ints
    
    El número en cada celda indica el acuario al que pertenece

  max_size: int, opcional

    Ancho y alto máximo que puede tener esta ventana.
  """

  def __init__(self, mat_height: int, mat_width: int, max_size: int=512):
    super().__init__()


    # Dejamos la ventana tan grande como sea posible sin romper la proporción

    # Nos regimos por el que sea más grande
    if mat_height > mat_width:
      tile_size = math.floor(max_size / (mat_height + 2))
    else:
      tile_size = math.floor(max_size / (mat_width + 2))

    height = (mat_height + 2) * tile_size
    width = (mat_width + 2) * tile_size


    self.setFixedSize(width, height)

    self.canvas = Canvas(width, height, mat_height, mat_width)
    self.canvas.redraw()

    self.content = QtWidgets.QLabel()
    self.content.setPixmap(self.canvas)
    self.setCentralWidget(self.content)

    self.updater = UpdateThread()
    self.updater.set_cell_tank.connect(self.set_cell_tank)
    self.updater.set_cell_value.connect(self.set_cell_value)
    self.updater.clear_cell.connect(self.clear_cell)
    self.updater.set_col_number.connect(self.set_col_number)
    self.updater.set_row_number.connect(self.set_row_number)
    self.updater.update.connect(self.update)
    self.updater.close.connect(self.close)
    self.updater.start()

    self.show()

  def closeEvent(self, event):
    self.updater.stop()
    # self.canvas.save("out.png")
    event.accept()

  def set_cell_tank(self, row: int, col: int, ID: int):
    """Indica a que estanque pertenece la celda"""
    self.canvas.aquariums[row][col] = ID

  def set_cell_value(self, row: int, col: int, filled: bool):
    """Pinta una celda para indicar que está llena o vacía"""

    self.canvas.values[row][col] = 1 if filled else -1 

  def clear_cell(self, row: int, col: int):
    """Elimina el color de una celda para indicar que no se sabe si está llena o vacía"""

    self.canvas.values[row][col] = 0

  def set_col_number(self, top: bool, col: int, number: int, color: Tuple[int,int,int] = (0,0,0)):
    """
    Dibuja el número asociado a una columna
    
    Parámetros:
    -----------

    top: bool
      
      Con True se dibujará arriba de la columna, con False abajo

    col: int

      La columna a la cual queremos asociarle este número, de izquierda a derecha, partiendo en 0

    number: int

      El número a asociar a la columna

    colors: Color como tupla (R,G,B), opcional

      Color con que pintar el número. Útil para formular podas. Blanco por defecto.

    """

    qcolor = QtGui.QColor(color[0], color[1], color[2])
    if top:
      self.canvas.top_row[col] = (number, qcolor)
    else:
      self.canvas.bot_row[col] = (number, qcolor)

  def set_row_number(self, left: bool, row: int, number: int, color: Tuple[int,int,int] = (0,0,0)):
    """
    Dibuja el número asociado a una fila
    
    Parámetros:
    -----------

    left: bool
      
      Con True se dibujará a la izquierda de la fila, con False a la derecha    

    row: int

      La fila a la cual queremos asociarle este número, de arriba a abajo, partiendo en 0

    number: int

      El número a asociar a la fila

    colors: Color como tupla (R,G,B), opcional

      Color con que pintar el número. Útil para formular podas. Blanco por defecto.

    """

    qcolor = QtGui.QColor(color[0], color[1], color[2])
    if left:
      self.canvas.left_col[row] = (number, qcolor)
    else:
      self.canvas.right_col[row] = (number, qcolor)

  def update(self):
    """Actualiza el contenido de la ventana para reflejar los cambios"""

    self.canvas.redraw()

    self.content.setPixmap(self.canvas)