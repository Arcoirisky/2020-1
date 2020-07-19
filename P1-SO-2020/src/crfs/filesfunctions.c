#include "filesfunctions.h"

int readBloque(crFILE* file_desc, void* buffer, int nbytes){
    if(file_desc == NULL){
       printf("archivo nulo, no se puede leer nada...\n");
       return -1;
    }
    if(file_desc->lectBuff->bytesLeidos == file_desc->tamano){
        //Me he quedado sin archivo para leer
        return 0;
    }
    if(file_desc->lectBuff->bytesLeidos + nbytes > file_desc->tamano){
        nbytes = file_desc->tamano - file_desc->lectBuff->bytesLeidos; //bytes restantes a leer...
    }

    int pos_actual = file_desc->lectBuff->pByte;
    for(int i = 0; i < nbytes; i++){
        ((char*)buffer)[i] = file_desc ->lectBuff->buffBloque[pos_actual+i];

    }

    file_desc->lectBuff->pByte += nbytes;
    file_desc->lectBuff->bytesPorLeer -= nbytes;
    file_desc->lectBuff->bytesLeidos += nbytes; 
    if(file_desc->lectBuff->bytesPorLeer == 0 && file_desc->lectBuff->pBloque < file_desc->lectBuff->len_punteros){
        cargar_bloqueSig(file_desc);
    }
    return nbytes;
}

void descargar_archivo(unsigned int disk, char* orig, char* dest){
    crFILE* aux_fil = cr_open(disk, orig,'r');
    if(aux_fil==NULL){
        printf("IMPOSIBLE DESCARGAR %s...\n",orig);
        return;
    }
    unsigned int leer_bytes = BLOCK_SIZE;
    
    unsigned char* aux_buffer = calloc(leer_bytes, sizeof(unsigned char));
    //cr_read(aux_fil, aux_buffer, leer_bytes);
    int leidos = readBloque(aux_fil, aux_buffer, leer_bytes);
    //archivo a escribir
    FILE *write_ptr = fopen(dest,"wb");
    while(leidos > 0){
        fwrite(aux_buffer, sizeof(unsigned char), leidos, write_ptr);
        leidos = readBloque(aux_fil, aux_buffer, leidos);   
    } 
    fclose(write_ptr);

    cr_close(aux_fil);
    free(aux_buffer);
}

char* nombreDest(crFILE* file){
    if(softLinkRec(file)){
        char* nombre = file->nombre;
        nombre[1] = '-';
        return nombre;
    }
    else{
        return file->nombre;
    }

}

int cr_unload(unsigned int disk, char* orig, char* dest){
    if (disk == 0){
        // copio todo el disco TODO
        for(int part = 0; part < 4; part++){
            cr_unload(part+1, NULL, NULL);
        }
        return 0;
        
    }
    else{
        if(orig == NULL){
            //copio la particion completa
            // ASUMIRE que dest es una carpeta o '/'
            int disco = disk - 1;
            for(int i=0; i < diskMount->partitions[disco]->largo; i++){
                char* name = diskMount->partitions[disco]->files[i]->nombre;
                char* nameDestino = nombreDest(diskMount->partitions[disco]->files[i]);
                char aux[255] = "";
                sprintf(aux, "%d_", diskMount->partitions[disco]->files[i]->particion);
                strcat(aux, nameDestino);
                printf("%s\n",aux);
                descargar_archivo(disk, name, aux);
                
            }
            return 0;
        }
        else{
            if(cr_exists(disk, orig)){
                // copio el archivo en la direccion dest de mi computador
                descargar_archivo(disk, orig, dest);
                printf("Archivo %s descargado...\n",dest);
                return 0;
            }
            else{
                printf("Tu archivo no existe en la particion indicada... imposible de descargar\n");
                return -1;
            }
        }
    }
}