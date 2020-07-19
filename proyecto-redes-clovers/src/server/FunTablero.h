#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "FunServidor.h"
#include "FunPoderes.h"

void ejecutarCasilla(CasType type, Partida* partida, int jugador);

void winPower(Partida* partida, int jugador);
void cristianStar(Partida* partida,int jugador);
void store(Partida* partida,int jugador);
void superF(Partida* partida,int jugador);
void salidaBox(Partida* partida,int jugador);
void fondaDonYadran(Partida* partida,int jugador);

void somebodyWin(Partida* partida);
void anunciarGanador(Partida* partida, int jugador);
void enviarPoder(PowerType power, int jugador, Partida* partida);

