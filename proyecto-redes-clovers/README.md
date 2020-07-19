# Intengrantes

* Constanza Osorio 13634089
* Jessica Hormazabal 15639150
* Daniel Soto 14206153

# Ejecución

El Makefile en el proyecto se encarga de crear ambos ejecutables (server y client)

```
$ make
$ ./server -i 127.0.0.1 -p 8888
$ ./client -i 127.0.0.1 -p 8888
```

# Supuestos

* Los jugadores son inteligentes y no se equivocan ni se arrepienten de sus decisiones. No estaba explícito en el enunciado el manejo de errores y tal como se mencionó en una issue, dumbclient != dumbuser

* El movimiento manual si bien solo pide movimientos entre el 1 y el 10, dejé libre el número a elección para agilizar la corrección. Así no tienen que jugar N veces para caer en una misma casilla o probar distintos estados en muchos turnos.


# Decisiones de diseño
Se separó el proyecto en 4 partes:

## Cliente

Todo dentro de esta carpeta es usado por el cliente para inicializar la conexión con el servidor. No hace mucho más que eso.

## Servidor

Aquí se encuentra la mayor parte del programa. Intenté separar lo mejor posible las funciones para que quedara más claro. Por lo cual encontrarán 3 pares de archivos .c .h

La mayor parte de los cambios en la partida se realizan en el servidor, una vez este es actualizado cada función envía un mensaje al jugador afectado por estos cambios para poder mantener la información actualizada de estos.

### FunServidor

Aquí se encuentra la mayoria de código que inicializa la conexión principal del programa. 

También existen las funciones principales para que el juego tenga sentido, como lo son actualizar la posición del jugador, la cantidad de méritos y el manejo principal de un turno.

### FunPoderes

Aquí se describen los tres únicos poderes de todo el juego, Duelo, Dados dobles y Robar Meritos. También trabaja el random para cuando se gana un poder y cuando se elimina. 

### FunTablero

Se encarga completamente de la ejecución de las casillas, incluyendo las funciones que revisan si es que se a terminado la partida. 

No hay diferencia en si se a ganado por estrellas o por rendición. Simplemente ganas o pierdes según tus acciones, ya que es explicita la rendición y no puedes rendirte en un turno donde ya has ganado 3 Ruz Stars o más.

## Juego

Aquí se encuentran las estructuras utilizadas durante el proyecto,su inicialización y las funciones que imprimen el tablero + la información de los jugadores.

## Mensajes

Aquí está toda la comunicación entre cliente y servidos. No quise separarlos ya que podría ser demasiado confuso al momento de revisar quién hace qué cosa. 

De todas formas, hay muchos casos en el switch case que pueden reducirse como un simple mensaje, solo que no se me ocurrió hasta muy tarde hacer una llamada que imprimiera un mensaje en específico para los clientes. 

Los paquetes utilizados se dividen en las 3 partes según especifica el enunciado. Aquí tomense su tiempo para leer porque son muchos codigos...

* StartConnection y ConnectionEstablished: Se encargan de la conexión exitosa entre cliente y servidor

*  AskNickname y RetNickname: El servidor pregunta por el nickName a un jugador y lo recibe como respuesta en forma de paquete con RetNickname

* WelcomeName: Despliega el mensaje de bienvenida personalizado

* Disconnect: Llama a los clientes a liberar la memoria y desconectarse

* ShowBoard: Despliega el tablero y la información de los jugadores en sus respectivas ventanas

* ErrBadPkg: En caso de que suceda algún error en el envio de paquetes nis envíe un paquete al menos vacío.

* AskBoard y RetBoard: Pide al cliente el path del tablero y envia este path como respuesta con RetBoard al servidor

* AskOption y RetOption: Pregunta y entrega al servidor la opción del jugador al iniciar un turno (Rendirse, Avanzar, Utilizar poder)

* MovePlayer y MoveEnemy: envia el cambio de posición de un jugador, sea el main o el contrincante.

* GiveId: Permite darle una identificación a los jugadore para si mismos
  
* GivePlayerName: Entrega a cada jugador su propio nombre de usuario

* GiveEnemyName: Entrega a cada jugador el nombre de usuario de su enemigo

* Wait: Solo despliega un mensaje de que el otro jugador está jugando.

* AskMovType: En caso de que el jugador desee Avanzar necesita elegir entre hacerlo de forma manual o aleatoria. Pues, aquí se maneja esa elección.

* AskMovNum: Recibe el numero que desea avanzar manualmente el jugador

* MoreMerPlayer y MoreMerEnemy: Actualiza la cantidad de meritos del jugador main y el enemigo

* AddStarPlayer y AddStarEnemy: Actualiza la cantidad de RuzStars para el jugador y el enemigo

* Win y Lose: Despliegan los mensajes de ganador o perdedor

* GoYadranGo y CryYadranCry: Despliegan los mensajes de cuando caes en una casilla "Posada Don Yadran" y pierdes un turno.

* Message: La señal que envia mensajes de texto y pudo ahorrar muchas señales pero fue implementada tarde. F

* AddPower y AddPowerEnemy: Actualiza los poderes del jugador main y del enemigo en los clientes

* AskStore: Menú de la tienda

* AskPower: Menú de utilización de poderes

* DellPower y DellPowerEnemy: Señales que eliminan los poderes utilizados por el jugador main o el enemigo, según sea el caso.

* AskCachipun: Pide el movimiento del juego Cachipún
