#include "funcionesStructs.h"
#include "structs.h"
#include "cr_API.h"
#include "leerArchivos.h"
#include "filesfunctions.h"
#include "cargarArchivos.h"

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
  cr_mount(argv[1]);
  diskMount = montar_disco();
  printf("Disco %s montado...\n", diskName);
  
  // imprimir_disco(diskMount);
  
  /* Probando cr_ls */
  
  //cr_ls(0);
  

  /* Probando cr_exists */
  
  //int existe = cr_exists(1,"guides.txt");
  //printf("¿guides.txt existe en particion 1?: %i\n", existe);
  

  /* Probando cr_bitmap */
  //cr_bitmap(0, true);
  cr_ls(0);
  
  char* file_Name = "guides.txt";
  unsigned int disco_file = 1;
  /* Probando lectura de archivos */
  
  printf("Abriendo archivo %s\n",file_Name);
  crFILE* auxy_fil = cr_open(disco_file,file_Name,'r');
  
  unsigned int leer_bytes = auxy_fil->tamano;
  unsigned char* auxy_buffer = calloc(leer_bytes,sizeof(unsigned char));
  int leidos = cr_read2(auxy_fil, auxy_buffer, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos);
  free(auxy_buffer);
  
  unsigned char* auxy_buffer2 = calloc(leer_bytes,sizeof(unsigned char));
  leidos = cr_read2(auxy_fil, auxy_buffer2, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos);
  free(auxy_buffer2);
/*
  unsigned char* auxy_buffer3 = calloc(leer_bytes,sizeof(unsigned char));
  leidos = cr_read(auxy_fil, auxy_buffer3, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos);
  free(auxy_buffer3);

  unsigned char* auxy_buffer4 = calloc(leer_bytes,sizeof(unsigned char));
  leidos = cr_read(auxy_fil, auxy_buffer4, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos);
  free(auxy_buffer4);
  */
  cr_close(auxy_fil);
  

  /* Probando descargar archivo */
  // ojo cuidado que no me deja guardar nombres del tipo 1/testeo.mp3
  // cr_unload(disco_file, file_Name, "testeo.mp3");
  //cr_unload(4, NULL, "");
  //cr_unload(0, NULL, "");
  //cr_unload(1, "Program in C.mkv", "movie.mkv");
  /*
  leer_bytes = 4400000;
  unsigned char* auxy_buffer2 = malloc(sizeof(unsigned char) * leer_bytes);
  
  leidos = cr_read(auxy_fil, auxy_buffer2, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos);
  free(auxy_buffer2);
  
  leer_bytes = 97732;
  unsigned char* auxy_buffer3 = malloc(sizeof(unsigned char) * leer_bytes);
  
  leidos = cr_read(auxy_fil, auxy_buffer3, leer_bytes);
  printf("efectivamente leidos: %i\n\n",leidos);
  free(auxy_buffer3);
  */
//  cr_bitmap(1, false);


  //prueba creacion hardlink
  //printf("\nCreando hardLink...\n");
  //cr_hardlink(1, "guides.txt", "NEWhardlink_guides.txt");
  //cr_ls(1);
  //printf("\nBorrando HardLink...\n");
  //int remove = cr_rm(1, "NEWhardlink_guides.txt");
  //cr_ls(1);

  //cr_unload(0, NULL, "");
  //  cr_bitmap(1, false);
  //printf("\nCreando softLink...\n");
  //prueba creacion softlink
  //cr_softlink(1, 3, "text.txt", "text.txt");
  //cr_ls(3);
  
  //printf("\nBorrando softlink...\n");
  //remove = cr_rm(3, "1/text.txt");
  //cr_ls(3);

  //cr_unload(3,"1/text.txt", "3_1-text.txt");
  //desmontar_disco(diskMount);
  //diskMount = montar_disco();
  //imprimir_disco(diskMount);
  //remove = cr_rm(4, "1/Baroque.mp3");
  //cr_ls(4);
  //cr_unload(4,"1/Baroque.mp3", "3_1-text.mp3");
  //cr_bitmap(1,false);

  //cr_ls(0);
  //char* name = "cargarYes.mp3";
  //cr_rm(1,name);
  //cr_bitmap(1,false);
  //cr_ls(1);
  //printf("cargando file... a medias\n");
  //cr_unload(2,"yes.mp3","cargarYes.mp3");
  //cr_load(1, name);
  //cr_ls(1);
  //desmontar_disco(diskMount);
  //diskMount = montar_disco();
  //imprimir_disco(diskMount);
  //int i = siguiente_BloqueVacio(1);
  //printf("%i\n",i);
  //cr_ls(0);
  //cr_unload(1,"cargarYes.mp3","DEScargarYes.mp3");
  printf("\n----------------\nLiberando Disco\n");
  desmontar_disco(diskMount);
  return 0;
}
