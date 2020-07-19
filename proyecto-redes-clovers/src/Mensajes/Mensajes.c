#include "Mensajes.h"

char* paquete_init(MsgType type, char* payload){
  int payloadSize = largo_Paquete(payload);
  char *paquete = malloc(3 + payloadSize);
  paquete[0] = type;
  paquete[1] = payloadSize;
  strcpy(&paquete[2], payload);
  return paquete;
}

void enviarMensaje(int socket, char* paquete){
    // Largo del paquete es el byte 1
    send(socket, paquete, 2 + paquete[1], 0);
    free(paquete); //Borramos el paquete enviado
}

char* recivirMensaje(int socket){
    // el mensaje se divide en 3 partes
    //printf("\n Paquete recibido !!!!\n");
    
    // Primero el ID
    char ID;
    recv(socket, &ID, 1, 0);
    //printf("ID: %d\n", ID);

    //Le sigue el payLoadSize
    char payloadSize;
    recv(socket, &payloadSize, 1, 0);
    //printf("PayloadSize: %d\n", payloadSize);

    //Por ultimo el contenido
    char payload[payloadSize + 1];
    recv(socket, payload, payloadSize, 0);
    payload[payloadSize] = '\0'; //Esto para el cierre
    //printf("Mensaje: %s\n", payload);

    char *paquete = malloc(3 + payloadSize);
    paquete[0] = ID;
    paquete[1] = payloadSize;
    strcpy(&paquete[2], payload);

    return paquete;
}

int largo_Paquete(char * input){
  int i = 0;
  while(true){
    if (input[i] == '\0'){
      return i;
    }
    i++;
  }
}

char* responderAccion(char *msg, int turn){
  MsgType type = msg[0];
  switch(type)
  {
    case StartConnection:
      return startconnection();
      break;
    case ConnectionEstablished:
      return connectionestablished(msg);
      break;
    case AskNickname:
      return asknickname();
      break;
    case RetNickname:
      return retnickname(msg);
      break;
    case WelcomeName:
      return welcomename(msg);
      break;
    case AskBoard:
      return askboard();
      break;
    case RetBoard:
      return retboard(msg);
      break;
    case ShowBoard:
      return showboard();
      break;
    case AskOption:
      return askoption();
      break;
    case RetOption:
      return retoption(msg);
      break;
    case Disconnect:
      disconnect();
      break;
    case MovePlayer:
      return moveplayer(msg);
      break;
    case MoveEnemy:
      return moveenemy(msg);
      break;
    case MoreMerPlayer:
      return moremerplayer(msg);
      break;
    case MoreMerEnemy:
      return moremerenemy(msg);
      break;
    case GiveId:
      return giveid(msg);
      break;
    case GivePlayerName:
      return giveplayername(msg);
      break;
    case GiveEnemyName:
      return giveenemyname(msg);
      break;
    case Wait:
      return wait();
      break;
    case AskMovType:
      return askmovetype();
      break;
    case AskMovNum:
      return askmovnum();
      break;
    case AddStarPlayer:
      return addstarplayer(msg);
      break;
    case AddStarEnemy:
      return addstarenemy(msg);
      break;
    case Win:
      return win();
      break;
    case Lose:
      return lose();
      break;
    case GoYadranGo:
      return goyadrango();
      break;
    case CryYadranCry:
      return cryyadrancry();
      break;
    case Message:
      return message(msg);
      break;
    case AddPower:
      return addpower(msg);
      break;
    case AddPowerEnemy:
      return addpowerenemy(msg);
      break;
    case AskStore:
      return askstore();
      break;
    case AskPower:
      return askpower();
      break;
    case DellPower:
      return dellpower(msg);
      break;
    case DellPowerEnemy:
      return dellpowerenemy(msg);
      break;
    case AskCachipun:
      return askcachipun();
    default:
      return paquete_init(ErrBadPkg, " ");
  }
  return NULL;
}

