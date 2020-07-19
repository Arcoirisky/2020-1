#include "structs.h"
bool string_equals(char* string1, char* string2)
{
  return !strcmp(string1, string2);
}

crFILE* montar_archivo(char* filename, int dir_BloqueIndice){
    crFILE* new_file = malloc(sizeof(crFILE));
    strcpy(new_file -> nombre, filename);
    new_file -> pos_BloqueIndice = dir_BloqueIndice;
    new_file -> particion = -1;
    new_file -> typeLink = NormalFile; //Al inicio todo archivo se considerara normal
    new_file -> linkRoto = InConnect;
    new_file -> tamano = 0;

}

Disco* montar_disco(){
    Disco* disco = malloc(sizeof(Disco));
    disco -> nombre = diskName;
    disco -> largo = 4; // por enunciado
    disco -> partitions = malloc(sizeof(Particion*)*4);

    for(int i=0; i<4; i++){
        Particion* particion = malloc(sizeof(Particion));
        particion -> id = i + 1;
        particion -> capacidad = 256;
        particion -> largo = 0;
        particion -> files = malloc(sizeof(crFILE*)*(particion->capacidad)); // maximo puedo tener 256 bloque direccion en el bloque directorio
        // aqui estoy leyendo el archivo!
        int disk = i;
        int count_file = 0;
        unsigned char Directorio[32];
        FILE *ptr;
        ptr = fopen(diskName,"rb");

        unsigned int index = 65536*disk;
        for (int i = 0; i < 64; i++){
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
                        
                        int dir_BloqueIndice = pos_indice_block(Directorio);
                        crFILE* new_file = montar_archivo(name, dir_BloqueIndice);
                        new_file -> particion = particion -> id - 1;
                        particion -> files[count_file] = new_file;
                        particion -> largo++;
                        count_file++;
                        break;
                    }

                };
            }
        };
        fclose(ptr);
        disco ->partitions[i] = particion;
    };
    decifrar_infoFiles(disco);
    return disco;
}

void imprimir_disco(Disco* disk){
    for(int i = 0; i< 4; i++){
        printf("PARTICION %i - %i:\n\n",disk->partitions[i]->id,disk->partitions[i]->largo);
        for(int archivo = 0; archivo < disk -> partitions[i] -> largo; archivo++){
            crFILE* Aux_File = disk -> partitions[i]->files[archivo];
            printf("Name: %s - dirIndice: %i - size: %i \n", Aux_File->nombre, Aux_File->pos_BloqueIndice, Aux_File->tamano);
        }
        printf("------------------\n");
    }
}

void desmontar_disco(Disco* disk){
    for(int i = 0 ; i < 4; i++){
        for(int archivo = 0; archivo < disk -> partitions[i] -> largo; archivo++){
            free(disk -> partitions[i]->files[archivo]);
        }
        free(disk -> partitions[i] -> files);
        free(disk -> partitions[i]);
    }
    free(disk-> partitions);
    free(disk);
}

int pos_indice_block(unsigned char* Directorio){
	int aux;
	int counter = 0;
    int binario_total = 0;
    Directorio[0] = Directorio[0] << 1; // elimino el bit de validez con un shift left
    for (int i = 0; i < 3; i++){
        for (int bit = (sizeof(unsigned char) * 7); bit >= 0; bit--) {
            aux = Directorio[i] >> bit;
            if (aux & 1) {
                binario_total += pow(2,23-counter);
            }
            counter++;
        }
    }
	return binario_total;
};


void decifrar_infoFiles(Disco* disco){
    /*
    Debo recorrer todos los archivos de una partición
    Con la información que tengo hasta ahora un archivo tiene:
    -nombre
    -pos_BloqueIndice
    -particion
    -typeLink

    segun el nombre podremos ver que sean softlinks... es lo más simple hasta ahora así que demosle.
    la cantidad de referencias nos hace ver si es hardfile
    */
   for(int part = 0; part < 4; part++){
       for(int arch = 0; arch < disco->partitions[part]->largo; arch++){
           crFILE* archivo = disco->partitions[part]->files[arch];
           char* nombre = archivo->nombre;
           if(nombre[1] == '/'){
                archivo->typeLink = SoftLink;

                //printf("%c",nombre[1]);
                int partition_i = nombre[0] - '0';

                //printf("partition %i, %li\n",partition_i, strlen(nombre));
                int length = strlen(nombre);
                char sub[255];
                int c = 0;
                
                while (c < length) {
                    sub[c] = nombre[3+c-1];
                    c++;
                }
                sub[c] = '\0';
                
                for(int i = 0; i < disco->partitions[partition_i-1]->largo;i++){
                    if(string_equals(disco->partitions[partition_i-1]->files[i]->nombre,sub)){
                        // cuando existe link estable, debo conseguir el bloque Indice de ese archivo
                        archivo->linkRoto = Connect;
                        archivo->pos_BloqueIndice = disco->partitions[partition_i-1]->files[i]->pos_BloqueIndice;
                        break;
                    }

                }
                //printf("existe? %i -- ",archivo->linkRoto);
                //printf("nombre del puntero \"%s\"\n", sub); // '\"' to print "
           }
           else{
               int ref = cantidad_refArchivo(archivo);
               if (ref > 1){
                   archivo->typeLink = HardLink;
               }
           }
           // Meterle el tamaño del archivo
           int sizE = tamanio_File(archivo);
           archivo->tamano = sizE;
       }
   }
}

