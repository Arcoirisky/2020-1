#pragma once
#include "structs.h"
#include "cr_API.h"

extern Disco* diskMount;
extern char* diskName;
extern unsigned int BLOCK_SIZE;

crFILE* cr_open(unsigned disk, char* filename, char mode);
int cr_read(crFILE* file_desc, void* buffer, int nbytes);
int cr_read2(crFILE* file_desc, void* buffer, int nbytes);
int cr_close(crFILE* file_desc);

crFILE* obtenerFile(unsigned disk, char* filename);
void leer_punterosDireccion(crFILE* archivo, bool indirectoSimple);
void cargar_bloqueSig(crFILE* archivo);
