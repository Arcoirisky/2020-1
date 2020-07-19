#include "leerArchivos.h"

crFILE* obtenerFile(unsigned disk, char* filename){
    int existe = cr_exists(disk,filename);
    if(existe == 1){
        disk--;
        for(int i = 0; i < diskMount->partitions[disk]->largo;i++){
            if(string_equals(diskMount->partitions[disk]->files[i]->nombre,filename)){
                return diskMount->partitions[disk]->files[i];
            }
        }
    }
    else{
        return NULL;
    }
}

int cr_close(crFILE* file_desc){
    if(file_desc == NULL){
        printf("No puedo cerrar un archivo no abierto\n");
        return -1;
    }
    else{   
        if (file_desc-> fileMode == OpenRead || file_desc-> fileMode == OpenWrite){
            printf("cerrando archivo %s\n",file_desc->nombre);
            //libero la memoria del buffBloque
            free(file_desc->lectBuff->buffBloque);
            free(file_desc->lectBuff->punteros);
            //reinicio valores
            file_desc -> lectBuff->buffBloque = NULL;
            file_desc -> lectBuff->punteros = NULL;
            file_desc -> lectBuff->pBloque = 0;
            file_desc -> lectBuff->pByte = 0;
            file_desc -> lectBuff->bytesPorLeer = BLOCK_SIZE;
            file_desc -> lectBuff->bytesLeidos = 0;
            file_desc -> lectBuff->len_punteros = 0;
            file_desc -> fileMode = CloseFile;
            return 0;
        }
        else{
            printf("No puedo cerrar un archivo no abierto\n");
            return -1;
        }
    }
}

crFILE* cr_open(unsigned disk, char* filename, char mode){
    // Necesito una funcion que me saque el archivo
    crFILE* file = obtenerFile(disk, filename);
    if(file && mode == 'r'){
        printf("el archivo %s existe... modo LECTURA\n",file->nombre);
        if(file ->typeLink == SoftLink && file->linkRoto == InConnect){
            printf("Archivo existe pero es un Link Roto, imposible leer, retorno nulo...\n");
            return NULL;
        }
        file -> fileMode = OpenRead;
        file -> lectBuff-> buffBloque = malloc(sizeof(unsigned char) * BLOCK_SIZE); // entonces pido la memoria para inicializar mi buffBloque
        //Necesito cargar los punteros...
        if(file ->lectBuff->dirSim==0){
            // significa que solo tengo direccionamiento directo 2044 punteros
            file -> lectBuff-> punteros = malloc(sizeof(unsigned int) * 2044);
            //ahora debo llenar este array con mis punteros asignados en el bloquedir del archivo...
            leer_punterosDireccion(file, false);
        }
        else{
            // el puntero al indireccionamiento simple existe
            file -> lectBuff-> punteros = malloc(sizeof(unsigned int) * (2044+2048));
            leer_punterosDireccion(file, true);

        }
        // ahora es cuando cargo el buffer con el primer bloque listo para leer
        cargar_bloqueSig(file);

        //retorno el archivo po
        return file;
    }
    else{
        // ojo cuidado que aun no hago el modo write
        if(file && mode == 'w'){
            // ...
        }
        
        printf("Archivo no existe, retorno nulo...\n");
        return NULL;
    }
}

