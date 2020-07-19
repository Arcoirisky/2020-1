#include "cr_API.h"
#include <errno.h>

extern int errno;
int errnum;
unsigned int BLOCK_SIZE = 1024*8;


// https://nomadaselectronicos.wordpress.com/2015/03/05/manipulacion-de-bits-en-c-y-aplicaciones/
// BIT(x) regresa el bit x puesto a uno y los demas bits en cero, ej. BIT(3) regresa 00001000
#define BIT(x)         (1<<(x))
// BIT_GET(x,b) regresa el bit b-esimo de x ej. BIT_GET(PINC,3)
#define BIT_GET(x,b)   ((x) & BIT(b))


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
/*
void cr_ls(unsigned disk){
    
    //Ojo cuidado:

    //Así como está esta función, solo funciona con disk entre 1..4
    //cualquier otro valor no funciona ni tampoco tira error.

    //Dejo pendiente el manejo de errores y el cómo se usarán las funciones
    //para una pronta reunión y revisión.
    
    if (disk != 0 && disk > 0 && disk < 5){
        disk--; //con esto llevo disk a 0..3
        unsigned char Directorio[32]; //32 bytes de directorio
        FILE *ptr;
        ptr = fopen(diskName,"rb");  // r for read, b for binary
        
        unsigned int index = 65536*disk;
        printf("\n---------------------\nREADING BLOCK %i\n", index);
        for (int i = 0; i < 64; i++) {
            fseek(ptr, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
            fread(Directorio, sizeof(unsigned char), 32, ptr);
            
            if (Directorio[0]>>7 == 1){ // el primer bit es el de validez
                // se supone que los siguientes 23 bits es el numero de bloque donde se encuentra el bloque indice del archivo
                for(int i=3; i<32;i++){
                    if(Directorio[i] != 0x0){
                        printf("%c", Directorio[i]);
                    }
                    else{
                        printf("  ");
                        break;
                    }

                }
                
                //printf("-- Bloque Indice: ");
                //pos_indice_block(Directorio);
                //printf("\n");
                
            }
        };
        printf("\n");
        fclose(ptr);
    }
    else{
        
        //pregunta: ls disk == 0 ¿será error o ls a todo el disco?
        //ERROOOOOOOOOOOOOR
        
    }
}
*/
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

/*
int cr_exists(unsigned disk, char* filename){
    
    //Lo mismo que con cr_ls, funciona si y solo si disk esta entre 0 y 3,
    //incluyendo el 0 y el 3.

    //No hay memory leaks pero si hay que tener cuidado con los errores.
    
    if (disk != 0 && disk > 0 && disk < 5){
        disk--;
        unsigned char Directorio[32];
        FILE *ptr;
        ptr = fopen(diskName,"rb");
        
        unsigned int index = 65536*disk;
        printf("\n---------------------\nBUSCAR %s IN BLOCK %i\n", filename,index);
        for (int i = 0; i < 64; i++) {
            fseek(ptr, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
            fread(Directorio, sizeof(unsigned char), 32, ptr);
    
            char aux_name[255]="";
            char name[255]="";
            if (Directorio[0]>>7 == 1){ // el primer bit es el de validez
                for(int i=3; i<32;i++){
                    if(Directorio[i] != 0x0){
                        
                        sprintf(aux_name, "%c",Directorio[i]);
                        sprintf(name, "%s%s",name,aux_name);
                    }
                    else{
                        
                        if(string_equals(name,filename)){
                            fclose(ptr);
                            return 1;
                        }
                        break;
                    }

                }
            }
        };
        printf("\n");
        fclose(ptr);
    }
    else{
        
        //ERROOOOOOOOOOR
        
    }
    return 0;
}
*/

void cr_mount(char* diskname){
    /*
    Establece como variable global la ruta local donde se encuentra el
    archivo.bin correspondiente al disco
    */
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
	// Cuenta la cantidad de 1's
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
    //printf("\n");
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