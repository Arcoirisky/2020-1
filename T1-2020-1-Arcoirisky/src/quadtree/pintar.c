#include "pintar.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void contar_hojas(Arbol* arbol, double alpha, int* contador){
    if (!(arbol)){
        return;
    }
    else{
        if(arbol -> gamma < alpha){
            *contador+=1;
            return;
        }
    }
    contar_hojas(arbol->hijo_Norte, alpha, contador);
    contar_hojas(arbol->hijo_Sur, alpha, contador);
    contar_hojas(arbol->hijo_Este, alpha, contador);
    contar_hojas(arbol->hijo_Oeste, alpha, contador);
    
}

void pintar_arbol(Arbol* arbol, double alpha, Image* img){
    if (!(arbol)){
        return;
    }
    else{
        if(arbol -> gamma < alpha){
            // pintar segun color 
            //printf("nvl %i: %d vs %f\n",arbol->nivel, arbol->gamma, alpha);
            int size = arbol->x1 - arbol->x0;
            img_square_paint(img, arbol->z0, arbol->x0, size, arbol->media_color);
            return;
        }
    }
    //matar!matar!matar!
    pintar_arbol(arbol->hijo_Norte, alpha, img);
    pintar_arbol(arbol->hijo_Sur, alpha, img);
    pintar_arbol(arbol->hijo_Este, alpha, img);
    pintar_arbol(arbol->hijo_Oeste, alpha, img);
}