void cargar_bloqueSig(crFILE* archivo){
    /* 
    Los archivos deberian de tener o no un punteo IndirectoSimple a menos que sea un softLink roto...
    */
    if(!softLinkRec(archivo) || archivo->linkRoto==Connect){
        if(archivo->lectBuff->pBloque < archivo->lectBuff->len_punteros){
            //aun tengo punteros para seguir
            unsigned int bloque = archivo->lectBuff->pBloque;
            int disk = particion_Archivo(archivo);
            
            FILE* file = fopen(diskName, "rb");
            //unsigned int index = 65536*disk;
            unsigned int puntero = archivo->lectBuff->punteros[bloque];
            //printf("puntero %i index %i resta %i\n",puntero,index,puntero+index);
            
            fseek(file, ((unsigned int) ((puntero) * BLOCK_SIZE)), SEEK_SET);
            fread(archivo->lectBuff->buffBloque, sizeof(unsigned char), BLOCK_SIZE, file);
            archivo->lectBuff->pByte = 0;
            archivo->lectBuff->pBloque++;
            if(archivo->lectBuff->pBloque == archivo->lectBuff->len_punteros){
                archivo->lectBuff->bytesPorLeer = (archivo->tamano) - (archivo->lectBuff->bytesLeidos);
            }
            else{
                archivo->lectBuff->bytesPorLeer=BLOCK_SIZE;
            }
            fclose(file);
        }
        else{
            // No puedo seguir leyendo más punteros...
        }
    }
    else{
        // Link roto, no hay nada que llenar
    }
}

int cr_read2(crFILE* archivo, void* buffer, int nbytes){
    if(archivo == NULL){
       printf("archivo nulo, no se puede leer nada...\n");
       return -1;
    }
    int leidos = 0;
    int total = nbytes;
    int punt = 0;
    while(punt < archivo ->lectBuff->len_punteros && leidos != total){
        nbytes = BLOCK_SIZE;
        if(archivo->lectBuff->bytesLeidos + nbytes > archivo->tamano){
          nbytes = archivo->tamano - archivo->lectBuff->bytesLeidos; //bytes restantes a leer...
        }

        int pos_actual = archivo->lectBuff->pByte;
        for(int i = 0; i < nbytes; i++){
            ((char*)buffer)[punt] = archivo ->lectBuff->buffBloque[pos_actual+i];
            leidos++;
            archivo->lectBuff->pByte++;
            archivo->lectBuff->bytesPorLeer--;
            archivo->lectBuff->bytesLeidos++; 
            if(leidos >= total){
                return leidos;
            }
            punt++;    
        }
        
        if(archivo->lectBuff->bytesPorLeer == 0 && archivo->lectBuff->pBloque < archivo->lectBuff->len_punteros){
           cargar_bloqueSig(archivo);
        }
    }
    return leidos;
    
}

int cr_read(crFILE* file_desc, void* buffer, int nbytes){
    if(file_desc == NULL){
       printf("archivo nulo, no se puede leer nada...\n");
       return -1;
    }
    if(file_desc->lectBuff->bytesLeidos == file_desc->tamano){
        //Me he quedado sin archivo para leer
        return 0;
    }

    if(nbytes <= file_desc -> lectBuff -> bytesPorLeer){
       // la cantidad de bytes a leer es menos a la que me quedan disponibles en mi buffer...
       if(file_desc->lectBuff->bytesLeidos + nbytes > file_desc->tamano){
           nbytes = file_desc->tamano - file_desc->lectBuff->bytesLeidos; //bytes restantes a leer...
       }

       int pos_actual = file_desc->lectBuff->pByte;
       for(int i = 0; i < nbytes; i++){
           ((char*)buffer)[i] = file_desc ->lectBuff->buffBloque[pos_actual+i];
           //printf("%X", ((char*)buffer)[i]);
       }
       printf("\n");

       file_desc->lectBuff->pByte += nbytes;
       file_desc->lectBuff->bytesPorLeer -= nbytes;
       file_desc->lectBuff->bytesLeidos += nbytes; 
       /*
       En caso de que bytesPorLeer llegue a 0 debo actualizar mi buffBloque con el nuevo bloque...
       en caso de que pBloque == len_punteros entonces no puedo leer más...
       */
       if(file_desc->lectBuff->bytesPorLeer == 0 && file_desc->lectBuff->pBloque < file_desc->lectBuff->len_punteros){
           cargar_bloqueSig(file_desc);
       }

       return nbytes;

   }
   else{
       // la cantidad de bytes a leer es mayor a la que me quedan disponibles en mi pobre buffer...
       int leidos = 0;
       // Como ya sé que no tengo suficiente en mi buffer entonces...
       //printf("estoy en el ELSE %i\n",nbytes);
       //printf("bytes por leer en bloque %i\n",file_desc->lectBuff->bytesPorLeer);
       while(nbytes > 0){
            int pos_actual = file_desc->lectBuff->pByte;
            int bPorLeer = file_desc->lectBuff->bytesPorLeer;
            for(int i = 0; i < file_desc->lectBuff->bytesPorLeer; i++){
                ((char*)buffer)[i] = file_desc ->lectBuff->buffBloque[pos_actual+i];
                leidos++;
                nbytes--;
                file_desc->lectBuff->pByte++;
                file_desc->lectBuff->bytesLeidos++;
                file_desc->lectBuff->bytesPorLeer--;
                if(file_desc->lectBuff->bytesLeidos == file_desc->tamano){
                    //Me he quedado sin archivo para leer
                    nbytes = 0;
                    cargar_bloqueSig(file_desc);
                    return leidos;
                }
                if(nbytes == 0){
                    return leidos;
                }
            }
            if(file_desc->lectBuff->bytesPorLeer == 0 && file_desc->lectBuff->pBloque < file_desc->lectBuff->len_punteros){
                cargar_bloqueSig(file_desc);  
            }
       }
       
       return leidos;
   }
   return -1;
}

