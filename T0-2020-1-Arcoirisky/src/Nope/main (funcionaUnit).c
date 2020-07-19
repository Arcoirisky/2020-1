#include "structs.h"
#include "ingreso_terminal.h"
#include "clausura.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/** Retorna true si ambos strings son iguales */
bool string_equals(char* string1, char* string2)
{
  return !strcmp(string1, string2);
}

void simulate(char* filename)
{
  // Abrimos el archivo
  FILE* file = fopen(filename, "r");

  // Leemos la cantidad de terminales
  int terminal_count;
  fscanf(file, "%i", &terminal_count);

  // TODO: Inicializar estructura principal
  // esto esta en structs.h y .c
  Titanic* terminales = titanic_init(terminal_count);

  for (int term = 0; term < terminal_count; term++)
  {
    int gate_count; //- solo me dice la cantidad de puertas
    fscanf(file, "%i", &gate_count); //- aqui es donde saca el valor del txt

    // TODO: Inicializar terminal
    // Creo el terminal id term con gate_count puertas (cada una con sus colas vacias)
    // y capacidad de 10 entes
    Terminal* terminal_nuevo = terminal_init(term, gate_count);
    // Lo agrego a los terminales
    terminales -> station[term] = terminal_nuevo;
  }
  
  char command[32];

  while(true)
  {
    fscanf(file, "%s", command);

    if(string_equals(command, "END"))
    {
      break;
    }
    else if(string_equals(command, "INGRESO"))
    {
      int terminal, passenger_id, priority;

      fscanf(file, "%i %i %i", &terminal, &passenger_id, &priority);
      //printf("INGRESAR %i\n", passenger_id);
      // TODO: Procesar ingreso
      Persona* persona_nueva = persona_init(passenger_id, priority);

      //inicializo una variable posicion para poder modificarla a traves de punteros.
      int posicion = 0;
      int* pos = &posicion;
      int eleccion_puerta = elegir_puerta(terminales, terminal, priority, pos);

      insertar_cola(terminales -> station[terminal] -> lista -> door, eleccion_puerta,persona_nueva, posicion);
      // LISTO -YAAAY ULTRA MACHO-
    }
    else if(string_equals(command, "ABORDAJE"))
    {
      int terminal, gate;
      fscanf(file, "%i %i", &terminal, &gate);
      int index = 0;
      int pod = terminales -> station[terminal] -> lista -> door[gate] -> countPods;
      printf("POD %i %i %i LOG\n", terminal, gate, pod);
      for(int j= 0; j < 8; j++){
        printf("%i\n",terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[index] -> id_persona);
        free(terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[index]);
        int largo_laser = terminales -> station[terminal] -> lista -> door[gate] -> cola -> largo;
        for(int continua = index; continua < largo_laser -1; continua++){
          terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[continua] = terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[continua + 1];
        }
        terminales -> station[terminal] -> lista -> door[gate] -> cola -> largo -= 1;
      }
      terminales -> station[terminal] -> lista -> door[gate] -> countPods++;
      
      // TODO: Procesar abordaje
      //printf("Inicio del abordaje en el terminal %i puerta %i\n", terminal, gate);
    }
    else if(string_equals(command, "CIERRE"))
    {
      int terminal, gate;
      fscanf(file, "%i %i", &terminal, &gate);
      terminales -> station[terminal] -> lista -> door[gate] -> estado = 1;
      for(int total_p = 0; total_p < terminales -> station[terminal] -> lista -> door[gate] -> cola -> largo; total_p++){
        Persona* persona_aux = terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[total_p];
        int priority = persona_aux ->tipo;
        int posicion = 0;
        int* pos = &posicion;
        int eleccion_puerta = elegir_puerta(terminales, terminal, priority, pos);
      
        insertar_cola(terminales -> station[terminal] -> lista -> door, eleccion_puerta,persona_aux, posicion);
      }
      terminales -> station[terminal] -> lista -> door[gate] -> cola -> largo = 0;
      // TODO: Procesar cierre de puerta
      //printf("Cierre de puerta %i en terminal %i\n", gate, terminal);
      
    }
    else if(string_equals(command, "CLAUSURA"))
    {
      int term_out, term_in;
      fscanf(file, "%i %i", &term_out, &term_in);
      // TODO: Procesar clausura de terminal
      meter_entes(terminales,term_out,term_in,0);
      meter_entes(terminales,term_out,term_in,1);
      meter_entes(terminales,term_out,term_in,2);
      terminales -> station[term_out] -> estado = 1; 
      //printf("Clausura de terminal %i, todos los pasajeros proceder al terminal %i\n", term_out, term_in);
    }
    else if(string_equals(command, "LASER"))
    {
      int terminal, gate, index;
      fscanf(file, "%i %i %i", &terminal, &gate, &index);
      // TODO: Procesar laser
      //printf("MUERE %i\n", terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[index] -> id_persona);
      free(terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[index]);
      int largo_laser = terminales -> station[terminal] -> lista -> door[gate] -> cola -> largo;
      //printf("index %i\n", index);
      for(int continua = index; continua < largo_laser - 1; continua++){
        //printf("index: %i, index+1: %i\n",continua, continua+1);
        terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[continua] = terminales -> station[terminal] -> lista -> door[gate] -> cola -> people[continua + 1];
      }
      terminales -> station[terminal] -> lista -> door[gate] -> cola -> largo -= 1;
      //printf("Laser perdido impacta a la persona en indice %i de la fila para la puerta %i del terminal %i\n", index, gate, index);
      //imprimir_cola(terminales -> station[terminal] -> lista -> door[gate] -> cola);
    }
  }

  fclose(file);
  printf("TITANIC LOG\n");
  // TODO: Liberar recursos e imprimir los pasajeros que siguen a bordo
  
  for(int terminal=0; terminal < terminales -> largo; terminal++){
    if(terminales -> station[terminal] ->estado == 0){
      printf("TERMINAL %i\n",terminal);
      for(int puerta = 0; puerta < terminales -> station[terminal] -> lista -> largo; puerta++){
        if(terminales -> station[terminal] -> lista -> door[puerta] -> estado == 0){
          printf("GATE %i: %i\n", puerta, terminales -> station[terminal] -> lista -> door[puerta] -> cola -> largo);
          imprimir_cola(terminales -> station[terminal] -> lista -> door[puerta] -> cola);
        }
      }
    }
  }
  
  /** Libera todos los recursos asociados a esta lista */
  //- Liberar terminal
  glaciar(terminales);

  printf("END LOG\n");
}

int main(int argc, char *argv[])
{
  // Este programa recibe dos parámetros:
  //  argv[0] = el programa en sí
  //  argv[1] = la ruta al archivo de input
  if (argc != 2)
  {
    printf("Cantidad de parámetros incorrecta\n");
    printf("Uso correcto: %s PATH_TO_INPUT\n", argv[0]);
    return 1;
  }

  simulate(argv[1]);

  return 0;
}
