#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdbool.h>

#include "Juego.h"

typedef enum msgType
{
  StartConnection,
  ConnectionEstablished,
  AskNickname,
  RetNickname,
  WelcomeName,
  Disconnect,
  ShowBoard,
  ErrBadPkg,
  AskBoard,
  RetBoard,
  AskOption,
  RetOption,
  MovePlayer,
  MoveEnemy,
  GiveId,
  GivePlayerName,
  GiveEnemyName,
  Wait,
  AskMovType,
  AskMovNum,
  MoreMerPlayer,
  MoreMerEnemy,
  AddStarPlayer,
  AddStarEnemy,
  Win,
  Lose,
  GoYadranGo,
  CryYadranCry,
  Message,
  AddPower,
  AddPowerEnemy,
  AskStore,
  AskPower,
  DellPower,
  DellPowerEnemy,
  AskCachipun
} MsgType;

extern Tablero* boardGame;

char* paquete_init(MsgType type, char* payload);
void enviarMensaje(int socket, char* paquete);
char* recivirMensaje(int socket);
int largo_Paquete(char * input);
char* responderAccion(char *msg, int turn);

char* startconnection();
char* connectionestablished(char* msg);
char* asknickname();
char* retnickname(char* msg);
char* welcomename(char* msg);
char* askboard();
char* retboard(char* msg);
char* giveid(char* msg);

char* wait();
char* showboard();
char* askoption();
char* retoption(char* msg);
char* moveplayer(char* msg);
char* moveenemy(char* msg);
char* moremerenemy(char* msg);
char* moremerplayer(char* msg);

char* addpowerenemy(char* msg);
char* addpower(char* msg);
char* askpower();
char* dellpower(char* msg);
char* dellpowerenemy(char* msg);
char* message(char* msg);
char* askstore();

char* giveenemyname(char* msg);
char* giveplayername(char* msg);

char* addstarenemy(char* msg);
char* addstarplayer(char* msg);
char* win();
char* lose();
char* cryyadrancry();
char* goyadrango();

char* askmovnum();
char* askmovetype();

char* askcachipun();

void disconnect();
char* errbadpkg();