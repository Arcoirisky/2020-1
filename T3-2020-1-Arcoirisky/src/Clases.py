from watcher.watcher import Watcher
import sys

class Casilla:

  def __init__ (self, fila, col):
    self.x = fila
    self.y = col
    self.lleno = False
  
class Estanque:

  def __init__ (self, id):
    self.id = id
    self.n = 0
    self. inicio = 0
    self.coord = {}
    self.pase = False
    self.lleno = False

  def agregarCasilla(self, x, y, nivel):
    if nivel in self.coord.keys():
      self.coord[nivel].append(Casilla(x,y))
    else:
      self.coord[nivel] = [Casilla(x,y)]
      self.n += 1
  
  def llenarNivel(self):
    if self.n > 0:
      self.n -= 1
      nivel = self.n + self.inicio
      for elem in self.coord[nivel]:
        elem.lleno = True
      return len(self.coord[nivel])
    else:
      return 0
  
  def vaciarNivel(self):
    if self.n + self.inicio in self.coord.keys():
      nivel = self.n + self.inicio
      for elem in self.coord[nivel]:
        elem.lleno = False
      output = len(self.coord[nivel])
      self.n += 1
      return output
    else:
      return 0

class Tablero:
  def __init__(self, alto, ancho, n):
    self.alto = alto
    self.ancho = ancho
    self.top_rules = []
    self.bot_rules = []
    self.left_rules = []
    self.rigth_rules = []
    self.estanques = {}
    self.cantEstanques = n
    self.unassigned = [i for i in range(n)]
    self.llenables = [i for i in range(self.cantEstanques)]

  def agregarEstanque(self, id):
    self.estanques[id] = Estanque(id)
  
  def nivelEstanques(self):
    print(self.estanques.keys())
    for key in self.estanques.keys():
      item = self.estanques[key]
      print("Estanque {}: Niveles {}, inicio {}".format(key, item.n, item.inicio))

  def llenarEstanque(self, id):
    estanque = self.estanques[id]
    nivel = estanque.n - 1 + estanque.inicio
    #print("llenables",self.llenables)
    #if id not in self.llenables:
    #  return None
    
    x_i = estanque.coord[nivel][0].x
    y_i = estanque.coord[nivel][0].y
    total = estanque.llenarNivel()
    
    if estanque.n <= 0:
      while(id in self.llenables):
        self.llenables.remove(id)

    self.left_rules[x_i] -= total
    for i in range(total):
      self.top_rules[y_i+i] -= 1

  def vaciarEstanque(self, id):
    estanque = self.estanques[id]
    nivel = estanque.n + estanque.inicio
    if nivel not in estanque.coord.keys():
      return None
    
    #if id not in self.llenables:
    #  self.llenables.append(id)

    x_i = estanque.coord[nivel][0].x
    y_i = estanque.coord[nivel][0].y
    total = estanque.vaciarNivel()

    self.left_rules[x_i] += total
    for i in range(total):
      self.top_rules[y_i+i] += 1

  def chequearBlancaRestricciones(self):
    for i in range(len(self.bot_rules)):
      if self.bot_rules[i] < 0:
        return False
    for j in range(len(self.rigth_rules)):
      if self.rigth_rules[j]<0:
        return False
    return True

  def chequearRestricciones(self):
    # if rompe reglas de arriba -> False
    #print("REVISAR")
    for i in range(len(self.top_rules)):
      if self.top_rules[i] < 0:
          #print("ROMPIO REGRA TOP")
          return False
    # if rompe reglas del lado -> False
    for j in range(len(self.left_rules)):
      if self.left_rules[j] < 0:
          #print("ROMPIO REGRA LEFT")
          return False
    # No rompe ninguna regla -> true
    return True

  def chequearSolucion(self):
  
    for i in range(len(self.top_rules)):
      if self.top_rules[i] > 0:
          return False
    # if rompe reglas del lado -> False
    for j in range(len(self.left_rules)):
      if self.left_rules[i] > 0:
          return False
    # No rompe ninguna regla -> true
    return True

  def imprimitEstado(self):
    matrix = [[[] for j  in range(self.ancho)] for i in range(self.alto)]
    for key in self.estanques.keys():
      for key_casilla in self.estanques[key].coord.keys():
        for casilla in self.estanques[key].coord[key_casilla]:
          x = casilla.x
          y = casilla.y
          matrix[x][y] = 1 if not casilla.lleno else 0
    
    aux = " "
    for elem in self.top_rules:
      aux += "{}  ".format(elem)
    print(aux)
    cont = 0
    for elem in matrix:
      print(elem, self.left_rules[cont])
      cont+=1

  def imprimirTablero(self):
    matrix = [[[] for j  in range(self.ancho)] for i in range(self.alto)]
    for key in self.estanques.keys():
      for key_casilla in self.estanques[key].coord.keys():
        for casilla in self.estanques[key].coord[key_casilla]:
          x = casilla.x
          y = casilla.y
          matrix[x][y] = self.estanques[key].id
    
    aux = " "
    for elem in self.top_rules:
      aux += "{}  ".format(elem)
    print(aux)
    cont = 0
    for elem in matrix:
      print(elem, self.left_rules[cont])
      cont+=1

  def funcionar(self, id):
    #estanque = self.estanques[id]
    #print("NIVEL ESTANQUE {}: {} e inivio {}".format(id,estanque.n, estanque.inicio))
    if id not in self.llenables:
      return False
    else:
      self.llenarEstanque(id)
      if self.chequearRestricciones():
        return True
      self.vaciarEstanque(id)
      if id not in self.llenables:
        self.llenables.append(id)
      return False

  def choose_unnasigned_variable(self):
    return self.llenables.pop(0)

  def is_solvable(self):
    if self.chequearSolucion():
      print("WIIIIIIIIIIIIIII")
      return True
    
    #self.imprimitEstado()
    #input("eeeh help")
    id = self.choose_unnasigned_variable()
    for value in Board.unassigned:
      #No estoy pasando por todos los caminos
      if self.funcionar(value):
        if self.is_solvable():
          return True
      
      if self.estanques[id].n > 0 and id not in self.llenables:
        self.llenables.append(id)
      else:
        if id not in self.llenables:
          self.vaciarEstanque(id)
          self.llenables.append(id)
    return False

        

