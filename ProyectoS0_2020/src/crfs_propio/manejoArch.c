#include "manejoArch.h"

int size_Archivo(unsigned char* bytes, int inicio, int leer){
	
	int aux;
	int counter = 0;
    int binario_total = 0;
    for (int i = inicio; i < inicio+leer; i++){
        for (int bit = (sizeof(unsigned char) * 7); bit >= 0; bit--) {
            aux = bytes[i] >> bit;
            if (aux & 1) {
                binario_total += pow(2,(8*leer-1)-counter);
            }
            counter++;
        }
    }

	//printf("%i", binario_total);
    //printf("\n");
	return binario_total;
};

void leer_bloqueIndice(unsigned int bloque, unsigned int disk){
    if (bloque > 0){
        /*
        los bloques indices asignados segun particion están corridos según el index (basado en la particion en la que esten)
        asi que hay que correrlos al inicio, por eso es bloque - index
        */
        FILE* file = fopen(diskName, "rb");
        unsigned int index = 65536*disk;
        fseek(file, ((unsigned int) ((bloque-index) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);

        printf("4Bytes cant tot de ref: %i\n",size_Archivo(byteIndice,0,4));
        printf("8Bytes tamaño archivo: %i\n",size_Archivo(byteIndice,4,8));
        
        /*
        Los punteros son conjunto de 4 bytes de los 8176 bytes siguientes de byteIndice
        */
        for(int i = 12; i < 13; i++){
            int aux = size_Archivo(byteIndice,i,4);
            if(aux){
                printf("%i\n",aux - index);
            }
        }
        printf("4Bytes indirectoSimple: %i\n\n",size_Archivo(byteIndice,8176+12,4));
        free(byteIndice);
    }
    else{
        /*
        Los softLinks tienen una maña, tienes que buscar al archivo según su nombre <particion>/archivo para poder obtener el indice correspondiente...
        Que flojeraaaaa pero ya voy entendiendo más como funciona esto
        */
        FILE* file = fopen(diskName, "rb");
        // el inicio del bloque direccion + 2* bloque_size = inicio bloque Indice
        unsigned int index = 65536*disk;
        fseek(file, ((unsigned int) ((bloque) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);

        printf("4Bytes cant tot de ref: %i\n",size_Archivo(byteIndice,0,4));
        printf("8Bytes tamaño archivo: %i\n",size_Archivo(byteIndice,4,8));
        
        free(byteIndice);
    }
}