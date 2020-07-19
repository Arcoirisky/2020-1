#include "structs.h"

BufferLectura* buffer_init(){
    BufferLectura* new_buffer = malloc(sizeof(BufferLectura));
    new_buffer -> pBloque = 0;
    new_buffer -> pByte = 0;
    new_buffer -> bytesPorLeer = BLOCK_SIZE;
    new_buffer -> buffBloque = NULL;
    new_buffer -> punteros = NULL; // guarda la ubicacion real, sin el desplazo del index
    new_buffer -> dirSim = 0; // guarda la ubicacion real, sin el desplazo del index
    new_buffer -> bytesLeidos = 0;
    new_buffer -> len_punteros = 0;
    return new_buffer;
}
    

crFILE* montar_archivo(char* filename, int dir_BloqueIndice){
    crFILE* new_file = malloc(sizeof(crFILE));
    strcpy(new_file -> nombre, filename);
    new_file -> pos_BloqueIndice = dir_BloqueIndice;
    new_file -> particion = -1;
    new_file -> typeLink = NormalFile; //Al inicio todo archivo se considerara normal
    new_file -> linkRoto = InConnect;
    new_file -> fileMode = CloseFile;
    new_file -> tamano = 0;
    new_file -> lectBuff = buffer_init();

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
        for (int i = 0; i < 256; i++){
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
            printf("Name: %s - dirIndice: %i - size: %i - indirSimple: %i\n", Aux_File->nombre, Aux_File->pos_BloqueIndice, Aux_File->tamano, Aux_File->lectBuff->dirSim);
        }
        printf("------------------\n");
    }
}

void desmontar_disco(Disco* disk){
    for(int i = 0 ; i < 4; i++){
        for(int archivo = 0; archivo < disk -> partitions[i] -> largo; archivo++){
            free(disk -> partitions[i]->files[archivo]->lectBuff);
            free(disk -> partitions[i]->files[archivo]);
        }
        free(disk -> partitions[i] -> files);
        free(disk -> partitions[i]);
    }
    free(disk-> partitions);
    free(disk);
}


