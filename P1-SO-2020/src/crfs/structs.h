#pragma once

typedef enum typeLink {SoftLink, HardLink, NormalFile} TypeLink;
typedef enum brokenLink {InConnect, Connect} BrokenLink;
typedef enum modeFile {OpenRead, OpenWrite, CloseFile} ModeFile;

struct BufferLectura;
typedef struct BufferLectura BufferLectura;
struct BufferLectura
{
    int pBloque;
    int pByte;
    int bytesPorLeer;
    int bytesLeidos;
    int dirSim; //puntero indirecto simple, si es != 0 entonces existe
    unsigned char* buffBloque; //bloque guardado para la lectura
    unsigned int* punteros; //punteros a los bloques direccion...
    int len_punteros;
};

struct crFILE;
typedef struct crFILE crFILE;
struct crFILE
{
    int pos_BloqueIndice;
    int particion;
    int tamano;
    char nombre[255];
    TypeLink typeLink;
    BrokenLink linkRoto;
    ModeFile fileMode;
    BufferLectura* lectBuff;
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
#include "funcionesStructs.h"

extern Disco* diskMount;
extern char* diskName;
extern unsigned int BLOCK_SIZE;

crFILE* montar_archivo(char* filename, int dir_BloqueIndice);
Disco* montar_disco();

void imprimir_disco(Disco* disk);
void desmontar_disco(Disco* disk);