char* giveenemyname(char* msg){
  char *name = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(name, &msg[2]);
  name[(int) msg[1]] = '\0';
  if(iD == 1){
    strcpy(playerGame->Player2->nombre,name);
  }
  else{
    strcpy(playerGame->Player1->nombre,name);
  }
  free(name);
  return NULL;
}

char* giveplayername(char* msg){
  char *name = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(name, &msg[2]);
  name[(int) msg[1]] = '\0';
  if(iD == 1){
    strcpy(playerGame->Player1->nombre,name);
  }
  else{
    strcpy(playerGame->Player2->nombre,name);
  }
  free(name);
  return NULL;
}

char* dellpowerenemy(char* msg){
  char *poder = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(poder, &msg[2]);
  poder[(int) msg[1]] = '\0';
  Jugador* enemy;
  if(iD == 1){
    enemy = playerGame -> Player2;
  }
  else{
    enemy = playerGame -> Player1;
  }
  
  if(atoi(poder) == 1){
    free(poder);
    enemy -> poder1 = Nada;
    return NULL;
  }

  if(atoi(poder) == 2){
    enemy -> poder2 = Nada;
    free(poder);
    return NULL;
  }

  if(atoi(poder) == 3){
    enemy -> poder3 = Nada;
    free(poder);
    return NULL;
  }
}

char* dellpower(char* msg){
  char *poder = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(poder, &msg[2]);
  poder[(int) msg[1]] = '\0';
  Jugador* player;
  if(iD == 1){
    player = playerGame -> Player1;
  }
  else{
    player = playerGame -> Player2;
  }
  
  if(atoi(poder) == 1){
    free(poder);
    player -> poder1 = Nada;
    return NULL;
  }

  if(atoi(poder) == 2){
    player -> poder2 = Nada;
    free(poder);
    return NULL;
  }

  if(atoi(poder) == 3){
    player -> poder3 = Nada;
    free(poder);
    return NULL;
  }
}

char* askpower(){
  char input[100];
  Jugador* player;
  if(iD == 1){
    player = playerGame -> Player1;
  }
  else {
    player = playerGame -> Player2;
  }
  printf("\nEsta es tu biblioteca de poderes:\n");
  printf("1) %s\n",traducirPoder(player->poder1));
  printf("2) %s\n",traducirPoder(player->poder2));
  printf("3) %s\n",traducirPoder(player->poder3));
  printf("\n\nRespuesta:");
  scanf("%s", input);
  char poder[100];
  if(atoi(input) == 1){
    sprintf(poder, "%i-%i", player -> poder1, 1);
  }
  else{
    if(atoi(input) == 2){
      sprintf(poder, "%i-%i", player -> poder2, 2);
    }
    else{
      sprintf(poder, "%i-%i", player -> poder3, 3);
    }
  }
  printf("\n");
  char* paquete = paquete_init(RetOption, poder);
  return paquete;
}


char* addpowerenemy(char* msg){
  char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(cant, &msg[2]);
  cant[(int) msg[1]] = '\0';
  Jugador* player;
  if(iD == 1){
    player = playerGame ->Player2;
  }
  else{
    player = playerGame -> Player1;
  }
  
  if(player -> poder1 == Nada){
    player -> poder1 = atoi(cant);
    free(cant);
    return NULL;
  }
  
  if(player -> poder2 == Nada){
    player -> poder2 = atoi(cant);
    free(cant);
    return NULL;
  }

  if(player -> poder3 == Nada){
    player -> poder3 = atoi(cant);
    free(cant);
    return NULL;
  }

  return NULL;
}


char* addpower(char* msg){
  char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(cant, &msg[2]);
  cant[(int) msg[1]] = '\0';
  Jugador* player;
  if(iD == 1){
    player = playerGame ->Player1;
  }
  else{
    player = playerGame -> Player2;
  }
  
  if(player -> poder1 == Nada){
    player -> poder1 = atoi(cant);
    free(cant);
    return NULL;
  }
  
  if(player -> poder2 == Nada){
    player -> poder2 = atoi(cant);
    free(cant);
    return NULL;
  }

  if(player -> poder3 == Nada){
    player -> poder3 = atoi(cant);
    free(cant);
    return NULL;
  }

  return NULL;
}

