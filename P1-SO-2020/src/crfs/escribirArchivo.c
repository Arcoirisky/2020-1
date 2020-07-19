#include "escribirArchivo.h"
#include "funcionesStructs.h"
#include "structs.h"
#include "cr_API.h"
#include "leerArchivos.h"
#include "filesfunctions.h"
#include "cargarArchivos.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <ctype.h>
#include <math.h>

/** variables globales */
/*
unsigned int BLOCK_SIZE = 1024*8; // 8192bytes por bloque
Disco* diskMount;
char* diskName;
*/
int buscar_blocke_para_indice_de_file(int particion){
    int lugar_de_bitmap = particion*65536*BLOCK_SIZE+1*BLOCK_SIZE;   
    FILE *file = fopen(diskName, "rb+");
    // el inicio del bloque direccion + bloque_size = inicio bloque bitmap
    
    fseek(file, lugar_de_bitmap, SEEK_SET);
    unsigned char bytemap [8192];
    fread(bytemap, 1, BLOCK_SIZE, file);
    
    int count_ones = 0;
    int bite_donde_se_encuentra_bit_disponible = 0;// el nombre mas apropiado para esta var podria ser bite enque se encuentra el bit que indica la direccion del bloque disponible para colocar el blocke indice de la informacion
    int lugar_del_bit_disponible = 0;
    unsigned char bite_con_el_bit_uno_aniadido[1];
    for (int i = 0; i < 8192; i++) {
        //printf("bite del bitmap %x\n",bytemap[i]);
        //printf("resultado %x ",bytemap[i]&0b10000000);
        if ((bytemap[i]&0b10000000) == 0){ // el primer bit es el de validez
            printf("entro 1\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 0;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b10000000;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        else if((bytemap[i]&0b01000000) == 0){
            printf("entro 2\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 1;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b01000000;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        else if((bytemap[i]&0b00100000) == 0){
            printf("entro 3\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 2;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b00100000;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        else if((bytemap[i]&0b00010000) == 0){
            printf("entro 4\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 3;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b00010000;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        else if((bytemap[i]&0b00001000) == 0){
            printf("entro 5\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 4;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b00001000;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        else if((bytemap[i]&0b00000100) == 0){
            printf("entro 6\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 5;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b00000100;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        else if((bytemap[i]&0b00000010) == 0){
            printf("entro 7\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 6;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b00000010;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        else if((bytemap[i]&0b00000001) == 0){
            printf("entro 8\n");
            bite_donde_se_encuentra_bit_disponible = i;
            lugar_del_bit_disponible = 7;
            bite_con_el_bit_uno_aniadido[0] = bytemap[i]|0b00000001;
            printf("bite del bitmap %x\n",bite_con_el_bit_uno_aniadido[0]);
            break;
        }
        //printf("no hay espacio bite del bitmap %x\n",bytemap[i]);
    }
    
    //cambiar bit por 1 para hacer ver que se usara ese bit 
     
    fseek(file, lugar_de_bitmap+bite_donde_se_encuentra_bit_disponible , SEEK_SET);
    //añadirle el 1 al bite elegido en la posicion correcta
    fwrite (bite_con_el_bit_uno_aniadido,1 , 1, file);
    fclose(file);
    return 8*bite_donde_se_encuentra_bit_disponible+lugar_del_bit_disponible+1;//revisar
}



char* stringToBinary(char* s) {
    if(s == NULL) return 0; /* no input string */
    size_t len = strlen(s);
    char *binary = malloc(len*8 + 1); // each char is one byte (8 bits) and + 1 at the end for null terminator
    binary[0] = '\0';
    for(size_t i = 0; i < len; ++i) {
        char ch = s[i];
        for(int j = 7; j >= 0; --j){
            if(ch & (1 << j)) {
                strcat(binary,"1");
            } else {
                strcat(binary,"0");
            }
        }
    }
    return binary;
}



int buscar_bloque_director_libre(int particion, char *filename){
    unsigned char Directorio[32];
    FILE *ptr;
    ptr = fopen(diskName,"rb+");
    unsigned int index = 65536*particion;
    int ind_blocke_a_usar = 0;
    for (int i = 0; i < 256; i++){
        fseek(ptr, (32 * i) + ((unsigned int) ((index) * BLOCK_SIZE)), SEEK_SET);
        fread(Directorio, sizeof(unsigned char), 32, ptr);
        
        printf("algo del dir %x y i %d y direcero %x\n",Directorio[0]>>7,i,Directorio[0]);
        if (Directorio[0]>>7 == 0){ // el primer bit es el de validez
            ind_blocke_a_usar = i;
            printf("entro %i",i);
            break;
        }
    }
    printf("blocke a usar %i \n",ind_blocke_a_usar);
    if (ptr != NULL) {
	fseek(ptr,65536*particion*BLOCK_SIZE,SEEK_SET);
    unsigned char buffer[256];
    //para elegir un bloque nesesito revisar el bitmap
    int blocke_elegido = 6;
    //cr_bitmap(1,false);
    printf("separacion \n");
    blocke_elegido = buscar_blocke_para_indice_de_file(particion);
    printf("dir de blocke indice elegido %d\n",blocke_elegido);
    //cr_bitmap(1,false);
    char *nombre = filename;
    int length = strlen(nombre);//sizeof(nombre)/sizeof(char); // length=6
    int blocke_aux = blocke_elegido;
    for (int i = 0; i <= 23; i++){
        if(i==0){
            buffer[i]=0b1;
        }
        else if(i<=23){
            
            if((blocke_aux%2)==0){
                buffer[24-i]=0b0;
            }
            else{
                buffer[24-i]=0b1;
            }
            blocke_aux = (int) (blocke_aux/2);
        }
        
        printf("%x, ",buffer[i]);
    }
    int count_word = 0;
    int count_of_bit = 0;

    char *polo = stringToBinary(nombre);
    ////int nopuede = polo[0] - '0';
    //printf("bit %s \n",polo);

    
    for (int i = 24; i < 256; i++){

        if(count_of_bit <= (length*8)){
          
            char bit_char = stringToBinary(nombre)[count_of_bit];
            if(isdigit(bit_char)){
                int bit = bit_char - '0';
                buffer[i]= bit;
                //printf("entro");
            }
            else{
                buffer[i] = 0b0;
            }
            
            //printf("test %c ,%d\n",stringToBinary(nombre)[count_of_bit],buffer[i]);
        }
        else{
            buffer[i] = 0b0;
        }
        printf("%x, ",buffer[i]);
        count_of_bit = count_of_bit + 1;
    }

    
    unsigned char buffer2[32];

    int count_of_char = 0;
    int count = 0;
    buffer2[0] = 0b00000000;
    for (int i = 0; i < 256; i++){
        if(count==8){
            //printf("\ncambio taya---------");
            count = 0;
            count_of_char = count_of_char + 1;
            buffer2[count_of_char] = 0b00000000;
            buffer2[count_of_char] = buffer2[count_of_char]|(buffer[i]<<(7-count));
            
        }
        else{
            buffer2[count_of_char] = buffer2[count_of_char]|(buffer[i]<<(7-count));
            //printf("\n una mas ee");
        }
        //printf("\n%x, mira bit y %i y count %i y cambio de bite %i y el num %x",buffer[i],i,count,count_of_char,buffer2[count_of_char]);
        count = count + 1;
    }

    for(int j = 0; j < 32; j++){
        printf("\n mira auqi %x",buffer2[j]);
    }
    

    //int other_len = 256*32*8; ind_blocke_a_usar*256*1,65536*particion , ind_blocke_a_usar
    fseek(ptr,32*ind_blocke_a_usar+BLOCK_SIZE*65536*particion, SEEK_SET);
	fwrite (buffer2,1 , 32, ptr);
    fclose (ptr);
	  return blocke_elegido;
	}
    
  return -1;

}
char* prefix_to_string(char* first_string,char* second_string){
	char* name = first_string;
	char* extension = second_string;	
	char* name_with_extension;
	name_with_extension = malloc(strlen(name)*(sizeof(char))+strlen(extension)*(sizeof(char))); /* make space for the new string (should check the return value ...) */
	strcpy(name_with_extension, name); /* copy name into the new var */
	strcat(name_with_extension, extension); /* add the extension */
	return name_with_extension;
}

 crFILE *alter_open(unsigned disk, char* filename){
    
    //crFILE *bloqueindice;
    //printf("aqui pasa");
    
    if(cr_exists(disk,filename)==1){
        //printf("nombre repetido\n");
        filename = prefix_to_string("copy_",filename);
    }
    disk=disk-1;
    
    int dir_bloque_indice = buscar_bloque_director_libre(disk,filename);
    crFILE *new_file = montar_archivo(filename, dir_bloque_indice);
    new_file -> particion = disk;
    //new_file -> CloseFile;
    new_file -> tamano = 0;
    //new_file -> lectBuff = buffer_init;
    return new_file;
}

int escribir_bloque_de_dato(int particion,unsigned char *buffer_para_un_bloque_de_memoria,int dir_donde_escribir_bloque){
    FILE *bloque_dato;
    bloque_dato = fopen(diskName,"rb+");
    fseek(bloque_dato,dir_donde_escribir_bloque*BLOCK_SIZE+BLOCK_SIZE*65536*particion, SEEK_SET);
	fwrite (buffer_para_un_bloque_de_memoria,1 , 8192, bloque_dato);
    fclose (bloque_dato);
    return 8192;
}
void ReverseArray(char* arr, int size)
{
    for (int i = 0; i < size/2; ++i)
    {
        int temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}
char *dec_to_bin(int decimal,int porte_arreglo){
    char *bit = malloc(sizeof(char)*porte_arreglo);
    for(int i = 0; i < porte_arreglo; i++)
    {
        if((decimal%2)==0){
            bit[i]=0b0;//16*8-(4*8+1)=12*8-1,16*8-12*8=4*8
        }
        else{
            bit[i]=0b1;
        }
        
        decimal = (int)(decimal / 2);
    }
    printf("antes de la caga\n");
    for (int i = 0; i < porte_arreglo; i++)
    {

        //printf("%x ,",bit[i]);
    }
    printf("\nimprimir bin\n");
    ReverseArray(bit,porte_arreglo);
    for (int i = 0; i < porte_arreglo; i++)
    {

        //printf("%x ,",bit[i]);

    }
    return bit;
}



int cr_write(crFILE *file_desc,void *buffer, int nbytes){
    unsigned char *buffer_en_bite = (unsigned char*)buffer;
    
    unsigned char array_of_bite_of_block[8192*8];
    //saltarse 4 bite para link
    printf("\n aqui empieza la escritura\n");
    for (int j = 0; j < 4*8; j++)
    {
      array_of_bite_of_block[j]=0b0;
      printf("%x, ",array_of_bite_of_block[j]);
    }
    printf("\n separacion primera\n");
    //luego 8 para tamaño
    int tamanio_aux = nbytes;
    char *array_of_bit = dec_to_bin(tamanio_aux,8*8);
    printf("\n empieza traspado\n");
    for (int i = 4*8; i < 12*8; i++){
        
        array_of_bite_of_block[i]=array_of_bit[i-(4*8)];
        
        
        printf("%x, ",array_of_bite_of_block[i]);
    }
    
    free(array_of_bit);
    printf("\n separacion segunda\n");
    
    //8176 para punteros para 2044 punteros osea cada puntero usa 4 byte
    tamanio_aux = nbytes;
    double aux_nbytes = (double)(nbytes);
    int punteros_nesesarios_para_datos = ceil(aux_nbytes/8192);
    printf("punteros nesesarios %i para %i bites \n",punteros_nesesarios_para_datos,nbytes);
    int aux_punteros_nesesarios_para_datos = punteros_nesesarios_para_datos;
    int existe_puntero_indirecto = 0;
    
    if(punteros_nesesarios_para_datos<=2044){

        for (int i = 0; i < 2044; i++){
            //pedir bloque al bitmap y que retorne direccion de dicho bloque
            if(i<punteros_nesesarios_para_datos){

                int dir_bloque_con_dato = buscar_blocke_para_indice_de_file(file_desc->particion);
            
                char *array_of_bit = dec_to_bin(dir_bloque_con_dato,4*8);
                printf("\n la direccion del bloque conseguido para los datos %d\n",dir_bloque_con_dato);
                for (int j = (12*8 + 4*8*i); j < (16*8 + 4*8*i); j++){

                    array_of_bite_of_block[j]=array_of_bit[j-(12*8 + 4*8*i)];
                    
                    printf("%x, ",array_of_bite_of_block[j]);
                }
                printf("\n separacion tercera\n");
                printf("veremos si se manteiene el evalor %i\n",dir_bloque_con_dato);
                unsigned char datos_para_bloque[8192];

                //recordar volver a poner aqui
                //recortar por bloques
                for (int w = 0; w < 8192; w++)
                {
                    
                    datos_para_bloque[w]=buffer_en_bite[(8192*i)+w];
                    printf("%c,",datos_para_bloque[w]);
                }
                printf("textificado\n");
                /*
                for (int w = 0; w < 8192; w++)
                {
                    
                    datos_para_bloque[w]=buffer_en_bite[(8192*i)+w];
                    printf("%c,",datos_para_bloque[w]);
                }
                */
                //escribir datos en archivo %c
                FILE *bloque_dato;
                bloque_dato = fopen(diskName,"rb+");
                
                fseek(bloque_dato,dir_bloque_con_dato*BLOCK_SIZE+BLOCK_SIZE*65536*(file_desc->particion), SEEK_SET);
                fwrite (datos_para_bloque,1 , 8192, bloque_dato);
                fclose (bloque_dato);

            }
            else{
                for (int j = (12*8 + 4*8*i); j < (16*8 + 4*8*i); j++){

                    array_of_bite_of_block[j]=0b0;
                    
                    //printf("%x, ",array_of_bite_of_block[j]);
                }
            }          

        }
    free(array_of_bit);
    }
    else if(punteros_nesesarios_para_datos>2044){
        existe_puntero_indirecto = 1;
        punteros_nesesarios_para_datos = punteros_nesesarios_para_datos - 2044;

        for (int i = 12; i < 2044+12; i++)
        {
            printf("entro donde no debia");
        }
        
    }
    //4 puntero indirecto 8188
    // 4+8+2044*4=8188 aqui son 4 bit de direccionamiento indirecto
    for (int i = 8188*8; i < 8192*8; i++)
    {
      array_of_bite_of_block[i]=0b0;
    }
    //unir lista de bit en una lista de byte
    unsigned char buffer2[8192];
    //buffer2[0] = (buffer[0]|(buffer[1]<<2))|(1<<4);
    //buffer2[1] = 0b00010101;

    for (int i = 0; i < 8192*8; i++){
        //printf("%x ,",array_of_bite_of_block[i]);
    }
    printf("empieza aqui la conversion");
    int count_of_char = 0;
    int count = 0;
    buffer2[0] = 0b00000000;
    for (int i = 0; i < 8192*8; i++){
        if(count==8){
            //printf("\ncambio taya---------");
            //printf("%x, ", buffer2[count_of_char]);
            count = 0;
            count_of_char = count_of_char + 1;
            buffer2[count_of_char] = 0b00000000;
            buffer2[count_of_char] = buffer2[count_of_char]|(array_of_bite_of_block[i]<<(7-count));
            
        }
        else{
            buffer2[count_of_char] = buffer2[count_of_char]|(array_of_bite_of_block[i]<<(7-count));
            //printf("\n una mas ee");
        }
        //printf("\n%x, mira bit y %i y count %i y cambio de bite %i y el num %x",array_of_bite_of_block[i],i,count,count_of_char,buffer2[count_of_char]);
        
        count = count + 1;
    }
    
    //escribir lista de byte en bloque
    FILE *bloque_indice;
    bloque_indice = fopen(diskName,"rb+");
    
    fseek(bloque_indice,(file_desc->pos_BloqueIndice)*BLOCK_SIZE+BLOCK_SIZE*65536*(file_desc->particion), SEEK_SET);
	fwrite (buffer2,1 , 8192, bloque_indice);
    fclose (bloque_indice);

 /*


    

    
    */
    return nbytes;
}
