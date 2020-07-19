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
    self.n_max = 0
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
      self.n_max += 1
  
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
    self.camino = []

  def agregarEstanque(self, id):
    self.estanques[id] = Estanque(id)
  
  def nivelEstanques(self):
    print(self.estanques.keys())
    for key in self.estanques.keys():
      item = self.estanques[key]
      print("Estanque {}: Niveles {}, inicio {}".format(key, item.n, item.inicio))

  def llenarEstanque(self, id):
    estanque = self.estanques[id]
    total = estanque.llenarNivel()

    if estanque.n < 0 or total == 0: #n es la cantidad de niveles disponibles
      return False

    nivel = estanque.n + estanque.inicio
    x_i = estanque.coord[nivel][0].x
    y_i = estanque.coord[nivel][0].y
    
    self.left_rules[x_i] -= total
    for i in range(total):
      self.top_rules[y_i+i] -= 1
    
    return True

  def vaciarEstanque(self, id):
    estanque = self.estanques[id]
    total = estanque.vaciarNivel()
    #print("total",total, estanque.n)
    if estanque.n > estanque.n_max:
      return False

    nivel = estanque.n + estanque.inicio - 1
    x_i = estanque.coord[nivel][0].x
    y_i = estanque.coord[nivel][0].y
    
    #print("nivel: {} - X: {} - Y: {}".format(nivel, x_i, y_i))
    self.left_rules[x_i] += total
    for i in range(total):
      self.top_rules[y_i+i] += 1
    #Board.imprimitEstado()
    return True

  def chequearRestricciones(self):
    for i in range(len(self.top_rules)):
      if self.top_rules[i] < 0:
          return False
    for j in range(len(self.left_rules)):
      if self.left_rules[j] < 0:
          return False
    return True

  def chequearSolucion(self):  
    for i in range(len(self.top_rules)):
      if self.top_rules[i] > 0:
          return False
    for j in range(len(self.left_rules)):
      if self.left_rules[i] > 0:
          return False
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

  def llenar_nivel(self, id): # Arreglar
    aux = []
    #Para todos los estanques
    puedo = self.llenarEstanque(id)
    if puedo:
      #Si puedo llenar entonces reviso restricciones
      if self.chequearRestricciones():
        #Si cumplo restricciones entonces meto en aux
        self.camino.append(id)
        aux.append(id)
      else:
        #No cumplo restricciones y retrocedo el movimiento
        self.vaciarEstanque(id)
    #No pude llenar porque no tenia más espacio asi que continuo con el siguiente
    return aux

  def choose_unnasigned_variable(self):
    return self.llenables.pop(0)

  def is_solvable(self, desde = 0):
    if self.chequearSolucion():
      #print("WIIIIIIIIIIIIIII")
      return True
    
    elems = [elem for elem in range(self.cantEstanques)]
    while(elems):
      #print("camino {}, {}".format(self.camino, elems))
      #input("enter2")
      #No estoy pasando por todos los camino
      id = elems.pop(0)
      #print(self.estanques[id].n)
      #if id not in self.camino:
      nivel = self.llenar_nivel(id)
      #print("NIVEL: ", nivel)
      if nivel:
        if self.is_solvable():
          return True
        else:
          sale = self.camino.pop()
          self.vaciarEstanque(sale)

    #print("BOOOOOOOOOOOOOOOOOOOOOOO")
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
    for n in range(Board.estanques[key].n_max):
      input("estanque {}".format(key))
      Board.vaciarEstanque(key)

def llenarBoard(Board):
  for key in Board.estanques.keys():
    for n in range(Board.estanques[key].n_max):
      Board.llenarEstanque(key)

if __name__ == "__main__":
  if len(sys.argv) != 3:
    print(f"Modo de uso: python3.6+ {sys.argv[0]} input_file output_file")
    print("Donde")
    print("\tinput_file es un archivo del test a resolver")
    print("\toutput_file es el archivo donde el programa guardará el output")
    sys.exit(1)

  input_file = sys.argv[1]
  output_file = sys.argv[2]

  Board = cargarBoard(input_file)
  #Board.imprimirTablero()
  #print("")
  #Board.nivelEstanques()
  #llenarBoard(Board)
  #vaciarBoard(Board)
  resultado = Board.is_solvable()
  if resultado:
    #print("Guardando...")
    with open(output_file,'w') as data:
      matrix = [[[] for j  in range(Board.ancho)] for i in range(Board.alto)]
      for key in Board.estanques.keys():
        for key_casilla in Board.estanques[key].coord.keys():
          for casilla in Board.estanques[key].coord[key_casilla]:
            x = casilla.x
            y = casilla.y
            matrix[x][y] = 1 if not casilla.lleno else 0
      
      for elem in matrix:
        for num in elem:
          data.write("{} ".format(num))
        data.write("\n")

  #Board.imprimitEstado()