#include "state_graph.h"
#include <stdio.h>

/****************************************************************************/
/*                              Estadísticas                                */
/****************************************************************************/

/* Cantidad total de colisiones */
uint64_t total_collisions;

/* Histograma de colisiones: donde la función de hash tiene una colision */
uint64_t* hash_histogram;

/* Histograma de colisiones: donde hay colisión debido al probing */
uint64_t* probing_histogram;

/* Cantidad de rehashings */
uint64_t resize_count;

/* Cantidad de veces que intentamos ver si un elemento existe */
uint64_t total_state_count;

/** Guarda los datos del histograma en un archivo */
void histograms_save_and_destroy(Graph* graph)
{
	if(histograms_file)
	{
		fprintf(histograms_file, "[");
		for(int i = 0; i < graph -> table_size; i++)
		{
			fprintf(
				histograms_file,
				"[%lu,%lu]",
				probing_histogram[i],
				hash_histogram[i]
			);
			if(i < graph ->table_size - 1)
			{
				fprintf(histograms_file, ",");
			}
		}
		fprintf(histograms_file, "],\n");
	}
	free(probing_histogram);
	free(hash_histogram);
}

/** Guardamos todas las estadísticas en sus respectivos archivos */
void statistics_save(Graph* graph)
{
	histograms_save_and_destroy(graph);

	if(general_stats_file)
	{
		fprintf(general_stats_file, "%lu,", total_state_count);
		fprintf(general_stats_file, "%d,", graph -> state_count);
		fprintf(general_stats_file, "%lu,", total_collisions);
		fprintf(general_stats_file, "%lu,", resize_count);
	}
}

/****************************************************************************/
/*                           Lógica de la tabla                             */
/****************************************************************************/


/** Obtiene el hash de un estado */
uint64_t state_hash(Graph* graph, State state)
{
	/* Hash en O(1) jeje */
        // porque retorna 0... siempre...
	return 0;
}

/** Ajusta el hash al tamaño de la tabla */
static uint64_t hash_table_adjust_hash(Graph* graph, uint64_t hash)
{
	/* Hacemos módulo, asi si el hash se sale de la tabla se da la vuelta */
	return hash % graph -> table_size;
}


/****************************************************************************/
/*                          Funciones de la tabla                           */
/****************************************************************************/

/** Inicializa la lista auxiliar por nodo del grafo */
Lista* init_lista(){
	Lista* new_lista = malloc(sizeof(Lista));
	new_lista ->capacidad = 7;
	new_lista->largo = 0;
	new_lista->nodos = calloc(new_lista->capacidad, sizeof(Node*));
	return new_lista;
}

/** Agregar nodo en la lista y resize la misma en cualquier caso */
void insertar_lista(Lista* mod_list, Node* new_nodo){
	int tengo = mod_list -> largo;
	int capacidad = mod_list -> capacidad;
	
	if(tengo < capacidad - 1){
		mod_list -> nodos[tengo] = new_nodo;
		mod_list -> largo += 1;
	}
	else{
		
		mod_list -> nodos = realloc(mod_list -> nodos, sizeof(Node*)*(mod_list -> capacidad +8));
		mod_list -> capacidad += 8;
		insertar_lista(mod_list, new_nodo);
	}
}