def cargarBoard(inputFile):
  with open(inputFile,"r") as data:
    alto = int(data.readline().strip())
    ancho = int(data.readline().strip())
    
    Board = Tablero(alto, ancho, 0)
    # Fila superior de números: cuanta materia oscura debe tener cada columna
    Board.top_rules = [int(x) for x in data.readline().strip().split(" ")]  
    # Columna izquierda de números: cuanta materia oscura debe tener cada fila
    Board.left_rules  = [int(x) for x in data.readline().strip().split(" ")]
    #Fila inferior
    Board.bot_rules = [alto - x for x in Board.top_rules]
    Board.rigth_rules = [ancho - x for x in Board.left_rules]

    niveles = int(data.readline().strip())
    Board.cantEstanques = niveles
    Board.unassigned = [i for i in range(niveles)]

    for n in range(niveles):
      Board.agregarEstanque(n)
      Board.llenables.append(n)

    n_fila = 0
    while(alto):
      fila = [int(x) for x in data.readline().strip().split(" ")]
      for i in range(len(fila)):
        if(not Board.estanques[fila[i]].coord.keys()):
          Board.estanques[fila[i]].inicio = n_fila
        Board.estanques[fila[i]].agregarCasilla(n_fila, i, n_fila)
      n_fila += 1 
      alto -= 1
  return Board

def vaciarBoard(Board):
  for key in Board.estanques.keys():
    for n in range(Board.alto):
      Board.vaciarEstanque(key)


if __name__ == "__main__":
  print("probando Clases.py\n")
  input_file = sys.argv[1]

  Board = cargarBoard(input_file)
  Board.imprimirTablero()
  print("")
  #llenarBoard(Board)
  Board.nivelEstanques()
  #vaciarBoard(Board)
  Board.llenables = [i for i in range(Board.cantEstanques)]
  Board.is_solvable()

    

  #recursion3(Board,[],0,[[elem for elem in range(Board.cantEstanques)]])
  #recursion2(Board, 0, [i for i in range(Board.cantEstanques)],[],0)
  #recursion(Board, 0,[i for i in range(Board.cantEstanques)])
  Board.imprimitEstado()
