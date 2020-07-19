#include "funcionesStructs.h"
#include "structs.h"
#include "cr_API.h"
#include "leerArchivos.h"
#include "filesfunctions.h"
#include "cargarArchivos.h"
#include "escribirArchivo.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>


/** variables globales */
unsigned int BLOCK_SIZE = 1024*8; // 8192bytes por bloque
Disco* diskMount;
char* diskName;

/** main */
int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Cantidad de parámetros incorrecta\n");
    printf("Uso correcto: %s PATH_TO_DISK\n", argv[0]);
    return 1;
  }
  /* Para esta prueba que demuestre el programa se usará simdiskfilled.bin */

  /* -------- Just General Functions ---------- */

  /* --> Montar disco */
  cr_mount(argv[1]);
  diskMount = montar_disco();
  printf("Disco %s montado...\n", diskName);

  /* --> Probando cr_ls */
  /*
  cr_ls(0);
  */

  /* --> Probando cr_exists */
  /*
  int existe = cr_exists(1,"guides.txt");
  printf("¿guides.txt existe en particion 1?: %i\n", existe);
  */

  /* --> Probando cr_bitmap */
  /*
  cr_bitmap(0, true);
  */

  /* ------------ Funciones de manejo de archivos ------------ */

  char* file_Name = "guides.txt";
  unsigned int disco_file = 1;
  
  /* --> Probando cr_read leyendo un archivo completo de mas de un bloque */
  /* 
  printf("Abriendo archivo %s\n",file_Name);
  crFILE* auxy_fil = cr_open(disco_file,file_Name,'r');
  
  unsigned int leer_bytes = auxy_fil->tamano;
  unsigned char* auxy_buffer = calloc(leer_bytes,sizeof(unsigned char));
  int leidos = cr_read(auxy_fil, auxy_buffer, leer_bytes);
  
  // Imprime el contenido del buffer en consola para demostrar que esta el contenido completo
  for(int i = 0; i < leidos; i++){
    printf("%c",auxy_buffer[i]);
  }

  printf("efectivamente leidos: %i\n\n",leidos);
  cr_close(auxy_fil);
  free(auxy_buffer);
  */

  /* --> Probando cr_read leyendo un archivo de mas de un bloque en partes */
  /*
  printf("Abriendo archivo %s\n",file_Name);
  crFILE* auxy_fil = cr_open(disco_file,file_Name,'r');
  
  // Pedire que lea el tamaño del archivo, menos 80 bytes
  unsigned int leer_bytes = auxy_fil->tamano - 80; 

  unsigned char* auxy_buffer = calloc(leer_bytes,sizeof(unsigned char));
  int leidos = cr_read(auxy_fil, auxy_buffer, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos);
  
  unsigned char* auxy_buffer2 = calloc(leer_bytes,sizeof(unsigned char));
  int leidos2 = cr_read(auxy_fil, auxy_buffer2, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos2);
  
  unsigned char* auxy_buffer3 = calloc(leer_bytes,sizeof(unsigned char));
  int leidos3 = cr_read(auxy_fil, auxy_buffer3, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos3);

  printf("Contenido del archivo:\n\n");


  for(int i = 0; i < leidos; i++){
    printf("%c",auxy_buffer[i]);
  }

  for(int i = 0; i < leidos2; i++){
    printf("%c",auxy_buffer2[i]);
  }

  for(int i = 0; i < leidos3; i++){
    printf("%c",auxy_buffer3[i]);
  }

  free(auxy_buffer);
  free(auxy_buffer2);
  free(auxy_buffer3);
  cr_close(auxy_fil);
  */
  
  /* --> prueba creacion de HardLinks, descargas y borrado */
  /*
  printf("\nCreando hardLink...\n");
  cr_hardlink(1, "guides.txt", "NEWhardlink_guides.txt");
  printf("Descargando archivo...\n\n");
  cr_unload(1,"NEWhardlink_guides.txt", "HardLink_Descargado.txt");
  cr_ls(1);
  printf("\nBorrando HardLink...\n");
  int removeHard = cr_rm(1, "NEWhardlink_guides.txt");
  cr_ls(1);
  */

  /* --> prueba creacion de SofrLinks, descargas y borrado */
  /*
  printf("\nCreando softLink...\n");
  
  cr_softlink(1, 3, "text.txt", "text.txt");
  cr_ls(3);
  printf("Descargando archivo...\n\n");
  cr_unload(3,"1/text.txt", "SoftLink_Descargado.txt");
  cr_ls(1);
  cr_ls(3);
  printf("\nBorrando softlink...\n");
  int removeSoft = cr_rm(3, "1/text.txt");
  cr_ls(3);
  */

  /* --> Prueba SoftLink Roto */
  /*
  printf("\nCreando softLink...\n");

  cr_softlink(1, 3, "text.txt", "text.txt");
  cr_ls(3);
  printf("\nBorrando archivo original...\n");
  int removeSoft = cr_rm(1, "text.txt");
  cr_ls(1);
  cr_ls(3);
  printf("Intento descargar softLink...\n\n");
  cr_unload(3,"1/text.txt", "SoftLink_Descargado_success.txt");
  */

  /* --> probando cargar archivos (Necesario ubicar un archivo para cargar)*/
  /*
  char* file_Name_carga = "tiles.png";
  unsigned int disco_file_carga = 1;
  cr_ls(disco_file_carga);
  printf("Cargando imagen...\n");
  // Cuidado con correrlo mas veces, crearan muchas copias del archivooooo
  cr_load(disco_file_carga, file_Name_carga);
  cr_ls(disco_file_carga);
  cr_unload(disco_file_carga, file_Name_carga, "tiles_disco.png");
  */
  printf("\n----------------\nLiberando Disco\n");
  desmontar_disco(diskMount);
  return 0;
}
