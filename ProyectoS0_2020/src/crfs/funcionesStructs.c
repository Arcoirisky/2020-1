#include "funcionesStructs.h"

bool string_equals(char* string1, char* string2)
{
  return !strcmp(string1, string2);
}

int pos_indice_block(unsigned char* Directorio){
	int aux;
	int counter = 0;
    int binario_total = 0;
    Directorio[0] = Directorio[0] << 1; // elimino el bit de validez con un shift left
    Directorio[0] = Directorio[0] >> 1;
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
                        archivo->lectBuff->dirSim = punteroIndirectoSimple(archivo);
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
                   archivo->lectBuff->dirSim = punteroIndirectoSimple(archivo);
               }
           }
           // Meterle el tamaño del archivo
           int sizE = tamanio_File(archivo);
           archivo->tamano = sizE;
           archivo->lectBuff->dirSim = punteroIndirectoSimple(archivo);
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
        fseek(file, ((unsigned int) ((bloque) * BLOCK_SIZE)), SEEK_SET);
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
        fseek(file, ((unsigned int) ((bloque) * BLOCK_SIZE)), SEEK_SET);
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

int punteroIndirectoSimple(crFILE* archivo){
    /* 
    Los archivos deberian de tener o no un punteo IndirectoSimple a menos que sea un softLink roto...
    */
    if(!softLinkRec(archivo) || archivo->linkRoto==Connect){
        int disk = particion_Archivo(archivo);
        unsigned int bloque = archivo->pos_BloqueIndice;
        FILE* file = fopen(diskName, "rb");
        unsigned int index = 65536*disk;
        fseek(file, ((unsigned int) ((bloque) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);

        int puntIndSim = size_Archivo2(byteIndice,8176+12,4);
        free(byteIndice);
        return puntIndSim;
    }
    else{
        return 0; // Archivo vacio pos men
    }

}

void leer_bloqueDireccion(crFILE* archivo){
    printf("Archivo %s particion %i: \n",archivo->nombre, archivo->particion);
    if(archivo->typeLink == NormalFile || archivo ->typeLink ==HardLink){
        int disk = archivo -> particion;
        unsigned int bloque = archivo->pos_BloqueIndice;

        FILE* file = fopen(diskName, "rb");
        fseek(file, ((unsigned int) ((bloque) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
        fclose(file);

        printf("4Bytes cant tot de ref: %i\n",size_Archivo2(byteIndice,0,4));
        printf("8Bytes tamaño archivo: %i\n",size_Archivo2(byteIndice,4,8));
        
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
            fseek(file, ((unsigned int) ((bloque) * BLOCK_SIZE)), SEEK_SET);
            unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
            fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);
            fclose(file);

            printf("4Bytes cant tot de ref: %i\n",size_Archivo2(byteIndice,0,4));
            printf("8Bytes tamaño archivo: %i\n",size_Archivo2(byteIndice,4,8));
            
            printf("4Bytes indirectoSimple: %i\n\n",size_Archivo2(byteIndice,8176+12,4));
            free(byteIndice);
        }
        else{
            printf("LINK ROTO, imposible leer contenido :C\n");
        }
    }
}


void agregar_directorio(unsigned int disk, unsigned int bloque_indice, char* name){
    FILE* file = fopen(diskName, "rb");
    unsigned int index = 65536*disk;
    fseek(file, ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
    unsigned char* byteDirectorio = malloc(sizeof(unsigned char)*BLOCK_SIZE);
    fread(byteDirectorio, sizeof(unsigned char), BLOCK_SIZE, file);
    fclose(file);


    for (int i = 0; i < 256; ++i)
    {
        //printf("byte %i - %c: %i\n", i, byteDirectorio[i], byteDirectorio[i]);

        if(byteDirectorio[i*32]>>7 == 0){ //verificamos que el bit de validez sea cero para guardar

            //seteamos el bit de validez a 1 e ingresamos posicion del bloque indice en los primeros 3 bytes

            for (int j = 2; j >= 0; --j)
            {
                byteDirectorio[i*32 + j] = bloque_indice%256;
                bloque_indice /= 256;
            }

            // cambiamos el bit de validez a 1
            byteDirectorio[i*32] = byteDirectorio[i*32] | 128; //operador logico or del primer byte con 10000000

            //guardamos el nombre en los 29 bytes destinados para ello
            int contador = 0;
            for (int j = 32*i; j < 32*i + 29; ++j)
            {
                int letra;

                if(contador < strlen(name)){
                    letra = name[contador]; //obtengo el caracter ascii
                    byteDirectorio[j+3] = letra;
                }

                else{
                    byteDirectorio[j+3] = 0; //dejo los demas bytes en cero (NULL)
                }
                contador++;
            }

            break;
        }
    }

    FILE* file_update = fopen(diskName, "rb+");
    fseek(file_update, ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
    fwrite(byteDirectorio, sizeof(unsigned char), BLOCK_SIZE, file_update);
    fclose(file_update);

    free(byteDirectorio);
}
