#include "Juego.h"

char* traducirPoder(PowerType power){
  if (power == Nada){
    return "-Vacio-";
  }
  if(power == DobleDice){
    return "Dados Dobles";
  }
  if(power == Duelo){
    return "Duelo";
  }
  if(power == RobaMeritos){
    return "Roba Meritos";
  }
}

Jugador* player_init(int socket_player, char* name){
  Jugador* new_player = malloc(sizeof(Jugador));
  new_player -> socket = socket_player;
  new_player -> meritos = 0;
  new_player -> ruzStar = 0;
  new_player -> poder1 = Nada;
  new_player -> poder2 = Nada;
  new_player -> poder3 = Nada;
  new_player -> len_Poderes = 0;
  new_player -> posicion = 0;
  new_player -> Yadran = false;
  
  strcpy(new_player->nombre, name);

  return new_player;
}

Partida* partida_init(Tablero* tablero, Jugador* Player1, Jugador* Player2){
  Partida* new_game = malloc(sizeof(Partida));
  new_game -> board = tablero;
  new_game -> Player1 = Player1;
  new_game -> Player2 = Player2;
  return new_game;
}

Tablero* tablero_init(char* path){
  FILE* tablero_file = fopen(path, "r");

  if(tablero_file == NULL){
    printf("archivo de tablero no encontrado\n");
    exit(1); // aca salimos del juego pero se puede cambiar el manejo
  }

  int largo_tablero;
  fscanf(tablero_file, "%i", &largo_tablero);

  Tablero* tablero = malloc(sizeof(Tablero));
  tablero->largo = largo_tablero;
  tablero->boxs = (Casilla**)malloc(sizeof(Casilla*)*largo_tablero);

  char tipo[largo_tablero];
  fscanf(tablero_file, "%s", tipo);

  for (int box = 0; box < largo_tablero; ++box)
  {
    tablero->boxs[box] = (Casilla*)malloc(sizeof(Casilla));
    tablero->boxs[box]->id = box;
    tablero->boxs[box]->p1 = false;
    tablero->boxs[box]->p2 = false;

    switch(tipo[box]){
      case 'I':
        tablero->boxs[box]->tipo = I;
        break;

      case 'O':
        tablero->boxs[box]->tipo = O;
        break;

      case 'X':
        tablero->boxs[box]->tipo = X;
        break;

      case 'R':
        tablero->boxs[box]->tipo = R;
        break;

      case 'P':
        tablero->boxs[box]->tipo = P;
        break;

      case 'T':
        tablero->boxs[box]->tipo = T;
        break;

      case 'F':
        tablero->boxs[box]->tipo = F;
        break;

      case 'E':
        tablero->boxs[box]->tipo = E;
        break;

      case 'A':
        tablero->boxs[box]->tipo = A;
        break;

      case 'D':
        tablero->boxs[box]->tipo = D;
        break;

      case 'Y':
        tablero->boxs[box]->tipo = Y;
        break;

      default:
        printf("error en el archivo del tablero\n");
    }
  }
  fclose(tablero_file);

  return tablero;
}


void show_board(Partida* partida){
  for (int i=0; i < partida->board->largo; i++){
    if (i == partida->Player1->posicion){
      printf("1 ");
    }
    else{
      printf("- ");
    }
  }
  printf("\n");
  char* cas_types[] = {"I", "O", "X", "R", "P", "T", "F", "E", "A", "D", "Y"};
  for (int i=0; i < partida->board->largo; i++){
    printf("%s ", cas_types[partida->board->boxs[i]->tipo]);
  }
  printf("\n");
  for (int i=0; i < partida->board->largo; i++){
    if (i == partida->Player2->posicion){
      printf("2 ");
    }
    else{
      printf("- ");
    }
  }
  printf("\n");
}


void destroy_Partida(Partida* partida){

  free(partida -> Player1);
  free(partida -> Player2);
  for (int i = 0; i < partida -> board -> largo; ++i)
  {
    free(partida -> board ->boxs[i]);
  }

  free(partida ->board->boxs);
  free(partida ->board);

  free(partida);

}

int rollSinleDice(){
  // Supuestamente me da un int entre (0 y 9) + 1 seria entre 1 y 10
  int dice = (rand() % 10) + 1;
  return dice;
}

void show_players(struct Jugador* p1, struct Jugador* p2){
int w[3] = {4,8,20};  // Adjust as desired
int indent = 4;
printf("%*s%*s\n", w[2], "Jugador 1:", w[2]*2, "Jugador 2:");
// Se arman nuevos strings para acomodarse al formatting
char* name_p1 = new_strcat("- Nombre: ", p1->nombre);
char* name_p2 = new_strcat("- Nombre: ", p2->nombre);

char* merit_p1 = new_strintcat("- Meritos: ", p1->meritos);
char* merit_p2 = new_strintcat("- Meritos: ", p2->meritos);

char* ruz_p1 = new_strintcat("- Estrellas Ruz: ", p1->ruzStar);
char* ruz_p2 = new_strintcat("- Estrellas Ruz: ", p2->ruzStar);
printf("%*s%-*s%s\n", w[2]-indent, "", w[2]*2, name_p1, name_p2);
printf("%*s%-*s%s\n", w[2]-indent, "", w[2]*2, merit_p1, merit_p2);
printf("%*s%-*s%s\n", w[2]-indent, "", w[2]*2, ruz_p1, ruz_p2);
printf("%*s%-*s%s\n", w[2]-indent, "", w[2]*2, "- Poderes: ", "- Poderes: ");

//Powers
show_powers(p1->poder1, p2->poder1);
show_powers(p1->poder2, p2->poder2);
show_powers(p1->poder3, p2->poder3);

//FREEEE'EM ALL
free(name_p1);
free(name_p2);
free(merit_p1);
free(merit_p2);
free(ruz_p1);
free(ruz_p2);
}

void show_powers(PowerType power_1, PowerType power_2){
int w[3] = {4,8,20};  // Adjust as desired
int indent = 4;
char* power_name[4] = {"", "Duelo", "Dado Doble", "Roba Meritos"};
char* power_p1 = new_strcat("- ", power_name[power_1]);
char* power_p2 = new_strcat("- ", power_name[power_2]);

printf("%*s%-*s%s\n", w[2]+indent, "", w[2]*2, power_p1, power_p2);

free(power_p1);
free(power_p2);
}

char* new_strcat(const char* tag, const char* data){
  char* new_str = calloc(strlen(tag) + strlen(data) + 1, sizeof(char));
  strcpy(new_str, tag);
  strcat(new_str, data);
  return new_str;
}

char* new_strintcat(const char* tag, int num){
  char buff[32];
  sprintf(buff, "%d", num);
  return new_strcat(tag, buff);
}

