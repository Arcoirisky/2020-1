#pragma once

#include "../Mensajes/Mensajes.h"
#include "../Mensajes/Juego.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netdb.h>

#include "FunTablero.h"
#include "FunPoderes.h"

int* server_init(char* ip, int port);
Partida* inicializarPrograma(char* ip, int port);

void mostrarTablerosJugadores(Partida* partida);
void startturn(Partida* partida, int jugador); //En progreso

void actualizarPosJug(Partida* partida, int jugador, int pos);
int actualizarMerJug(Partida* partida, int jugador, int cant);
void avanzar(Partida* partida, int jugador);
Jugador* sacarPlayer(Partida* partida, int jugador, bool enemy);