char* message(char* msg){
  char *mensaje = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(mensaje, &msg[2]);
  mensaje[(int) msg[1]] = '\0';
  printf("%s\n", mensaje);
  free(mensaje);
  return NULL;
}

char* cryyadrancry(){
  printf("¡ALTO POLICIA! niveles altos de alcohol, pierdes el turno\n");
  return NULL;
}

char* goyadrango(){
  printf("¡Bienvenido a la Fonda de Don Yadra!\n");
  return NULL;
}

char* addstarenemy(char* msg){
  int new_star;
  char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(cant, &msg[2]);
  cant[(int) msg[1]] = '\0';
  new_star = atoi(cant);
  
  printf("\n ¡Tu contrincante gano +%i Ruz-Stars!\n",new_star);
  
  if (iD == 1){
    playerGame ->Player2->ruzStar += new_star;
  }
  else{
    playerGame ->Player1->ruzStar += new_star;
  }
  free(cant);
  return NULL;
}

char* addstarplayer(char* msg){
    int new_star;
    char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
    strcpy(cant, &msg[2]);
    cant[(int) msg[1]] = '\0';
    new_star = atoi(cant);
    
    printf("\n ¡GANASTE +%i RUZ-STARS!\n",new_star);
    
    if (iD == 1){
      playerGame ->Player1->ruzStar += new_star;
    }
    else{
      playerGame ->Player2->ruzStar += new_star;
    }
    free(cant);
    return NULL;
}


char* moremerenemy(char* msg){
  int new_meritos;
  char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(cant, &msg[2]);
  cant[(int) msg[1]] = '\0';
  new_meritos = atoi(cant);
  if(new_meritos > 0){
    printf("\n ¡Tu contrincante gano +%i meritos!\n",new_meritos);
  }
  else{
    printf("\n ¡Tu contrincante perdió %i meritos!\n",new_meritos);
  }
  if (iD == 1){
    playerGame ->Player2->meritos += new_meritos;
  }
  else{
    playerGame ->Player1->meritos += new_meritos;
  }
  free(cant);
  return NULL;
}

char* moremerplayer(char* msg){
    int new_meritos;
    char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
    strcpy(cant, &msg[2]);
    cant[(int) msg[1]] = '\0';
    new_meritos = atoi(cant);
    if (new_meritos > 0){
      printf("\n¡GANASTE +%i MERITOS!\n",new_meritos);
    }
    else{
      printf("\n¡PERDISTE %i MERITOS!\n",new_meritos);
    }
    if (iD == 1){
      playerGame ->Player1->meritos += new_meritos;
    }
    else{
      playerGame ->Player2->meritos += new_meritos;
    }
    free(cant);
    return NULL;
}

char* lose(){
  printf("\n¡¡¡¡PERDISTE :C!!!!\n");
  return NULL;
}

char* win(){
  printf("\n¡¡¡¡GANASTE :D!!!!\n");
  return NULL;
}

char* askmovnum(){
  char input[100];
  printf("\nElige un numero entre 1 y 10\n\nRespuesta: ");
  scanf("%s", input);
  printf("\n");
  char* paquete = paquete_init(RetOption, input);
  return paquete;
}

char* askmovetype(){
  char input[100];
  printf("\nElige el tipo de movimiento:\n1) Aleatorio\n2) Manual\n\nRespuesta: ");
  scanf("%s", input);
  printf("\n");
  char* paquete = paquete_init(RetOption, input);
  return paquete;
}

char* wait(){
  printf("Espera a que juege tu oponente...\n");
  return NULL;
}

