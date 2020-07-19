#include "FunTablero.h"


void ejecutarCasilla(CasType type, Partida* partida, int jugador){
    switch (type)
    {
    case I:
      return;
      break;
    case O:
      actualizarMerJug(partida, jugador, 5);
      return;
      break;
    case X:
      actualizarMerJug(partida, jugador, -5);
      return;
      break;
    case R: //
      return cristianStar(partida, jugador);
      break;
    case P: //
      return winPower(partida, jugador);
      break;
    case T: //
      return store(partida, jugador);
      break;
    case F: //
      return superF(partida, jugador);
      break;
    case E: //
      return salidaBox(partida, jugador);
      break;
    case A:
      actualizarMerJug(partida, jugador, 30);
      return;
      break;
    case D: //
      return DuDuDuDueloPower(partida, jugador);
      break;
    case Y: //
      return fondaDonYadran(partida, jugador);
      break;
    default:
      printf("Casilla invalida? why?");
      break;
    }
}

void store(Partida* partida,int jugador){
  Jugador* player = sacarPlayer(partida, jugador, false);
  if(player -> meritos < 10){
    enviarMensaje(player->socket, paquete_init(Message, "No tienes suficiente dinero para comprar"));
    return;
  }
  if(player ->len_Poderes >=3){
    enviarMensaje(player->socket, paquete_init(Message, "No puedes acumular mas poderes"));
    return;
  }
  enviarMensaje(player->socket, paquete_init(AskStore, " "));
  char* msg_in = recivirMensaje(player->socket);
  char* opcion = responderAccion(msg_in, 0);
  free(msg_in);
  if(atoi(opcion) == 4){
    free(opcion);
    enviarMensaje(player->socket, paquete_init(Message, "¡Gracias por comprar!"));
    return;
  }
  enviarPoder(atoi(opcion), jugador, partida);
  free(opcion);
  enviarMensaje(player->socket, paquete_init(Message, "¿Deseas comprar algo mas?"));
  actualizarMerJug(partida,jugador, -10);
  store(partida, jugador);
  return;
}

void salidaBox(Partida* partida,int jugador){
  printf("Casilla E\n");
  Jugador* player = sacarPlayer(partida, jugador, false);
  player->posicion = 0;
  actualizarMerJug(partida,jugador,20);
  actualizarPosJug(partida, jugador, 0);
  mostrarTablerosJugadores(partida);
}

void superF(Partida* partida,int jugador){
  printf("Casilla F\n");
  Jugador* player = sacarPlayer(partida, jugador, false);
  player->posicion = 0;
  actualizarPosJug(partida, jugador, 0);
  mostrarTablerosJugadores(partida);
}

void actualizarRuzStar(Partida* partida, int jugador, int cant){
  Jugador* player = sacarPlayer(partida, jugador, false);
  Jugador* enemy = sacarPlayer(partida, jugador, true);

  char sint[100];
  sprintf(sint, "%i", cant);
  printf("\n");
  if (jugador==1){
      printf("Jugador 1 gano +%i Ruz-Stars\n", cant);
      partida ->Player1->ruzStar += cant;
  }
  else
  {
      printf("Jugador 2 gano +%i Ruz-Stars\n", cant);
      partida ->Player2->ruzStar += cant;
  }

  enviarMensaje(player->socket, paquete_init(AddStarPlayer, sint));
  enviarMensaje(enemy->socket, paquete_init(AddStarEnemy, sint));
}

void cristianStar(Partida* partida,int jugador){
  Jugador* player = sacarPlayer(partida, jugador, false);
  int cont = 0;
  int mer_aux = player -> meritos;
  while(mer_aux >= 20){
    mer_aux -= 20;
    cont++;
  }
  if(cont > 0){
    actualizarMerJug(partida,jugador, -cont*20);
    actualizarRuzStar(partida, jugador, cont);
  }
}

