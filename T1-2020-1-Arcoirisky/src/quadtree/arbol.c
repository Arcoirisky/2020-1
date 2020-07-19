#include "arbol.h"
#include "funcionesSimilitud.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// Me base en el siguiente codigo de arbol binario para la creacion O:
// link https://github.com/liobouchan/Arboles/blob/master/main.c
void Insertar(Arbol** arbol, int x0,int z0,int x1,int z1,int nvl, Image* img){
    int h = (x1-x0)/2;
    
    Arbol* temporal = NULL;
    if(!(*arbol)){
        temporal = (Arbol*)malloc(sizeof(Arbol));
        temporal->hijo_Norte = NULL;
        temporal->hijo_Sur = NULL;
        temporal->hijo_Este = NULL;
        temporal->hijo_Oeste = NULL;
        temporal->x0 = x0;
        temporal->x1 = x1;
        temporal->z0 = z0;
        temporal->z1 = z1;
        temporal->nivel = nvl;
        temporal ->media_color = promedio_Color(img,x0,z0,x1,z1);
        temporal -> gamma = calcular_gamma(x0, z0, x1, z1, img);;
        (*arbol) = temporal;
    }
    
    if(h >= 1){
        nvl++;
        Insertar(&(*arbol)->hijo_Norte, x0, z0, x0+h, z0+h, nvl, img);
        Insertar(&(*arbol)->hijo_Sur, x0, z0+h, x0+h, z1, nvl, img);
        Insertar(&(*arbol)->hijo_Este, x0+h, z0+h, x1, z1, nvl, img);
        Insertar(&(*arbol)->hijo_Oeste, x0+h, z0, x1, z0+h, nvl, img);
    }
    else{
        return;
    }
}

void Insertar_filto(Arbol** arbol, int x0,int z0,int x1,int z1,int nvl, Image* img, double alpha){
    int h = (x1-x0)/2;
    
    Arbol* temporal = NULL;
    if(!(*arbol)){
        temporal = (Arbol*)malloc(sizeof(Arbol));
        temporal->hijo_Norte = NULL;
        temporal->hijo_Sur = NULL;
        temporal->hijo_Este = NULL;
        temporal->hijo_Oeste = NULL;
        temporal->x0 = x0;
        temporal->x1 = x1;
        temporal->z0 = z0;
        temporal->z1 = z1;
        temporal->nivel = nvl;
        temporal ->media_color = promedio_Color(img,x0,z0,x1,z1);
        temporal -> gamma = calcular_gamma(x0, z0, x1, z1, img);;
        (*arbol) = temporal;
    }
    
    if(h >= 1){
        nvl++;
        Insertar(&(*arbol)->hijo_Norte, x0, z0, x0+h, z0+h, nvl, img);
        Insertar(&(*arbol)->hijo_Sur, x0, z0+h, x0+h, z1, nvl, img);
        Insertar(&(*arbol)->hijo_Este, x0+h, z0+h, x1, z1, nvl, img);
        Insertar(&(*arbol)->hijo_Oeste, x0+h, z0, x1, z0+h, nvl, img);
    }
    else{
        return;
    }
}


void Alturas(Arbol* arbol){
    if(arbol){
        printf("-----\n");
        printf("ALTURA: %d\n", (arbol->nivel)-1);
        printf("Nivel: %d\n", arbol->nivel);
        printf("size: %i\n",(arbol->x1 - arbol->x0));
        printf("x0: %i,z0: %i, x1: %i, z1: %i\n",arbol->x0,arbol->z0, arbol->x1, arbol->z1);
        Alturas(arbol->hijo_Norte);
        Alturas(arbol->hijo_Sur);
        Alturas(arbol->hijo_Este);
        Alturas(arbol->hijo_Oeste);
    }
}

int altura(Arbol *arbol, int nvl){
    if(!arbol){
        return 0;
    }
    else{
        int alturaN;
        alturaN = altura(arbol->hijo_Norte,nvl);
        return alturaN + 1;
    }
}

// Larga vida a los arboles binarios
// https://stackoverflow.com/questions/9181146/freeing-memory-of-a-binary-tree-c

void destroy(Arbol* arbol){
    //Si no hay arbol, no hacer nada
    if (!(arbol)){
        return;
    }
    //matar!matar!matar!
    destroy(arbol->hijo_Norte);
    destroy(arbol->hijo_Sur);
    destroy(arbol->hijo_Este);
    destroy(arbol->hijo_Oeste);

    free(arbol);
}