char* moveenemy(char* msg){
  int new_pos;
  char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(cant, &msg[2]);
  cant[(int) msg[1]] = '\0';
  new_pos = atoi(cant);
  if (iD == 1){
    playerGame ->Player2->posicion = new_pos;
  }
  else{
    playerGame ->Player1->posicion = new_pos;
  }
  free(cant);
  return NULL;
}

char* moveplayer(char* msg){
    int new_pos;
    char *cant = malloc(2 + sizeof(char) * (int) msg[1]);
    strcpy(cant, &msg[2]);
    cant[(int) msg[1]] = '\0';
    new_pos = atoi(cant);
    if(new_pos == 0){
      printf("¡Regresas al inicio!\n");
    }
    char* paquete;
    if (iD == 1){
      playerGame ->Player1->posicion = new_pos;
    }
    else{
      playerGame ->Player2->posicion = new_pos;
    }
    free(cant);
    return NULL;
}

char* giveid(char* msg){
  char *sId = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(sId, &msg[2]);
  sId[(int) msg[1]] = '\0';
  iD = atoi(sId);
  free(sId);
  return NULL;
}

char* askoption(){
  char input[100];
  printf("\nMenu:\n1) Utilizar Poder\n2) Rendirse\n3) Avanzar\n\nRespuesta: ");
  scanf("%s", input);
  printf("\n");
  char* paquete = paquete_init(RetOption, input);
  return paquete;
}

char* askstore(){
  char input[100];
  printf("\nBienvenido a la tienda:\n1) Duelo [10 Meritos]\n2) Dados Dobles [10 Meritos]\n3) Roba Meritos [10 Meritos]\n4) Salir sin comprar\n\nRespuesta: ");
  scanf("%s", input);
  printf("\n");
  char* paquete = paquete_init(RetOption, input);
  return paquete;
}

char* retoption(char* msg){
  char *option = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(option, &msg[2]);
  option[(int) msg[1]] = '\0';
  return option;
}

char* showboard(){
  printf("\n");
  show_board(playerGame);
  show_players(playerGame->Player1, playerGame->Player2);
  return NULL;
}

char* startconnection(){
  char* pkg = paquete_init(ConnectionEstablished, " ");
  return pkg;
}

char* connectionestablished(char* msg){
  return NULL;
}

char* askboard(){
  char input[100];
  printf("\nIngresa el path del tablero\nRespuesta: ");
  scanf("%s", input);
  printf("\n");
  char* paquete = paquete_init(RetBoard, input);
  printf("Cargando...\n");
  return paquete;
}

char* retboard(char* msg){
  char *boardName = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(boardName, &msg[2]);
  boardName[(int) msg[1]] = '\0';
  return boardName;
}

char* welcomename(char* msg){
  char* nickname = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(nickname, &msg[2]);
  nickname[(int) msg[1]] = '\0';
  printf("¡Bienvenido %s!\n", nickname);
  free(nickname);
  return NULL;
}

char* asknickname(){
  char input[100];
  printf("\nIngresa nombre de usuario. En caso de estar ocupado se preguntará por este nuevamente\nRespuesta: ");
  scanf("%s", input);
  printf("\n");

  char* paquete = paquete_init(RetNickname, input);
  return paquete;
}

char* retnickname(char* msg){
  char *nickname = malloc(2 + sizeof(char) * (int) msg[1]);
  strcpy(nickname, &msg[2]);
  nickname[(int) msg[1]] = '\0';
  return nickname;
}

char* askcachipun(){
  char input[100];
  printf("\nEs momento del duelooo !!\n\nIngresa tu movida del cachipun: \n1) Piedra\n2) Papel\n3) Tijeras\nRespuesta: ");
  scanf("%s", input);
  printf("\n");
  char* paquete = paquete_init(RetOption, input);
  return paquete;
}

void disconnect(){
  printf("\n...Desconectando...\n");
  exit(0);
}

char* errbadpkg(){
  char msg[] = " ";
  char* pkg = paquete_init(StartConnection, msg);
  return pkg;
}
