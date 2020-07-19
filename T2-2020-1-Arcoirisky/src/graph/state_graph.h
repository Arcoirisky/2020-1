#pragma once

#include "../state/state.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/** Variable global: Archivo donde se irán guardando los histogramas */
FILE* histograms_file;
/** Variable global: Archivo donde se irán guardando los valores de hash */
FILE* hashes_file;
/** Variable global: Archivo donde se irán guardando las demás estadísticas */
FILE* general_stats_file;


/** Un nodo dentro del grafo para un estado dado */
struct state_node;
/** Un nodo dentro del grafo para un estado dado */
typedef struct state_node Node;

/** Un nodo dentro del grafo para un estado dado */
struct state_node
{
	/* El estado en sí */
	State state;
	/* El hash del estado */
	uint64_t hash_value;
	/* Arista al padre */
	Node* parent;
	/* Operación de la arista */
	Operation op;
};

struct lista_aux;
typedef struct lista_aux Lista;
struct lista_aux{
	Node** nodos;
	int largo;
	int capacidad;
};

/** Una tabla de hash que representa un grafo de estados */
struct state_graph
{
	/** Un arreglo de nodos que conforman la tabla de hash */
	Node** hash_table;
	Lista** hash_list;
	int list_size;
	int list_cap;
	/** Tamaño de la tabla de hash */
	int table_size;
	/** Cantidad de estados que tiene la tabla */
	int state_count;
};
/** Una tabla de hash que representa un grafo de estados */
typedef struct state_graph Graph;

/** Inicializa un grafo vacío */
Graph*   state_graph_init();
/** Crea un nuevo nodo para un estado, pero solo si ese estado no existía */
Node*    state_graph_new_node (Graph* graph, State state, Node* parent, Operation op);
/** Libera todos los recursos asociados a la tabla */
void     state_graph_destroy  (Graph* graph);
/** Calcula el hash de un estado */
uint64_t state_hash           (Graph* graph, State state);
