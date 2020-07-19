#include "laser.h"

void matarPersona(Cola* cola,int index){
    //printf("id: %i,tipo: %i\n",cola -> people[index]-> id_persona, cola -> people[index]-> tipo);
    free(cola -> people[index]);
    int largo_laser = cola -> largo;
    for(int continua = index; continua < largo_laser - 1; continua++){
        cola -> people[continua] = cola -> people[continua + 1];
    }
    cola -> largo -= 1;
}