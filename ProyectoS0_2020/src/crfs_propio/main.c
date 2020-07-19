#include "structs.h"
#include "cr_API.h"
#include "manejoArch.h"
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
char* diskName;
Disco* diskMount;

int main(int argc, char *argv[])
{
  // Este programa recibe tres parametros:
  //  argv[0] = el programa en si
  //  argv[1] = la ruta al disco
  if (argc != 2)
  {
    printf("Cantidad de parámetros incorrecta\n");
    printf("Uso correcto: %s PATH_TO_DISK\n", argv[0]);
    return 1;
  }
  cr_mount(argv[1]);
  diskMount = montar_disco();
  printf("Disco %s montado...\n", diskName);
  
  /* Probando cr_ls */
  /*
  cr_ls(0);
  */

  /* Probando cr_exists */
  /*
  int existe = cr_exists(1,"guides.txt");
  printf("¿guides.txt existe en particion 1?: %i\n", existe);
  */

  /* Probando cr_bitmap */
  //cr_bitmap(1, true);
  
  
  imprimir_disco(diskMount);
  

  /* Probar la lectura del bloque direccion de los archivos
  for(int part = 0; part < 4; part++){
    printf("PARTICON %i:\n\n",part +1);
    for(int i = 0; i < diskMount->partitions[part]->largo;i++){
      leer_bloqueDireccion_aux(diskMount->partitions[part]->files[i]);
    }
  }
   */
  

  printf("\n----------------\nLiberando Disco\n");
  desmontar_disco(diskMount);

  return 0;
}
