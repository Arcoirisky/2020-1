#include "cargarArchivos.h"

void write_bitmap(unsigned int index, unsigned int value)
{
	unsigned char *byte = malloc(sizeof(unsigned char));
	int offset = 7 - index % 8;

	FILE *file = fopen(diskName, "rb+");
	fseek(file, BLOCK_SIZE + index / 8, SEEK_SET);
	fread(byte, sizeof(unsigned char), 1, file);

	if (value == 1) byte[0] |= (1u << offset);
	else byte[0] &= ~(1u << offset);

	fseek(file, BLOCK_SIZE + index / 8, SEEK_SET);
	fwrite(byte, sizeof(unsigned char), 1, file);

	free(byte);
	fclose(file);
}


int siguiente_BloqueVacio(unsigned int disk){
    disk--;
    FILE* file = fopen(diskName, "rb");
    unsigned int index = 65536*(disk);
    // el inicio del bloque direccion + bloque_size = inicio bloque bitmap
    fseek(file, ((unsigned int) ((index) * BLOCK_SIZE + BLOCK_SIZE)), SEEK_SET);
    unsigned char* bytemap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
    fread(bytemap, sizeof(unsigned char), BLOCK_SIZE, file);
    fclose(file);

    int cero_counter = 0;
    int aux;
    char *binary_number = malloc(sizeof(unsigned char) * 9);
    int i = 0;
    int counter;
    int posicion = -1;
    for (i; i < 8192; i++) {
        counter = 0;
        for (int bit = (sizeof(unsigned char) * 7); bit >= 0; bit--) {
            aux = bytemap[i] >> bit;
            
            if (aux & 1) {
                binary_number[counter] = '1';
                
            }
            else{
                binary_number[counter] = '0';
                cero_counter++;
                if(posicion == -1){
                    posicion = counter;
                }   
            } 
            counter++;
        }
	    binary_number[counter] = '\0';
        if(cero_counter>0){
            break;
        }
    }
    if (cero_counter <= 0){
        return -1;
    }
    //printf("%s, %i, %i, %i\n", binary_number, i, counter, posicion);
    int bloque = index + posicion + i * 8;
    int offset = 7-bloque %8;

    //printf("offset %i\n", 7-bloque %8);
    
    //printf("mi siguiente bloque libre es el %i\n", bloque);
    free(binary_number);
    free(bytemap);
    return bloque;
}

crFILE* file_Aux(unsigned int disk, char* origen){
    int dir_BloqueIndice = siguiente_BloqueVacio(disk);
    if(dir_BloqueIndice == -1){
        printf("NO HAY MAS ESPACIO EN LA PARTICION, F \n");
        return NULL;
    }
    crFILE* aux = montar_archivo(origen, dir_BloqueIndice);

    aux -> particion = disk-1; // de 0 a 3

    FILE* file_computador = fopen(origen,"rb");
    fseek(file_computador, 0, SEEK_END);
    aux -> tamano = ftell(file_computador);
    fseek(file_computador, 0, SEEK_SET);
    fclose(file_computador);
    

    return aux;
}

int cant_bloquesAUsar(int sizeFile){
    div_t respuesta;
    respuesta = div(sizeFile, BLOCK_SIZE);
    int bloques = respuesta.quot;
    if (respuesta.rem > 0){
        bloques++;
    }
    return(bloques);
}

void escribirPunterosIndirSim(crFILE* archivo, int bloques){
    printf("FALTAN %i PUNTEROS \n",bloques);
    unsigned char* bytePuntero = calloc(BLOCK_SIZE, sizeof(unsigned char));
    int ubic = 0;
    while(bloques && ubic < BLOCK_SIZE){
        int puntero = siguiente_BloqueVacio(archivo->particion+1);
        write_bitmap(puntero,1);
        bytePuntero[ubic] = (puntero >> (24)) & 0xFF;
        bytePuntero[ubic + 1] = (puntero >> (16)) & 0xFF;
        bytePuntero[ubic + 2] = (puntero >> (8)) & 0xFF;
        bytePuntero[ubic + 3] = (puntero >> (0)) & 0xFF;
        ubic+=4;
        bloques--;
    }
    printf("%i guarde indir\n",bloques);
    FILE *file = fopen(diskName, "rb+");
	fseek(file, (unsigned int) ((BLOCK_SIZE * archivo->lectBuff->dirSim)), SEEK_SET);
    fwrite(bytePuntero, sizeof(unsigned char), BLOCK_SIZE, file);
    fclose(file);
    free(bytePuntero);
}

