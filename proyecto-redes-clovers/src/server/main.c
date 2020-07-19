#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "FunServidor.h"
#include "../Mensajes/Mensajes.h"
#include "../Mensajes/Juego.h"


/** variables globales */
int PORT;
char IP[100];

int cantPlayers = 0;

/** main */
int main(int argc, char *argv[])
{
  if (argc != 5 || strcmp(argv[1], "-i") != 0 || strcmp(argv[3], "-p") != 0) {
    printf("Uso correcto: ./server -i <ip_address> -p <tcp-port> -l");
    return 1;
  }
  
  strcpy(IP, argv[2]);
  PORT = atoi(argv[4]);
  
  // Conecta servidores, toma los jugadores y todo
  Partida* partida = inicializarPrograma(IP, PORT);
  show_board(partida);
  show_players(partida -> Player1, partida -> Player2);

  mostrarTablerosJugadores(partida);

  while(1){
    startturn(partida, 1); //En construcción
    startturn(partida, 2); //En construcción
  }
  
  /* LIBERACION DE MEMORIA */
  enviarMensaje(partida ->Player1->socket, paquete_init(Disconnect, " "));
  enviarMensaje(partida ->Player2->socket, paquete_init(Disconnect, " "));

  free(partida -> Player1);
  free(partida -> Player2);
  for (int i = 0; i < partida -> board -> largo; ++i)
  {
    free(partida -> board ->boxs[i]);
  }

  free(partida ->board->boxs);
  free(partida ->board);

  free(partida);

  return 0;
}
