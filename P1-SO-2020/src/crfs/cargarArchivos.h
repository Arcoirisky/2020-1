#pragma once
#include "structs.h"
#include "cr_API.h"
#include "leerArchivos.h"
#include "stdio.h"

extern Disco* diskMount;
extern char* diskName;
extern unsigned int BLOCK_SIZE;

int cr_load(unsigned int disk, char* origen);
int siguiente_BloqueVacio(unsigned int disk);