#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

struct crFILE;
typedef struct crFILE crFILE;
typedef enum typeLink {SoftLink, HardLink, NormalFile} TypeLink;
typedef enum brokenLink {InConnect, Connect} brokenLink;
struct crFILE
{
    char nombre[255];
    int pos_BloqueIndice;
    int particion;
    int tamano;
    TypeLink typeLink;
    brokenLink linkRoto;
};

struct Particion;
typedef struct Particion Particion;

struct Particion
{
    crFILE** files;
    int id;
    int largo;
    int capacidad;
};

struct Disco;
typedef struct Disco Disco;
struct Disco
{
    Particion** partitions;
    char* nombre;
    int largo;
};

extern char* diskName;
extern unsigned int BLOCK_SIZE;

bool string_equals(char* string1, char* string2);
crFILE* montar_archivo(char* filename, int dir_BloqueIndice);
int size_Archivo2(unsigned char* bytes, int inicio, int leer);
void leer_bloqueDireccion_aux(crFILE* archivo);
Disco* montar_disco();
void decifrar_infoFiles(Disco* disco);
void imprimir_disco(Disco* disk);
void desmontar_disco(Disco* disk);
int pos_indice_block(unsigned char* Directorio);

int cantidad_refArchivo(crFILE* archivo);
int tamanio_File(crFILE* archivo);