void leer_punterosDireccion(crFILE* archivo, bool indirectoSimple){
    //printf("Archivo %s particion %i: \n",archivo->nombre, archivo->particion);
    if(archivo->typeLink == NormalFile || archivo ->typeLink ==HardLink || archivo->linkRoto == Connect){
        int disk = particion_Archivo(archivo);
        unsigned int bloque = archivo->pos_BloqueIndice;

        FILE* file = fopen(diskName, "rb");
        //unsigned int index = 65536*disk;
        fseek(file, ((unsigned int) ((bloque) * BLOCK_SIZE)), SEEK_SET);
        unsigned char* byteIndice = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        fread(byteIndice, sizeof(unsigned char), BLOCK_SIZE, file);

        unsigned char* byteIndirecSim = malloc(sizeof(unsigned char) * BLOCK_SIZE);
        if(indirectoSimple){
            unsigned int indirect_bloque = archivo->lectBuff->dirSim;
            
            fseek(file, ((unsigned int) ((indirect_bloque) * BLOCK_SIZE)), SEEK_SET);
            fread(byteIndirecSim, sizeof(unsigned char), BLOCK_SIZE, file);
        }
        fclose(file);

        int pos_aux = 0;
        for(int i = 12; i < 8176+12; i+=4){
            int aux = size_Archivo2(byteIndice,i,4);
            if(aux){ //solo guardo las mayores a cero... debo ahora ver bien que este usando todos los punteros...
                //printf("%i\n",aux - index);
                archivo->lectBuff->punteros[pos_aux] = aux;
                archivo->lectBuff->len_punteros++;
                pos_aux++;
            }
        }
        
        if(indirectoSimple){
            for(int i = 0; i < BLOCK_SIZE; i+=4){
                int aux = size_Archivo2(byteIndirecSim,i,4);
                if(aux){ //solo guardo las mayores a cero... debo ahora ver bien que este usando todos los punteros...
                    //printf("%i\n",aux - index);
                    archivo->lectBuff->punteros[pos_aux] = aux;
                    archivo->lectBuff->len_punteros++;
                    pos_aux++;
                }
            }
        }
        //printf("%i solo para comprobar. len: %i\n",pos_aux, archivo->lectBuff->len_punteros);
        free(byteIndice);
        free(byteIndirecSim);
        
    }
    else{
        printf("LINK ROTO, imposible leer contenido :C\n");
        
    }
}