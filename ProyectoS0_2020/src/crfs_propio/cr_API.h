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
// Funciones Generales
void cr_mount(char* diskname);
void cr_bitmap(unsigned disk, bool hex);
int cr_exists(unsigned int disk, char* filename);
void cr_ls(unsigned disk);