void agregar_bloqueIndice(crFILE* archivo){
    //printf("Posicion del bloque indice %i\n", archivo->pos_BloqueIndice);
    write_bitmap(archivo->pos_BloqueIndice,1); //marco como ocupado
    // 4 bytes para las referencias, como estoy creandolo recien entonces es 1
    unsigned char* byteIndice = calloc(BLOCK_SIZE, sizeof(unsigned char));
    int tam = 4;
    int n = 1;
    int j = 8*(tam -1);
    for(int i = 0; i < tam; i++){
        
        byteIndice[i] = (n >> (j)) & 0xFF;
        j-=8;
    }
    //printf("4Bytes cant tot de ref: %i\n",size_Archivo2(byteIndice,0,4));
    // 8 bytes para el tamaño del archivo
    long int size = archivo->tamano;
    tam = 8;
    j = 8*(tam-1);
    for(int i = 4; i< 8+4; i++){
        byteIndice[i] = (size >> (j)) & 0xFF;
        j-=8;
    }
    //printf("8Bytes tamaño archivo: %i\n",size_Archivo2(byteIndice,4,8));
    // 8176 bytes para punteros (2044 punteros de 4 bytes cada uno)
    int bloques = cant_bloquesAUsar(archivo->tamano);
    bool indirex = false;
    if(bloques > 2044){
        indirex = true;
    }

    //printf("usa %i bloque(s)...\n", bloques);
    int ubic = 12;
    while(bloques && ubic < 8176 + 12){
        int puntero = siguiente_BloqueVacio(archivo->particion+1);
        write_bitmap(puntero,1);
        byteIndice[ubic] = (puntero >> (24)) & 0xFF;
        byteIndice[ubic + 1] = (puntero >> (16)) & 0xFF;
        byteIndice[ubic + 2] = (puntero >> (8)) & 0xFF;
        byteIndice[ubic + 3] = (puntero >> (0)) & 0xFF;
        ubic+=4;
        bloques--;
        
    }
    /* ME FALTA ESCRIBIR LOS BLOQUES */

    // 4 bytes para indireccionamiento indirecto
    if(indirex){
        archivo->lectBuff->dirSim = siguiente_BloqueVacio(archivo->particion+1);
        n = archivo->lectBuff->dirSim;
        escribirPunterosIndirSim(archivo, bloques);
    }
    else{
        n = 0;
    }
    tam = 4;
    j = 8*(tam -1);
    for(int i = 8176+12; i < BLOCK_SIZE; i++){
        byteIndice[i] = (n >> (j)) & 0xFF;
        j-=8;
    }
    
    //printf("4Bytes indirectoSimple: %i\n\n",size_Archivo2(byteIndice,8176+12,4));

    FILE *file = fopen(diskName, "rb+");
	fseek(file, (unsigned int) ((BLOCK_SIZE * archivo->pos_BloqueIndice)), SEEK_SET);
    fwrite(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
    fclose(file);
    free(byteIndice);

    archivo -> lectBuff-> buffBloque = malloc(sizeof(unsigned char) * BLOCK_SIZE);
    if(archivo ->lectBuff->dirSim==0){
        // significa que solo tengo direccionamiento directo 2044 punteros
        archivo -> lectBuff-> punteros = malloc(sizeof(unsigned int) * 2044);
        //ahora debo llenar este array con mis punteros asignados en el bloquedir del archivo...
        leer_punterosDireccion(archivo, false);
    }
    else{
        // el puntero al indireccionamiento simple existe
        archivo -> lectBuff-> punteros = malloc(sizeof(unsigned int) * (2044+2048));
        leer_punterosDireccion(archivo, true);
    }
}

void agregar_bloquesDatos(crFILE* archivo){
    FILE* original = fopen(archivo->nombre, "rb");
    FILE* disco = fopen(diskName, "rb+");
    //inicio intentando leer todo un bloque
    unsigned char* buff;
    int nbytes;
    int punt = 0;
    archivo->lectBuff->bytesPorLeer = archivo->tamano;
    while(punt < archivo ->lectBuff->len_punteros){
        nbytes = BLOCK_SIZE;
        if(archivo->lectBuff->bytesLeidos + nbytes > archivo->tamano){
          nbytes = archivo->tamano - archivo->lectBuff->bytesLeidos; //bytes restantes a leer...
        }

        buff = calloc(nbytes,sizeof(unsigned char));
        fread(buff,sizeof(unsigned char), nbytes, original);

        fseek(disco, (unsigned int) ((BLOCK_SIZE * archivo->lectBuff->punteros[punt])), SEEK_SET);
        fwrite(buff, sizeof(unsigned char), nbytes, disco);

        //archivo->lectBuff->pByte += nbytes;
        archivo->lectBuff->bytesPorLeer -= nbytes;
        archivo->lectBuff->bytesLeidos += nbytes; 
        printf("bytesporleer %i bytesleidos %i\n", archivo->lectBuff->bytesPorLeer, archivo->lectBuff->bytesLeidos);
        free(buff);
        punt++;
    }
    
    fclose(disco);
    fclose(original);
    
}

int cr_load(unsigned int disk, char* origen){
    printf("guardare el archivo %s en la particion %i...\n", origen, disk);
    if (disk > 0 && disk < 5){
        /* Extra:
        - Revisar que existe el archivo en orig...
        - ¿Que pasa si no tengo espacio en la particion?
        - No manejo errores de espacio ni esas cosas
        */
        // primero necesito encontrar un dir_BloqueIndice para mi nuevo archivo...
        crFILE* aux_new = file_Aux(disk, origen);
        /* agrega el archivo con su info al bloque directorio*/
        agregar_directorio(aux_new->particion, aux_new ->pos_BloqueIndice, aux_new->nombre);
        agregar_bloqueIndice(aux_new);
        
        printf("habemus %i punteros\n", aux_new->lectBuff->len_punteros);
        agregar_bloquesDatos(aux_new);
        //mientras no meta aux_new a ningun lado entonces tengo que liberarlo...
        free(aux_new->lectBuff->buffBloque);
        free(aux_new->lectBuff->punteros);
        free(aux_new->lectBuff);
        free(aux_new);
        
        // actualizo mi estructura
        desmontar_disco(diskMount);
        diskMount = montar_disco();
       return 0;
    }
    else{
        printf("Error al cargar archivo %s, el disco debe ser entre 1 y 4\n", origen);
        return -1;
    }
}