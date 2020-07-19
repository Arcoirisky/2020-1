#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "FunServidor.h"

void escogerPoder(Partida* partida, int jugador);

void DuDuDuDueloPower(Partida* partida,int jugador);
void DadosDoblesPower(Partida* partida,int jugador);
void RobaMeritosPower(Partida* partida,int jugador);

