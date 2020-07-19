// Indica que este archivo solo se debe incluir una vez en la compilación
#pragma once

struct Persona;
  struct Persona
  {
    int id_persona;
    int tipo;
  };
  typedef struct Persona Persona;
  
  struct colapersonas;
  struct colapersonas
  {
    Persona** people;
    int capacidad;
    int largo;
  };
  typedef struct colapersonas Cola;
  
  struct Puerta;
  struct Puerta
  {
    int id_puerta;
    int estado;
    int countPods;
    Cola* cola;
  };
  typedef struct Puerta Puerta;
  
  struct listaPuertas;
  struct listaPuertas
  {
   Puerta** door;
   int largo;
  };
  typedef struct listaPuertas Lista;

  struct Terminal;
  struct Terminal
  {
    int id_terminal;
    int estado;
    Lista* lista;
  };
  typedef struct Terminal Terminal;
  
  struct Titanic;
  struct Titanic
  {
   Terminal** station;
   int largo;
  };
  typedef struct Titanic Titanic;


// Declaramos las funciones asociadas

/** Inicializar un titanic con terminal_count terminales*/
Titanic* titanic_init(int terminal_count);
/** Inicializar un terminal con N puertas con cola vacía incluida*/
Terminal* terminal_init(int term, int gate_count);
/** Inicializar una persona con id passenger y prioridad priority*/
Persona* persona_init(int passenger_id, int priority);
/** F al titanic */
void glaciar(Titanic* terminales);
/** Inserta un nuevo elemento al final de la lista */
//void list_append(Cola* list, int value);
/** Imprime todos los elementos de la lista */
//void list_print(Cola* list);
/** Libera todos los recursos asociados a esta lista */
//void list_destroy(Cola* list);
