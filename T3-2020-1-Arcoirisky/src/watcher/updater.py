from PyQt5 import QtCore, QtGui

from typing import Tuple

import socket

class UpdateThread(QtCore.QThread):

  set_cell_value = QtCore.pyqtSignal(int, int, bool)
  set_cell_tank  = QtCore.pyqtSignal(int, int, int)
  clear_cell     = QtCore.pyqtSignal(int, int)
  set_col_number = QtCore.pyqtSignal(bool, int, int, object)
  set_row_number = QtCore.pyqtSignal(bool, int, int, object)
  update         = QtCore.pyqtSignal()
  close          = QtCore.pyqtSignal()

  def __init__(self):
    super().__init__()
    self.sock = socket.socket()

    host = socket.gethostname()
    self.sock.connect((host, 12345))

    self.listening = True


  def stop(self):
    if self.sock:
      self.sock.shutdown(socket.SHUT_RD)
    self.listening = False

  def run(self):    
    while self.listening:
      data = self.sock.recv(1024).decode("utf-8")
      
      commands = data.strip().split("\n")      

      for command_data in commands:
        command = command_data.strip().split(" ")
        if command[0] == "V":
          row = int(command[1])
          col = int(command[2])
          val = command[3] == "True"
          self.set_cell_value.emit(row, col, val)
        elif command[0] == "X":
          row = int(command[1])
          col = int(command[2])
          self.clear_cell.emit(row, col)
        elif command[0] == "C":
          top = command[1] == "True"
          row = int(command[2])
          num = int(command[3])
          R = int(command[4])
          G = int(command[5])
          B = int(command[6])
          self.set_col_number.emit(top, row, num, (R,G,B))
        elif command[0] == "R":
          left = command[1] == "True"
          col = int(command[2])
          num = int(command[3])
          R = int(command[4])
          G = int(command[5])
          B = int(command[6])
          self.set_row_number.emit(left, col, num, (R,G,B))
        elif command[0] == "U":
          self.update.emit()
        elif command[0] == "T":
          row = int(command[1])
          col = int(command[2])
          ID  = int(command[3])
          self.set_cell_tank.emit(row, col, ID)
        else:
          self.sock.close()
          self.sock = None
          self.close.emit()
          self.listening = False