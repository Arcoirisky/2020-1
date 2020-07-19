#include "cr_API.h"
#include "leerArchivos.h"
#include <errno.h>

extern int errno;
int errnum;


void cr_ls(unsigned int disk){
    printf("\nEjecutando cr_ls(%i)...\n\n",disk);
    if (disk != 0 && disk > 0 && disk < 5){
        disk--;
        for(int i = 0; i < diskMount->partitions[disk]->largo;i++){
            printf("%s  ",diskMount->partitions[disk]->files[i]->nombre);
        }
        printf("\n");
    }
    else{
        if(disk == 0){
            for(int part = 0; part < 4; part++){
                printf("Particion %i:\n",part + 1);
                for(int i = 0; i < diskMount->partitions[part]->largo;i++){
                    printf("%s  ",diskMount->partitions[part]->files[i]->nombre);
                }
                printf("\n ------------- \n");
            }
        }
        else{
            printf("Error en cr_ls, elija un disco entre 1..4 o 0 para ver el disco completo\n");
        }
    }
    printf("\n");
}

int cr_exists(unsigned int disk, char* filename){
    if (disk != 0 && disk > 0 && disk < 5){
        disk--;
        for(int i = 0; i < diskMount->partitions[disk]->largo;i++){
            if(string_equals(diskMount->partitions[disk]->files[i]->nombre,filename)){
                return 1;
            }
        }
    }
    else{
        printf("Error en cr_exist, elija un disco entre 1..4\n");
    }
    return 0;
}

void cr_mount(char* diskname){
    FILE* file_test = fopen (diskname, "rb");
    if (file_test == NULL){
        errnum = errno;
        fprintf(stderr, "Error montando el disco: %s\n", strerror(errnum));
        exit(errnum);
    }
    else{
        fclose(file_test);
        diskName = diskname;
    }
}

int byte_to_Bits(unsigned char byte, bool hex){
	int ones_counter = 0;

	int aux;
	int counter = 0;
	char *binary_number = malloc(sizeof(unsigned char) * 9);
	for (int bit = (sizeof(unsigned char) * 7); bit >= 0; bit--) {
		aux = byte >> bit;
        
		if (aux & 1) {
			binary_number[counter] = '1';
			ones_counter++;
		}
		else binary_number[counter] = '0';
		counter++;
	}
	binary_number[counter] = '\0';
    if (hex == false){
	    printf("%s", binary_number);
    }
    else{
        printf("%X", byte);
    }
	free(binary_number);
	return ones_counter;
}

void cr_bitmap(unsigned disk, bool hex){
    /*
    Se supone que imprime en stderr el estado actual del bloque de bitmap
    correspondiente a disk perteneciente 1..4
    ya sea en binario (hex false) o hexadecimal (hex true)

    Disk = 0 imprime el bitmap completo
    imprimiento además una linea con la cant de bloques ocupados
    y otra linea con la cant de bloques libres

    Se ubica a continuación del bloque direccion
    */
    if (disk != 0 && disk > 0 && disk < 5){
        disk--;
        FILE* file = fopen(diskName, "rb");
        unsigned int index = 65536*(disk);
        // el inicio del bloque direccion + bloque_size = inicio bloque bitmap
        fseek(file, ((unsigned int) ((index) * BLOCK_SIZE + BLOCK_SIZE)), SEEK_SET);
        unsigned char* bytemap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(bytemap, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);
        int count_ones = 0;
        
        for (int i = 0; i < 8192; i++) {
            count_ones += byte_to_Bits(bytemap[i], hex);
        }
        
        printf("\nBloques ocupados %i\n", count_ones);
        printf("Bloques libres %i\n", 8192 * 8 - count_ones);

        free(bytemap);

    }
    else{
        /*
        Caso disk == 0
        */
       if(disk == 0){
            int count_ones = 0;
            FILE* file = fopen(diskName, "rb");
            unsigned char* bytemap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
            while(disk < 4){
                // inicio con disk == 0 normal
                unsigned int index = 65536*(disk);
                fseek(file, ((unsigned int) ((index) * BLOCK_SIZE + BLOCK_SIZE)), SEEK_SET);
                
                fread(bytemap, sizeof(unsigned char), BLOCK_SIZE, file);
                
                for (int i = 0; i < 8192; i++) {
                count_ones += byte_to_Bits(bytemap[i], hex);
                }
                disk++;
            }
            fclose(file);
            free(bytemap);
            printf("\nBloques ocupados %i\n", count_ones);
            printf("Bloques libres %i\n", 8192 * 8 * 4 - count_ones);

            
       }
       else{
           printf("\nError en cr_ls, elija un disco entre 1..4 o 0 para ver el disco completo\n");
       }
    }
}