int size_Archivo2(unsigned char* bytes, int inicio, int leer){
	
	int aux;
	int counter = 0;
    int binario_total = 0;
    for (int i = inicio; i < inicio+leer; i++){
        for (int bit = (sizeof(unsigned char) * 7); bit >= 0; bit--) {
            aux = bytes[i] >> bit;
            if (aux & 1) {
                binario_total += pow(2,(8*leer-1)-counter);
            }
            counter++;
        }
    }
	return binario_total;
};

bool softLinkRec(crFILE* archivo){
    if(archivo->typeLink == NormalFile || archivo ->typeLink ==HardLink){
        return false;
    }
    else{
        return true;
    }
}

int particion_Archivo(crFILE* archivo){
    int disk;
    if(softLinkRec(archivo)){
        char* nombre = archivo->nombre;
        disk = nombre[0] - '0';
        disk--;
    }
    else{
        disk = archivo -> particion;
    }
    return disk;
}

int cantidad_refArchivo(crFILE* archivo){
    if(archivo->linkRoto==Connect){
        int disk = particion_Archivo(archivo);
        unsigned int bloque = archivo->pos_BloqueIndice;
        FILE* file = fopen(diskName, "rb");
        unsigned int index = 65536*disk;
        fseek(file, ((unsigned int) ((bloque-index) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);
        
        int ref = size_Archivo2(byteIndice,0,4);
        free(byteIndice);
        return ref;
    }
    else{
        return 1; //No estoy segura de si es 0 o 1, lo dejare como uno
    }
    
}

int tamanio_File(crFILE* archivo){
    /*
    Los archivos deberian de tener un tamaño mayor a cero A MENOS que sea un softLink roto...
    */
    if(!softLinkRec(archivo) || archivo->linkRoto==Connect){
        int disk = particion_Archivo(archivo);
        unsigned int bloque = archivo->pos_BloqueIndice;
        FILE* file = fopen(diskName, "rb");
        unsigned int index = 65536*disk;
        fseek(file, ((unsigned int) ((bloque-index) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);

        int sizE = size_Archivo2(byteIndice,4,8);
        free(byteIndice);
        return sizE;
    }
    else{
        return 0; // Archivo vacio pos men
    }
}

void leer_bloqueDireccion_aux(crFILE* archivo){
    printf("Archivo %s particion %i: \n",archivo->nombre, archivo->particion);
    if(archivo->typeLink == NormalFile || archivo ->typeLink ==HardLink){
        int disk = archivo -> particion;
        unsigned int bloque = archivo->pos_BloqueIndice;

        FILE* file = fopen(diskName, "rb");
        unsigned int index = 65536*disk;
        fseek(file, ((unsigned int) ((bloque-index) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);

        printf("4Bytes cant tot de ref: %i\n",size_Archivo2(byteIndice,0,4));
        printf("8Bytes tamaño archivo: %i\n",size_Archivo2(byteIndice,4,8));
        
        /*
        Los punteros son conjunto de 4 bytes de los 8176 bytes siguientes de byteIndice
        
        for(int i = 12; i < 13; i++){
            int aux = size_Archivo2(byteIndice,i,4);
            if(aux){
                printf("%i\n",aux - index);
            }
        }
        */
        printf("4Bytes indirectoSimple: %i\n\n",size_Archivo2(byteIndice,8176+12,4));
        free(byteIndice);
    }
    else{
        if (archivo->linkRoto == Connect){
            char* nombre = archivo->nombre;
            int disk = nombre[0] - '0';
            disk--;
            
            unsigned int bloque = archivo->pos_BloqueIndice;

            FILE* file = fopen(diskName, "rb");
            unsigned int index = 65536*disk;
            fseek(file, ((unsigned int) ((bloque-index) * BLOCK_SIZE)), SEEK_SET);
            unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
            fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
            fclose(file);

            printf("4Bytes cant tot de ref: %i\n",size_Archivo2(byteIndice,0,4));
            printf("8Bytes tamaño archivo: %i\n",size_Archivo2(byteIndice,4,8));
            
            /*
            Los punteros son conjunto de 4 bytes de los 8176 bytes siguientes de byteIndice
            
            for(int i = 12; i < 13; i++){
                int aux = size_Archivo2(byteIndice,i,4);
                if(aux){
                    printf("%i\n",aux - index);
                }
            }
            */

            printf("4Bytes indirectoSimple: %i\n\n",size_Archivo2(byteIndice,8176+12,4));
            free(byteIndice);
        }
        else{
            printf("LINK ROTO, imposible leer contenido :C\n");
        }
        

    }
}