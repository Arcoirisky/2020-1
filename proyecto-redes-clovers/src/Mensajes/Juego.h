#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

typedef enum CasType{
  I, O, X, R, P, T, F, E, A, D, Y
} CasType;

typedef enum PowerType{//0,1,2,3
  Nada, Duelo, DobleDice, RobaMeritos
} PowerType;

typedef struct Casilla{
  CasType tipo;
  bool p1;
  bool p2;
  int id;
} Casilla;

typedef struct Tablero{
  int largo;
  Casilla** boxs;
} Tablero;

typedef struct Jugador{
  int socket;
  char nombre[255];
  int meritos;
  int ruzStar;
  PowerType poder1;
  PowerType poder2;
  PowerType poder3;
  int len_Poderes;
  int posicion;
  bool Yadran;
} Jugador;

typedef struct Partida{
  Tablero* board;
  Jugador* Player1;
  Jugador* Player2;
} Partida;

Partida* playerGame;
int iD;

char* traducirPoder(PowerType power);

Jugador* player_init(int socket_player, char* name);
Partida* partida_init(Tablero* tablero, Jugador* Player1, Jugador* Player2);
Tablero* tablero_init(char* path);
void show_board(Partida* partida);
char* new_strcat(const char* tag, const char* data);
char* new_strintcat(const char* tag, int num);
void show_players(Jugador* p1, Jugador* p2);
void show_powers(PowerType power_1, PowerType power_2);

void destroy_Partida(Partida* partida);
int rollSinleDice();
int rollSinleDice();
