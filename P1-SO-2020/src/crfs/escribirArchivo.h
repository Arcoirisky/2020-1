#pragma once
#include "structs.h"
#include "cr_API.h"
/*
extern unsigned int BLOCK_SIZE = 1024*8; // 8192bytes por bloque
extern Disco* diskMount;
extern char* diskName;
*/
int buscar_blocke_para_indice_de_file(int particion);
char* stringToBinary(char* s);
int buscar_bloque_director_libre(int particion, char *filename);
char* prefix_to_string(char* first_string,char* second_string);
crFILE *alter_open(unsigned disk, char* filename);
int escribir_bloque_de_dato(int particion,unsigned char *buffer_para_un_bloque_de_memoria,int dir_donde_escribir_bloque);
void ReverseArray(char* arr, int size);
char *dec_to_bin(int decimal,int porte_arreglo);
int cr_write(crFILE *file_desc,void *buffer, int nbytes);