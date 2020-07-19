#include "FunPoderes.h"

void eliminarPoderJug(int poder, Jugador* player){
  if(poder == 1){
    player -> poder1 = Nada;
    player->len_Poderes--;
    return;
  }
  if(poder == 2){
    player -> poder2 = Nada;
    player->len_Poderes--;
    return;
  }
  if(poder == 3){
    player -> poder3 = Nada;
    player->len_Poderes--;
    return;
  }
}

void escogerPoder(Partida* partida, int jugador){
  Jugador* player = sacarPlayer(partida, jugador, false);
  Jugador* enemy = sacarPlayer(partida, jugador, true);
  if (player->len_Poderes==0){
    enviarMensaje(player->socket, paquete_init(Message, "¡No tienes poderes!"));
    return;
  }
  enviarMensaje(player->socket, paquete_init(AskPower, " "));
  char* msg_in = recivirMensaje(player->socket);
  char* opcion = responderAccion(msg_in, 0);
  free(msg_in);
  printf("opcion: %s\n", opcion);
  
  char dell[100];
  sprintf(dell, "%c",opcion[2]);
  
  char sput[100];
  sprintf(sput, "%c",opcion[0]);
  free(opcion);

  switch(atoi(sput))
    {
        case 1:
            DuDuDuDueloPower(partida, jugador);
            enviarMensaje(player->socket, paquete_init(DellPower, dell));
            enviarMensaje(enemy->socket, paquete_init(DellPowerEnemy, dell));
            eliminarPoderJug(atoi(dell),player);          
            break;
        case 2:
            DadosDoblesPower(partida, jugador);
            enviarMensaje(player->socket, paquete_init(DellPower, dell));
            enviarMensaje(enemy->socket, paquete_init(DellPowerEnemy, dell));
            eliminarPoderJug(atoi(dell),player);
            break;
        case 3:
            RobaMeritosPower(partida, jugador);
            enviarMensaje(player->socket, paquete_init(DellPower, dell));
            enviarMensaje(enemy->socket, paquete_init(DellPowerEnemy, dell));
            eliminarPoderJug(atoi(dell),player);
            break;
        default:
            printf("No ocupo ningun poder...\n");
            return;
    }
}

int cambioSigno(int robados){
  if(robados < 0){
    return -robados; //Lo vuelve positivo
  }
  return robados;
}

void DuDuDuDueloPower(Partida* partida,int jugador){
  Jugador* player = sacarPlayer(partida, jugador, false);
  Jugador* enemy = sacarPlayer(partida, jugador, true);

  enviarMensaje(player->socket, paquete_init(AskCachipun, " "));
  enviarMensaje(enemy->socket, paquete_init(AskCachipun, " "));

  char* msg_in1 = recivirMensaje(player->socket);
  char* opcion1 = responderAccion(msg_in1, 0);
  free(msg_in1);

  char* msg_in2 = recivirMensaje(enemy->socket);
  char* opcion2 = responderAccion(msg_in2, 0);
  free(msg_in2);

  // piedra = 1; papel = 2; tijeras = 3
  int robados;
  if(atoi(opcion1) == atoi(opcion2)){
    actualizarMerJug(partida, jugador, 5); //se actualiza los meritos del jugador
    actualizarMerJug(partida, jugador%2 + 1, 5); //se actualiza los meritos del rival
  }

  else if(atoi(opcion1) == 1 && atoi(opcion2) == 2){
    robados = actualizarMerJug(partida, jugador, -10); //se actualiza los meritos del jugador
    actualizarMerJug(partida, jugador%2 + 1, cambioSigno(robados)); //se actualiza los meritos del rival
  }

  else if(atoi(opcion1) == 1 && atoi(opcion2) == 3){
    robados = actualizarMerJug(partida, jugador%2 + 1, -10); //se actualiza los meritos del rival
    actualizarMerJug(partida, jugador, cambioSigno(robados)); //se actualiza los meritos del jugador
    
  }

  else if(atoi(opcion1) == 2 && atoi(opcion2) == 1){
    robados = actualizarMerJug(partida, jugador%2 + 1, -10); //se actualiza los meritos del rival
    actualizarMerJug(partida, jugador, cambioSigno(robados)); //se actualiza los meritos del jugador
    
  }

  else if(atoi(opcion1) == 2 && atoi(opcion2) == 3){
    robados = actualizarMerJug(partida, jugador, -10); //se actualiza los meritos del jugador
    actualizarMerJug(partida, jugador%2 + 1, cambioSigno(robados)); //se actualiza los meritos del rival
  }

  else if(atoi(opcion1) == 3 && atoi(opcion2) == 1){
    robados = actualizarMerJug(partida, jugador, -10); //se actualiza los meritos del jugador
    actualizarMerJug(partida, jugador%2 + 1, cambioSigno(robados)); //se actualiza los meritos del rival
  }

  else if(atoi(opcion1) == 3 && atoi(opcion2) == 2){
    robados = actualizarMerJug(partida, jugador%2 + 1, -10); //se actualiza los meritos del rival
    actualizarMerJug(partida, jugador, cambioSigno(robados)); //se actualiza los meritos del jugador
  }
  free(opcion1);
  free(opcion2);
}

void DadosDoblesPower(Partida* partida,int jugador){
  Jugador* player = sacarPlayer(partida, jugador, false);
  enviarMensaje(player->socket, paquete_init(Message, "--> ¡Utilizando Dados Dobles!"));
  int movimiento = rollSinleDice() + rollSinleDice();

  div_t posicion_final = div(player->posicion + movimiento, partida->board->largo);
  player->posicion = posicion_final.rem;
  
  if(posicion_final.quot > 0){
      printf("+20 meritos para el jugador %i\n", jugador);
      actualizarMerJug(partida,jugador,20);
  }
  actualizarPosJug(partida, jugador, posicion_final.rem);
  mostrarTablerosJugadores(partida);
  Casilla* box = partida->board->boxs[posicion_final.rem];

  ejecutarCasilla(box->tipo, partida, jugador);
  if(partida->Player1->posicion == partida->Player2->posicion && box->tipo == D){
        ejecutarCasilla(box->tipo, partida, jugador);
    } // este if es cuando despues de una movida ambos jugadores estan en la misma casilla de duelo (hay 2 duelos en total)
  show_board(partida);

  somebodyWin(partida);
}

void RobaMeritosPower(Partida* partida,int jugador){
  int cant = rollSinleDice();
  int robados;
  if(jugador == 1){
    robados = actualizarMerJug(partida, 2, -cant);
    actualizarMerJug(partida, 1, cambioSigno(robados));
  }
  else{
    robados = actualizarMerJug(partida, 1, -cant);
    actualizarMerJug(partida, 2, cambioSigno(robados));
  }
  printf("robados %i\n", cambioSigno(robados));

}