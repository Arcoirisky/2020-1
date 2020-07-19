#pragma once
#include "structs.h"
#include "cr_API.h"
#include "leerArchivos.h"

extern Disco* diskMount;
extern char* diskName;
extern unsigned int BLOCK_SIZE;

int cr_unload(unsigned int disk, char* orig, char* dest);