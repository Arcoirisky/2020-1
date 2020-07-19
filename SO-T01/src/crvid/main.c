#include "simulacion.h"
#include "tablero.h"
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
int tipo;
struct Generador;
struct Generador{
  int Pid;
  int num_generados;
  int* listaHijos;
};
typedef struct Generador Generador;
//Generador* PROCESO;
void adios(void){
  exit(0);
}

void ctrlCSIMULACION(int sig){
  //printf("He apretado control + C en %i SIMULACION\n", getpid());
  //printf("tipo %i\n", tipo);
  atexit(adios);
}

void ctrlCPROCESO(int sig){
  //printf("He apretado control + C en %i PROCESO\n", getpid());
  //printf("tipo %i con pid %i\n", tipo, getpid());
  //free(PROCESO -> listaHijos);
  //free(PROCESO);
  atexit(adios);
}

void alarmaSIMULACION(int sig){
  //printf("Alto ahi cerebrito SIMULACION %i\n",getpid());
  //printf("tipo %i\n", tipo);
  atexit(adios);
}

void alarmaPROCESO(int sig){
  //printf("Alto ahi cerebrito PROCESO %i\n", getpid());
  //printf("tipo %i\n", tipo);
  atexit(adios);
}

void funcionProcesos(char* filename, int line)
{
  int contador_linea = 0;
  FILE* file = fopen(filename, "r");
  while(contador_linea < line){
    //int tipo;
    //printf("%i: ",contador_linea);
    fscanf(file, "%i ", &tipo);
    if(tipo == 0){
      int time,num;
      fscanf(file, "%i %i", &time, &num);

      //printf("%i %i\nLista: ", time, num);
      int lista[num];
      for(int lin = 0; lin < num; lin++){
        int int_aux;
        fscanf(file, "%i", &int_aux);
        //printf("%i ", int_aux);
        lista[lin] = int_aux;
      }
      //printf("\n");
    }
    else{
      // tipo 1
      int iteraciones, A, B, C, D, tablero;
      fscanf(file, "%i %i %i %i %i %i", &iteraciones, &A, &B, &C, &D, &tablero);
      //printf("%i %i %i %i %i %i\n", iteraciones, A, B, C, D, tablero);
    }
    contador_linea++;
  };

  //int tipo;
  fscanf(file, "%i", &tipo);
  if (tipo == 1){
    int id = contador_linea;
    
    signal(SIGINT, ctrlCSIMULACION);
    signal (SIGALRM, alarmaSIMULACION);
    int iteraciones, A, B, C, D, tablero;
    fscanf(file, "%i %i %i %i %i %i", &iteraciones, &A, &B, &C, &D, &tablero);
    //printf("Soy una simulacion_%i %i\n",contador_linea,getpid());
    //printf("Mis datos son:\n - iteraciones: %i\n - A: %i\n - B: %i\n - C: %i\n - D: %i\n - Tablero: %i\n\n", iteraciones, A, B, C, D, tablero);
    
    fclose(file);
    juegoDeLaVida(iteraciones, A, B, C, D, tablero, contador_linea);
    atexit(adios);
  }
  else{
    // tipo generador
    int time,num;
    fscanf(file, "%i %i", &time, &num);
    
    int lista[num];
    for(int lin = 0; lin < num; lin++){
      int int_aux;
      fscanf(file, "%i", &int_aux);
      lista[lin] = int_aux;
    }
    fclose(file);
    //printf("Soy un generador_%i %i\n",contador_linea,getpid());
    
    
    // Debo ahora crear proceso
    int ListaProcesos[num];
    int id = contador_linea;
    int status;
    for(int proceso = 0; proceso<num; proceso++){
      //printf("\nGENERANDO PROCESO\n");
      int pid_generador_hijo = fork();
      
      alarm(time);
      signal(SIGINT,(void (*)(int))ctrlCPROCESO);
      signal (SIGALRM, alarmaPROCESO);
      //agrego al hijo en una lista
      
      ListaProcesos[proceso] = pid_generador_hijo;
      if(!pid_generador_hijo){
        funcionProcesos(filename, lista[proceso]);
        break;
      }
    }
    while(true){  
      int contabilizador = 0;
      for(int process=0; process < num; process ++){
        // este codigo lo saque de https://stackoverflow.com/questions/4200373/just-check-status-process-in-c
        pid_t return_pid = waitpid(ListaProcesos[process], &status, WNOHANG); /* WNOHANG def'd in wait.h */
        if (return_pid == -1) {
            /* error */
            //printf("Estoy en la parte del error pero es porque espero un proceso terminado %i\n", ListaProcesos[process]);
            contabilizador++;
        } else if (return_pid == 0) {
            /* child is still running */
            //printf("Soy %i y estoy esperando a que termine %i\n",getpid(),ListaProcesos[process]);
            //sleep(2);
        } else if (return_pid == ListaProcesos[process]) {
            /* child is finished. exit status in   status */

            //printf("terminó linea %i\n", contador_linea);
            //printf("Soy %i y mi hijo generador %i con pid: %i termino\n", getpid(),id, ListaProcesos[process]);
            //printf("lista de proceso %i\n ------------\n", lista[process]);
            contabilizador++;
            // Solo debo escribir sobre el archivo contador_linea.csv lo del archivo lista[porcess].csv
            //abrir archivo del proceso
            char fhijo_name[255];
            sprintf(fhijo_name,"%i.csv", lista[process]);
            FILE* fhijo = fopen(fhijo_name,"r");
            char linea[255];
            
            char farchivo_name[255];
            sprintf(farchivo_name,"%i.csv", contador_linea);
            FILE* farchivo = fopen(farchivo_name,"a");
            while(fgets(linea, 255, fhijo) != NULL){
              fprintf(farchivo, "%s",linea);
              //printf("la linea de %i.txt dice %s------------\n",lista[process],linea);
            }
            fclose(fhijo);
            fclose(farchivo);

            //free(PROCESO -> listaHijos);
            //free(PROCESO);
        }
      }
      if(contabilizador >= num){
        break;
      }
    };
  }
}

int main(int argc, char *argv[])
{
  // Este programa recibe tres parametros:
  //  argv[0] = el programa en si
  //  argv[1] = la ruta al archivo de input
  //  argv[2] = la linea por la cual iniciare el programa
  if (argc != 3)
  {
    printf("Cantidad de parámetros incorrecta\n");
    printf("Uso correcto: %s PATH_TO_INPUT LINE_N\n", argv[0]);
    return 1;
  }
  
  char* filename = argv[1];
  int linea = atoi(argv[2]);
  // Abrimos el archivo
  funcionProcesos(filename, linea);
  
  return 0;
}
