from PyQt5 import QtGui
from PyQt5 import QtCore
from PyQt5 import QtWidgets

import numpy
import math

from typing import List

class Colors():

  background   = QtGui.QColor(244, 244, 244) #QtGui.QColor(5, 10, 40)
  foreground   = QtGui.QColor(0, 0, 0)
  black_matter = QtGui.QColor(133, 102, 170) # QtGui.QColor(142, 148, 242)
  light_matter = QtGui.QColor(147, 198, 197) # QtGui.QColor(243, 185, 252)

class Canvas(QtGui.QPixmap):
  """
  Imagen que contiene la visualización del problema
  
  Parámetros:
  -----------

  width: int

    Ancho de la imagen, en pixeles

  height: int

    Alto de la imagen, en pixeles

  mat_height: int

    Alto de la matriz, en celdas

  mat_width: int

    Ancho de la matriz, en celdas
  """
  
  def __init__(self, width: int, height: int, mat_height: int, mat_width: int):
    super().__init__(width, height)

    # Acuarios
    self.aquariums = [[-1 for col in range(mat_width)] for row in range(mat_height)]   

    # Tamaño de cada celda
    self.tile_size = height / (mat_height + 2)

    # self.draw_borders()

    # Números arriba de la matriz
    self.top_row   = [(0, Colors.foreground) for _ in range(mat_width)]
    # Números abajo de la matriz
    self.bot_row   = [(0, Colors.foreground) for _ in range(mat_width)]
    # Números a la izquierda de la matriz
    self.left_col  = [(0, Colors.foreground) for _ in range(mat_height)]
    # Números a la derecha de la matriz
    self.right_col = [(0, Colors.foreground) for _ in range(mat_height)]

    # Matriz de valores. 0 = sin asignar
    self.values    = [[0 for _ in range(mat_width)]  for _ in range(mat_height)] 


    # TODO: self.board_layout : QtGui.QPicture 
    pass

  def draw_borders(self):

    mat_height = len(self.aquariums)
    mat_width = len(self.aquariums[0])

    painter = QtGui.QPainter()

    painter.begin(self)

    painter.setRenderHint(QtGui.QPainter.Antialiasing)

    light_border = QtGui.QPen(
      Colors.foreground, 
      self.tile_size / 500)

    

    painter.setPen(light_border)

    for row in range(mat_height + 1):
      sx = self.tile_size
      sy = (row + 1) * self.tile_size
      fx = (mat_width + 1) * self.tile_size
      fy = sy

      painter.drawLine(sx, sy, fx, fy)

    for col in range(mat_width + 1):
      sx = (col + 1) * self.tile_size
      sy = self.tile_size
      fx = sx
      fy = (mat_height + 1) * self.tile_size

      painter.drawLine(sx, sy, fx, fy)

    thick_border = QtGui.QPen(
      Colors.foreground, 
      int(self.tile_size / 5), 
      join=QtCore.Qt.MiterJoin)

    painter.setPen(thick_border)

    painter.drawRect(self.tile_size, self.tile_size, mat_width * self.tile_size, mat_height * self.tile_size)
    
    # Bordes gruesos (horizontales)
    for row in range(mat_height):
      for col in range(mat_width - 1):
        if self.aquariums[row][col] != self.aquariums[row][col + 1]:
          sx = (col + 2) * self.tile_size
          sy = (row + 1) * self.tile_size
          fx = sx
          fy = (row + 2) * self.tile_size

          painter.drawLine(sx, sy, fx, fy)
    
    for col in range(mat_width):
      for row in range(mat_height - 1):
        if self.aquariums[row][col] != self.aquariums[row + 1][col]:
          sx = (col + 1) * self.tile_size
          sy = (row + 2) * self.tile_size
          fx = (col + 2) * self.tile_size
          fy = sy

          painter.drawLine(sx, sy, fx, fy)
    
    thick_border = QtGui.QPen(
      Colors.background, 
      int(self.tile_size / 10), 
      join=QtCore.Qt.MiterJoin)

    painter.setPen(thick_border)

    painter.drawRect(self.tile_size, self.tile_size, mat_width * self.tile_size, mat_height * self.tile_size)
    
    for row in range(mat_height + 2):
      for col in range(mat_width + 2):
        if row == 0 or row == mat_height + 1 or col == 0 or col == mat_width + 1:
          x = (col) * self.tile_size
          y = (row) * self.tile_size
          painter.fillRect(
            x, 
            y, 
            self.tile_size, 
            self.tile_size,
            Colors.background
          )

    # Bordes gruesos (horizontales)
    for row in range(mat_height):
      for col in range(mat_width - 1):
        if self.aquariums[row][col] != self.aquariums[row][col + 1]:
          sx = (col + 2) * self.tile_size
          sy = (row + 1) * self.tile_size
          fx = sx
          fy = (row + 2) * self.tile_size

          painter.drawLine(sx, sy, fx, fy)
    
    for col in range(mat_width):
      for row in range(mat_height - 1):
        if self.aquariums[row][col] != self.aquariums[row + 1][col]:
          sx = (col + 1) * self.tile_size
          sy = (row + 2) * self.tile_size
          fx = (col + 2) * self.tile_size
          fy = sy

          painter.drawLine(sx, sy, fx, fy)

    painter.end()

  def draw_col_numbers(self):
    """
    Dibuja los números asociados a las columnas 
    """
    painter = QtGui.QPainter()

    painter.begin(self)

    painter.setFont(QtGui.QFont("monospace", pointSize=self.tile_size * 0.4))
    painter.setRenderHint(QtGui.QPainter.Antialiasing)
    mat_height = len(self.aquariums)
    mat_width = len(self.aquariums[0])

    sy = 0
    for col in range(mat_width):
      number = self.top_row[col][0]
      qcolor = self.top_row[col][1]

      painter.setPen(qcolor)

      sx = (col + 1) * self.tile_size

      painter.drawText(
        sx, 
        sy, 
        self.tile_size,
        self.tile_size,
        QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter,
        str(number)
      )

    sy = (mat_height + 1) * self.tile_size
    for col in range(mat_width):
      number = self.bot_row[col][0]
      qcolor = self.bot_row[col][1]

      painter.setPen(qcolor)

      sx = (col + 1) * self.tile_size

      painter.drawText(
        sx, 
        sy, 
        self.tile_size,
        self.tile_size,
        QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter,
        str(number)
      )

    painter.end()

  def draw_row_numbers(self):
    """
    Dibuja los números asociados a las filas 
    """
    painter = QtGui.QPainter()

    painter.begin(self)

    painter.setFont(QtGui.QFont("monospace", pointSize=self.tile_size * 0.4))
    painter.setRenderHint(QtGui.QPainter.Antialiasing)
    mat_height = len(self.aquariums)
    mat_width = len(self.aquariums[0])

    # Dibujamos los numeros de la izquierda
    sx = 0
    for row in range(mat_height):
      number = self.left_col[row][0]
      qcolor = self.left_col[row][1]

      painter.setPen(qcolor)

      sy = (row + 1) * self.tile_size

      painter.drawText(
        sx, 
        sy, 
        self.tile_size,
        self.tile_size,
        QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter,
        str(number)
      )

    # Dibujamos los numeros de la derecha
    sx = (mat_width + 1) * self.tile_size
    for row in range(mat_height):
      number = self.right_col[row][0]
      qcolor = self.right_col[row][1]

      painter.setPen(qcolor)

      sy = (row + 1) * self.tile_size

      painter.drawText(
        sx, 
        sy, 
        self.tile_size,
        self.tile_size,
        QtCore.Qt.AlignHCenter | QtCore.Qt.AlignVCenter,
        str(number)
      )

    painter.end()


  def redraw(self):
    
    self.fill(Colors.background)

    mat_height = len(self.aquariums)
    mat_width = len(self.aquariums[0])

    painter = QtGui.QPainter()

    painter.begin(self)

    painter.setRenderHint(QtGui.QPainter.Antialiasing)

    for row in range(mat_height):
      for col in range(mat_width):
        if self.values[row][col] == 1:
          x = (col + 1) * self.tile_size
          y = (row + 1) * self.tile_size
          painter.fillRect(
            x, 
            y, 
            self.tile_size, 
            self.tile_size,
            Colors.black_matter
          )
        elif self.values[row][col] == -1:
          x = (col + 1) * self.tile_size
          y = (row + 1) * self.tile_size
          painter.fillRect(
            x, 
            y, 
            self.tile_size, 
            self.tile_size,
            Colors.light_matter
          )    

    painter.end()

    self.draw_borders()
    self.draw_row_numbers()
    self.draw_col_numbers()


   