int cr_rm(unsigned disk, char * filename){
    desmontar_disco(diskMount);
    diskMount = montar_disco(diskMount);
    crFILE* archivo;
    unsigned int index = 65536*(disk-1);
    unsigned int refs = 0;
    if (disk > 0 && disk < 5){
        archivo = obtenerFile(disk, filename);
        if (archivo != NULL){
            refs = cantidad_refArchivo(archivo); // Para obtener la cantidad de referencias al archivo
            
            if (softLinkRec(archivo)){
            // entonces es un softlink y solo se elimina la entrada del directorio
                
                FILE* file = fopen(diskName, "rb+");
                unsigned char dir[32];
                for (int i = 0; i < 256; i++){
                    fseek(file, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
                    fread(dir, sizeof(unsigned char), 32, file);

                    char aux_name[255]="";
                    char name[255]="";
                    if (dir[0]>>7 == 1){ // el primer bit es el de validez
                        for(int i=3; i<32;i++){
                            if(dir[i] != 0x0){
                                sprintf(aux_name, "%c",dir[i]);
                                sprintf(name, "%s%s",name,aux_name);
                            }
                        }
                    }
                    
                    if (string_equals(name, filename)){
                        
                        fseek(file, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
                        dir[0]= 0;
                        fwrite(dir, sizeof(unsigned char), 32, file);
                        break;
                    }
                }
                fclose(file);
            }                  
            else if (refs > 1){
                // Solo se elimina una referencia
                refs--;
                FILE* file = fopen(diskName, "rb+");
                fseek(file, ((unsigned int) (archivo->pos_BloqueIndice * BLOCK_SIZE)), SEEK_SET);
                //https://stackoverflow.com/questions/3784263/converting-an-int-into-a-4-byte-char-array-c
                unsigned char* new_refs = calloc(4, sizeof(unsigned int));
                new_refs[0] = (refs >> 24) & 0xFF;
                new_refs[1] = (refs >> 16) & 0xFF;
                new_refs[2] = (refs >> 8) & 0xFF;
                new_refs[3] = refs & 0xFF;
                fwrite(new_refs, sizeof(unsigned char), 4, file);
                // Luego se elimina la entrada en el directorio.
                unsigned char dir[32];
                for (int i = 0; i < 256; i++){
                    fseek(file, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
                    fread(dir, sizeof(unsigned char), 32, file);

                    char aux_name[255]="";
                    char name[255]="";
                    if (dir[0]>>7 == 1){ // el primer bit es el de validez
                        for(int i=3; i<32;i++){
                            if(dir[i] != 0x0){
                                
                                sprintf(aux_name, "%c",dir[i]);
                                sprintf(name, "%s%s",name,aux_name);
                            }
                        }
                    }
                    if (string_equals(name, filename)){
                        fseek(file, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
                        dir[0]= 0;
                        fwrite(dir, sizeof(unsigned char), 32, file);
                        break;
                    }
                }
                fclose(file);
            }
            else if (refs == 1){
                int count_ones = 0;
                unsigned char* bytemap = get_new_bytemap(disk, archivo->pos_BloqueIndice, filename); // Se obtiene nuevo bytemap
                free(bytemap);
            }
        }
        else{
            printf("Tu archivo no existe... cr_rm no puede eliminarlo...\n");
            return 1;
        }
    }
    desmontar_disco(diskMount);
    diskMount = montar_disco();
    return 0;
}

unsigned char* get_new_bytemap(unsigned int disk, int bloque_indice, char* filename){

    unsigned int index = 65536*(disk-1);
    unsigned int* lista_bloques = calloc(2045+2048, sizeof(unsigned int));
    unsigned int* n_bloque_actual = calloc(4, sizeof(unsigned int));
    FILE* file = fopen(diskName, "rb+");
    fseek(file, ((unsigned int) (bloque_indice * BLOCK_SIZE)), SEEK_SET);
    unsigned int byte_actual =0;
    unsigned int lowest =0; 
    unsigned int low =0;
    unsigned int high =0;
    unsigned int highest =0;
    unsigned int number =0; 
    unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
    fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
    for (int i = 0; i < 2045; i++){
        for (int j=0; j<4 ;j++){
            byte_actual = size_Archivo2(byteIndice,12+j+(4*i),1);
            n_bloque_actual[j] = byte_actual;   
        }
        lowest = n_bloque_actual[0] << 24;
        low = n_bloque_actual[1] << 16;
        high = n_bloque_actual[2] << 8;
        highest = n_bloque_actual[3] << 0;
        number = lowest + low + high + highest;
        lista_bloques[i] = number;
    
    }
    // Ahora se agregan los bloques de indireccion simple si hay
    int total_punteros = 2045;
    // printf("====lista_bloques[2044] = %d====\n", lista_bloques[2044]);
    if (lista_bloques[2044] != 0){
        fseek(file, ((unsigned int) (lista_bloques[2044] * BLOCK_SIZE)), SEEK_SET);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        for (int i = 0; i < 2048; i++){
            for (int j=0; j<4 ;j++){
                byte_actual = size_Archivo2(byteIndice,j+(4*i),1);
                n_bloque_actual[j] = byte_actual;   
            }
            lowest = n_bloque_actual[0] << 24;
            low = n_bloque_actual[1] << 16;
            high = n_bloque_actual[2] << 8;
            highest = n_bloque_actual[3] << 0;
            number = lowest + low + high + highest;
            lista_bloques[2045+i] = number;
            // printf("i= %d - number: %d\n", i, number);
        }
    total_punteros += 2048;
    }

    
    free(n_bloque_actual);
    // comenzar a cambiar la validez de bloques de lista_bloques en el bitmap
    fseek(file, ((unsigned int) ((index) * BLOCK_SIZE + BLOCK_SIZE)), SEEK_SET);
    unsigned char* bytemap = malloc(sizeof(unsigned char) * BLOCK_SIZE);
    fread(bytemap, sizeof(unsigned char), BLOCK_SIZE, file);
    //Modificar el bytemap
    int count_ones = 0;
    unsigned char* pos_en_bytes = calloc(total_punteros, sizeof(unsigned char));
    for (int i=0; i < 2044; i++){
         if (lista_bloques[i] > 0){
        pos_en_bytes[i] = (lista_bloques[i]-index)-8*((lista_bloques[i]-index)/8);
        bytemap[(lista_bloques[i]-index)/8] -= pow(2,7-pos_en_bytes[i]);
        // printf("puntero nro: %d, bloque: %d, pos_en_bytes: %d\n", i, lista_bloques[i]-index, pos_en_bytes[i]);
        }
    }
    // printf("end of first for\n");
    for (int i=2045; i < total_punteros; i++){
        if (lista_bloques[i] > 0){
            pos_en_bytes[i] = (lista_bloques[i]-index)-8*((lista_bloques[i]-index)/8);
            bytemap[(lista_bloques[i]-index)/8] -= pow(2,7-pos_en_bytes[i]);
            // printf("puntero nro: %d, bloque: %d, pos_en_bytes: %d\n", i, lista_bloques[i]-index, pos_en_bytes[i]);
        }
        
    }
    fseek(file, ((unsigned int) (bloque_indice * BLOCK_SIZE)), SEEK_SET);
    // Ahora cambiamos la validez del bloque indice principal
    int block_index_rel = bloque_indice - index;
    unsigned int pos_indice_in_byte = block_index_rel-8*(block_index_rel/8);
    // printf("indice en: %d - pos_en_byte+ %d", block_index_rel, pos_indice_in_byte);
    bytemap[block_index_rel/8]-= pow(2, 7-pos_indice_in_byte);

    fseek(file, ((unsigned int) ((index) * BLOCK_SIZE + BLOCK_SIZE)), SEEK_SET);
    fwrite(bytemap, sizeof(unsigned char), BLOCK_SIZE, file);
    /* finalmente cambiamos el bit de validez en la entrada */
    unsigned char dir[32];
    for (int i = 0; i < 256; i++){
        fseek(file, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
        fread(dir, sizeof(unsigned char), 32, file);

        char aux_name[255]="";
        char name[255]="";
        if (dir[0]>>7 == 1){ // el primer bit es el de validez
            for(int i=3; i<32;i++){
                if(dir[i] != 0x0){
                    
                    sprintf(aux_name, "%c",dir[i]);
                    sprintf(name, "%s%s",name,aux_name);
                }
            }
        }
        if (string_equals(name, filename)){
            fseek(file, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
            dir[0]= 0;
            fwrite(dir, sizeof(unsigned char), 32, file);
            break;
        }
    }
    fclose(file);
    free(byteIndice);
    free(pos_en_bytes);
    free(lista_bloques);
    return bytemap;
}

int cr_hardlink(unsigned int disk, char* orig, char* dest){
    if(disk != 0 && disk > 0 && disk < 5){
        int archivo_encontrado = 0;
        int posicion_archivo = -1;

        for (int i = 0; i < diskMount->partitions[disk-1]->largo; ++i)
        {
            if(string_equals(diskMount->partitions[disk-1]->files[i]->nombre, dest)){
                printf("\nya existe un archivo con este nombre en el directorio\n");
                return 1;
            }
                
        }

        for(int i = 0; i < diskMount->partitions[disk-1]->largo; i++){
            if(string_equals(diskMount->partitions[disk-1]->files[i]->nombre, orig)){
                archivo_encontrado = 1;
                posicion_archivo = i;
                break;
            }
        }

        if(archivo_encontrado){
            crFILE* nuevo_hardlink = montar_archivo(dest, diskMount->partitions[disk-1]->files[posicion_archivo]->pos_BloqueIndice);
            nuevo_hardlink->particion = disk-1;
            nuevo_hardlink->typeLink = HardLink;
            nuevo_hardlink->tamano = diskMount->partitions[disk-1]->files[posicion_archivo]->tamano;

            /// se agrega a la estructura que representa el disco el archivo hardlink creado
            diskMount->partitions[disk-1]->files[diskMount->partitions[disk-1]->largo] = nuevo_hardlink;
            diskMount->partitions[disk-1]->largo++;

            /// se aumenta el numero de referencias al archivo
            unsigned int bloque = nuevo_hardlink->pos_BloqueIndice;
            FILE* file = fopen(diskName, "rb");
            unsigned int index = 65536*(disk-1);
            fseek(file, ((unsigned int) ((bloque-index) * BLOCK_SIZE)), SEEK_SET);
            unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
            fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
            int ref = size_Archivo2(byteIndice,0,4)+1;

            // actualizacion en la matriz byteIndice
            for (int i = 3; i >= 0; --i)
            {
                byteIndice[i] = ref%256;
                ref /= 256;
            }

            fclose(file);
            

            FILE *file_update = fopen(diskName, "rb+");
            fseek(file_update, ((unsigned int) ((bloque-index) * BLOCK_SIZE)), SEEK_SET);
            fwrite(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file_update);

            fclose(file_update);

            //ref = size_Archivo2(byteIndice,0,4);
            free(byteIndice);



            /// se escribe en el bloque directorio info del archivo

            agregar_directorio(nuevo_hardlink->particion, nuevo_hardlink->pos_BloqueIndice, nuevo_hardlink->nombre);
            return 0;
        }

        else{
            printf("\nError en cr_hardlink, el archivo referenciado no se encuentra en el directorio indicado\n");
            return 1;
        }
    }

    else{
        printf("\nError en cr_hardlink, elija un disco entre 1..4 para crear un hardlink\n");
        return 1;
    }
}


int cr_softlink(unsigned int disk_orig, unsigned int disk_dest, char* orig, char* dest){
    if(disk_orig != 0 && disk_dest != 0 && disk_orig > 0 && disk_orig < 5 && disk_dest > 0 && disk_dest < 5){
        int archivo_encontrado = 0;
        int posicion_archivo = -1;

        for (int i = 0; i < diskMount->partitions[disk_orig-1]->largo; ++i)
        {
            if(string_equals(diskMount->partitions[disk_orig-1]->files[i]->nombre, orig)){
                archivo_encontrado = 1;
                posicion_archivo = i;
                break;
            }
            
        }



        if(archivo_encontrado){
            char softlink_name[255];

            sprintf(softlink_name, "%i", disk_orig);
            strcat(softlink_name, "/");
            strcat(softlink_name, dest);

            for (int i = 0; i < diskMount->partitions[disk_dest-1]->largo; ++i)
            {
                if(string_equals(diskMount->partitions[disk_dest-1]->files[i]->nombre, softlink_name)){
                    printf("\nya existe un archivo con este nombre en el directorio\n");
                    return 1;
                }
                
            }

            crFILE* nuevo_softlink = montar_archivo(softlink_name, 0);
            nuevo_softlink->particion = disk_dest;
            nuevo_softlink->typeLink = SoftLink;
            nuevo_softlink->linkRoto = Connect;
            nuevo_softlink->particion = disk_dest-1;
            //no se si asignar un tamaño al softlink va o no
            nuevo_softlink->tamano = diskMount->partitions[disk_orig-1]->files[posicion_archivo]->tamano; 
            
            ///se agrega el softlink creado a la estructura que representa el disco
            diskMount->partitions[disk_dest-1]->files[diskMount->partitions[disk_dest-1]->largo] = nuevo_softlink;
            diskMount->partitions[disk_dest-1]->largo++;


            agregar_directorio(nuevo_softlink->particion, nuevo_softlink->pos_BloqueIndice, nuevo_softlink->nombre);
            desmontar_disco(diskMount);
            diskMount = montar_disco(diskMount);
            
            return 0; // valor 0 es retorno exitoso
        }

        else{
            printf("\nError en cr_softlink, el archivo referenciado no se encuentra en el directorio indicado\n");
            return 1; // valor 1 es para error por no encontrar archivo o disco
        }
    }

    else{
        printf("\nError en cr_softlink, elija discos de origen y destino entre 1..4 para crear un softlink\n");
        return 1;
    }
}