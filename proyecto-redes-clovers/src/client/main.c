#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "FunCliente.h"

/** variables globales */
int PORT;
char IP[100];
extern Tablero* boardGame;
extern int iD;

/** main */
int main(int argc, char *argv[])
{
  if (argc != 5 || strcmp(argv[1], "-i") != 0 || strcmp(argv[3], "-p") != 0) {
    printf("Uso correcto: ./client -i <ip_address> -p <tcp-port> -l");
    return 1;
  }
  strcpy(IP, argv[2]);
  PORT = atoi(argv[4]);
  
  printf("Inicializando cliente...\n");
  int socket = init_cliente(IP, PORT);
  printf("... ¡Cliente inicializado! ...\n");

  char* msg_in = NULL;
  char* msg_out = NULL;
  char msg[] = " ";
  msg_out = paquete_init(StartConnection, msg);
  enviarMensaje(socket, msg_out);

  while(true){
    msg_in = recivirMensaje(socket);
    if(msg_in[0]==Disconnect){
      free(msg_in);
      destroy_Partida(playerGame);
      disconnect();
    }
    
    msg_out = responderAccion(msg_in, 0);
    if(msg_in[0]==RetBoard){
      char* boardName = responderAccion(msg_in, 0);
  
      Tablero* boardGame = tablero_init(boardName);
      //Crea una partida con jugadores incomunicados
      playerGame = partida_init(boardGame, player_init(0, "Player1"), player_init(0, "Player2"));
      free(boardName);
      free(msg_out);
    }
    else{
      if(msg_out){
        enviarMensaje(socket, msg_out);
      }
    }
    if (msg_in){
      free(msg_in);
    }
  }

  return 0;
}