/** Inicializa un grafo vacío */
Graph* state_graph_init()
{
	Graph* graph = malloc(sizeof(Graph));

	/* Nuestra tabla tendra un tamaño inicial de 7, una potencia de 2, -1 */
	graph -> table_size = 7;
	graph -> list_size = 7;
	graph -> list_cap = 7;
	/* Calloc = inicializa el arreglo en 0. Eso significa que todo parte como NULL */
	graph -> hash_table = calloc(graph -> table_size, sizeof(Node*));
	graph -> hash_list = malloc(sizeof(Lista*)*graph->list_size);//calloc(graph -> table_size, sizeof(Lista*));
	for(int i = 0; i < graph -> list_cap; i++){
		graph -> hash_list[i] = init_lista();
	}
	/* Inicialmente no tenemos ningun estado */
	graph -> state_count = 0;

	/* Inicializamos las variables para generar métricas */
	total_collisions = 0;
	resize_count = 0;
	total_state_count = 0;
	probing_histogram = calloc(graph -> table_size, sizeof(uint64_t));
	hash_histogram = calloc(graph -> table_size, sizeof(uint64_t));

	return graph;
}
void rehashing_list(Graph* graph){
	//printf("resize!!1\n");
	resize_count++;
	int antiguo = graph->list_size;
	graph->list_size = 2 * (graph -> list_size + 1);
	//printf("list_size = %i vs antiguo %i \n",graph->list_size, antiguo);
	graph->hash_list = realloc(graph->hash_list, sizeof(Lista*)*(graph->list_size));	
	for(int i = antiguo; i < graph -> list_size; i++){
			graph -> hash_list[i] = init_lista();
	}
	//printf("done rehashoing /n");
}

void insertar_nodo(Graph* graph, Node* nodo, int index, uint64_t* new_collisions){
	while(graph->hash_list[index]->capacidad >= 8000){
		index++;
		if(index >= graph->list_size){
			rehashing_list(graph);
		}
	}
	insertar_lista(graph->hash_list[index], nodo);
}

/** Crea un nuevo nodo para un estado, pero solo si ese estado no existía */
Node*  state_graph_new_node (Graph* graph, State state, Node* parent, Operation op)
{
	/* Contamos que hemos intentado insertar un nuevo elemento */
	total_state_count++;

	/* Obtiene el hash del estado */
	uint64_t hash = state_hash(graph, state);

	/* Juntamos la información que contendrá el nuevo nodo */
	Node protonode = (Node)
	{
		.state = state,
		.hash_value = hash,
		.parent = parent,
		.op = op,
	};

	/* Indice en el que cae el nodo en la tabla */
	//uint64_t index = hash_table_adjust_hash(graph, protonode.hash_value);
	uint64_t index_org = hash_table_adjust_hash(graph, protonode.hash_value);
	// Siempre es cero...

	/* Cantidad de nuevas colisiones que se han generado en este paso */
	uint64_t new_collisions = 0;
	
	/* Si la celda ya está ocupada, reviso todos los elementos en la lista */
	// me da mucha extrañeza esto... ¿habra una forma más directa de hacerlo?
	// Si no reviso nada, entonces no pasa nada.
	// Meter todos los elementos no importan porque no hay estados distintos...
	// entonces no vale la pena recorrerlo o ¿si?
		

	/* Ya sabemos que el estado no está asique creamos un nuevo nodo para él */
	Node* new_node = malloc(sizeof(Node));
	/* Guardamos en este nuevo nodo la información necesaria */
	*new_node = protonode;
	/* Guardamos este nuevo nodo en la tabla*/
	//graph -> hash_table[index] = new_node;
	insertar_nodo(graph,new_node,index_org, &new_collisions);
	
	graph -> state_count++;
	/* Agregamos las colisiones al contador total */
	total_collisions += new_collisions;

	/* Guardamos el valor de este nuevo hash en el archivo de hashes */
	if(hashes_file) fprintf(hashes_file, "%lu\n", new_node -> hash_value);

	/* Retorna el puntero al nuevo nodo que creamos */
	return new_node;
}

/** Libera todos los recursos asociados a la tabla */
void state_graph_destroy(Graph* graph)
{
	/* Guardamos todas las estadisticas en sus archivos */
	statistics_save(graph);
	//printf("%i\n------------\n",graph->list_size);
	for(int j = 0; j < graph->list_size;j++){
		//printf("%i\n",graph->hash_list[j]->largo);
		for(int i = 0; i < graph->hash_list[j]->largo; i++){
			Node* node = graph->hash_list[j]->nodos[i];
			if(node){
				state_destroy(node->state);
				free(node);
			}
		}
		free(graph->hash_list[j]->nodos);
		free(graph->hash_list[j]);
	}
	free(graph->hash_list);

	printf("size = %i\n",graph->state_count);
	free(graph -> hash_table);
	free(graph);
}
