#pragma once
#include "structs.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

extern char* diskName;
extern Disco* diskMount;

int byte_to_Bits(unsigned char byte, bool hex);
unsigned char* get_new_bytemap(unsigned int, int, char*);
// Funciones Generales
void cr_mount(char* diskname);
void cr_bitmap(unsigned disk, bool hex);
int cr_exists(unsigned int disk, char* filename);
void cr_ls(unsigned disk);

int cr_rm(unsigned disk, char* filename);
int cr_hardlink(unsigned disk, char* orig, char* dest);
int cr_softlink(unsigned disk_orig, unsigned disk_dest, char* orig, char* dest);