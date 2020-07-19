import sys
import subprocess
import signal
import socket

from time import sleep

from typing import List, Tuple

class Watcher():
  """Clase estática encargada de manejar la interacción con la interfaz gráfica"""

  # Canal de comunicación con la ventana
  connection = None

  @staticmethod
  def open(height: int, width: int, max_size: int=640):
    """
    Abre la ventana que visualiza el problema
    
    Parámetros:
    -----------

    height: int

      Indica la altura del puzzle en celdas

    width: int

      Indica el ancho del puzzle en celdas
      
    max_size: int, opcional

      Ancho y alto máximo que puede tener esta ventana en píxeles
    """
    if not Watcher.connection:
      subprocess.Popen(
        [sys.executable, "src/watcher/main.py", str(height), str(width), str(max_size)], 
      )

      s = socket.socket()
      host = socket.gethostname()
      port = 12345

      s.bind((host,port))
      s.listen()
      
      Watcher.connection, _ = s.accept()      

      signal.signal(signal.SIGINT, Watcher._handle_interrupt)

  @staticmethod
  def set_cell_tank(row: int, col: int, ID: int):
    """
    Indica el estanque al que pertenece una celda

    Parámetros:
    -----------

    row: int

      Fila en la que vive la celda

    col: int 

      Columna en la que vive la celda

    ID: int

      Identificador del estanque.
          
    """    
    Watcher._send_message(f"T {row} {col} {ID}\n")

  @staticmethod
  def set_cell_matter(row: int, col: int, dark: bool):
    """
    Pinta una celda para indicar si está llena o vacía

    Parámetros:
    -----------

    row: int

      Fila en la que vive la celda

    col: int 

      Columna en la que vive la celda

    dark: bool

      True si la celda contiene materia oscura, False si contiene materia clara
          
    """    
    Watcher._send_message(f"V {row} {col} {dark}\n")

  @staticmethod
  def clear_cell(row: int, col: int):
    """

    Elimina el color de una celda para indicar que no se sabe el tipo de materia que contiene

    Parámetros:
    -----------

    row: int

      Fila en la que vive la celda

    col: int 

      Columna en la que vive la celda
    
    """
    Watcher._send_message(f"X {row} {col}\n")
    
  @staticmethod
  def set_col_number(top: bool, col: int, number: int, color: Tuple[int,int,int] = (0,0,0)):
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
    Watcher._send_message(f"C {top} {col} {number} {color[0]} {color[1]} {color[2]}\n")
    
  @staticmethod
  def set_row_number(left: bool, row: int, number: int, color: Tuple[int,int,int] = (0,0,0)):
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
    Watcher._send_message(f"R {left} {row} {number} {color[0]} {color[1]} {color[2]}\n")
    
  @staticmethod
  def update():
    """Actualiza el contenido de la ventana para reflejar los cambios"""
    Watcher._send_message(f"U\n")

  @staticmethod
  def close():
    """Cierra la ventana"""    
    Watcher._send_message(f"Z\n")
    if Watcher.connection:
      Watcher.connection.close()
      Watcher.connection = None

  @staticmethod
  def _handle_interrupt(sig, frame):
    """Intercepta Ctrl+C y se asegura de cerrar la ventana primero"""
    Watcher.close()
    sys.exit(0)

  @staticmethod
  def _send_message(message: str):
    if Watcher.connection:
      try:
        Watcher.connection.send(message.encode("utf-8"))
        sleep(0.01)
      except ConnectionResetError:
        Watcher.connection = None