void enviarPoder(PowerType power, int jugador, Partida* partida){
  char Msgoponente[100];
  char Msgjugador[100];
  if(power == DobleDice){
      printf("\nJugador %i gano Dados Dobles\n", jugador);
      sprintf(Msgoponente, "Tu oponente gano Dados Dobles");
      sprintf(Msgjugador, "¡Ganaste Dados Dobles!");
  }
  else{
    if(power == Duelo){
      printf("\nJugador %i gano Duelo\n", jugador);
      sprintf(Msgoponente, "Tu oponente gano Duelo");
      sprintf(Msgjugador, "¡Ganaste Duelo!");
    }
    else{
      if(power == RobaMeritos){
        printf("\nJugador %i gano Roba Meritos\n", jugador);
        sprintf(Msgoponente, "Tu oponente gano Roba Meritos");
        sprintf(Msgjugador, "¡Ganaste Roba Meritos!");
      }
      else{
        return;
      }
    }
  }
  Jugador* player = sacarPlayer(partida, jugador, false);
  player ->len_Poderes++;
  Jugador* enemy = sacarPlayer(partida, jugador, true);
  enviarMensaje(player->socket, paquete_init(Message, Msgjugador));
  enviarMensaje(enemy->socket, paquete_init(Message, Msgoponente));
  
  char poder[100];
  sprintf(poder, "%i",power);

  enviarMensaje(player->socket, paquete_init(AddPower, poder));
  enviarMensaje(enemy->socket, paquete_init(AddPowerEnemy, poder));
}

void winPower(Partida* partida,int jugador){
  Jugador* player = sacarPlayer(partida, jugador, false);
  if (player ->len_Poderes == 3){
    enviarMensaje(player->socket, paquete_init(Message, "No puedes ganar mas poderes"));
    return;
  }
  PowerType new_power = (rand() % 3) + 1;
  
  if (player -> poder1 == Nada){
    enviarPoder(new_power, jugador, partida);
    player -> poder1 = new_power;
    return;
  }
  if (player -> poder2 == Nada){
    enviarPoder(new_power, jugador, partida);
    player -> poder2 = new_power;
    return;
  }
  if (player -> poder3 == Nada){
    enviarPoder(new_power, jugador, partida);
    player -> poder3 = new_power;
    return;
  }
}

void fondaDonYadran(Partida* partida,int jugador){
  if (jugador==1){
    enviarMensaje(partida ->Player1->socket, paquete_init(GoYadranGo, " "));
    partida->Player1->Yadran = true;
  }
  else
  {
    enviarMensaje(partida ->Player2->socket, paquete_init(GoYadranGo, " "));
    partida->Player2->Yadran = true;
  }
}

void anunciarGanador(Partida* partida, int jugador){
  Jugador* player = sacarPlayer(partida, jugador, false);
  Jugador* enemy = sacarPlayer(partida, jugador, true);

  printf("\n");
  if (jugador==1){
    printf("Gana el jugador 1...\n");
    enviarMensaje(partida ->Player1->socket, paquete_init(Win, " "));
    enviarMensaje(partida ->Player2->socket, paquete_init(Lose, " "));
  }
  else
  {
    printf("Gana el jugador 2...\n");
    enviarMensaje(partida ->Player1->socket, paquete_init(Lose, " "));
    enviarMensaje(partida ->Player2->socket, paquete_init(Win, " "));
  }
  enviarMensaje(partida ->Player1->socket, paquete_init(Disconnect, " "));
  enviarMensaje(partida ->Player2->socket, paquete_init(Disconnect, " "));

  destroy_Partida(partida);
}

void somebodyWin(Partida* partida){
  int StarP1 = partida->Player1->ruzStar;
  int StarP2 = partida->Player2->ruzStar;
  if (StarP1 >= 3){// Win Player 1
    anunciarGanador(partida, 1);
    exit(0);
  }

  if (StarP2 >= 3){// Win Player 2
    anunciarGanador(partida, 2);
    exit(0);
  }
}