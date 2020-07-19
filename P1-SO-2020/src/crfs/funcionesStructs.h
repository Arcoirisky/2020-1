#pragma once

#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

extern Disco* diskMount;
extern char* diskName;
extern unsigned int BLOCK_SIZE;

int tamanio_File(crFILE* archivo);
int cantidad_refArchivo(crFILE* archivo);
int particion_Archivo(crFILE* archivo);
int size_Archivo2(unsigned char* bytes, int inicio, int leer);
int punteroIndirectoSimple(crFILE* archivo);
int pos_indice_block(unsigned char* Directorio);

void decifrar_infoFiles(Disco* disco);
void leer_bloqueDireccion(crFILE* archivo);

bool string_equals(char* string1, char* string2);
bool softLinkRec(crFILE* archivo);

void agregar_directorio(unsigned int disk, unsigned int bloque_indice, char* name);