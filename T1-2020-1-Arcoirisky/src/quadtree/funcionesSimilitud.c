#include "funcionesSimilitud.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void promedio_cuadrante(Image* img, int x0, int z0,int x1, int z1, double* promL, double* promA, double* promB){
    int size = (x1 - x0);
    double sumL = 0;
    double sumA = 0;
    double sumB = 0;
    double n = pow(size,2);
    for(int curr_row = z0; curr_row < z0 + size; curr_row++){
        for(int curr_col = x0; curr_col < x0 + size; curr_col++){
            sumL += img -> pixels[curr_row][curr_col].L;
            sumA += img -> pixels[curr_row][curr_col].a;
            sumB += img -> pixels[curr_row][curr_col].b;
        }
    }
    *promL = sumL / n;
    *promA = sumA / n;
    *promB = sumB / n;
}

Color promedio_Color(Image* img, int x0, int z0,int x1, int z1){
    int size = (x1 - x0);
    double sumL = 0;
    double sumA = 0;
    double sumB = 0;
    double n = pow(size,2);
    for(int curr_row = z0; curr_row < z0 + size; curr_row++){
        for(int curr_col = x0; curr_col < x0 + size; curr_col++){
            sumL += img -> pixels[curr_row][curr_col].L;
            sumA += img -> pixels[curr_row][curr_col].a;
            sumB += img -> pixels[curr_row][curr_col].b;
        }
    }
    Color aux = {sumL/n, sumA/n, sumB/n};
    return aux;
}

void desviacion_estandar(Image* img, int x0, int z0,int x1, int z1, double promL, double promA, double promB, double* desL, double* desA, double* desB){
    int size = (x1 - x0);
    double sumL = 0;
    double sumA = 0;
    double sumB = 0;
    double n = pow(size,2);
    for(int curr_row = z0; curr_row < z0 + size; curr_row++){
        for(int curr_col = x0; curr_col < x0 + size; curr_col++){
            sumL += pow((img -> pixels[curr_row][curr_col].L - promL),2);
            sumA += pow((img -> pixels[curr_row][curr_col].a - promA),2);
            sumB += pow((img -> pixels[curr_row][curr_col].b - promB),2);
        }
    }
    *desL = sqrt(sumL/n);
    *desA = sqrt(sumA/n);
    *desB = sqrt(sumB/n);
}

double calcular_gamma(int x0,int z0,int x1,int z1,Image* img){
    double promL = 0;
    double promA = 0;
    double promB = 0;
    promedio_cuadrante(img, x0, z0, x1, z1, &promL, &promA, &promB);

    // ahora que tenemos los promedios debemos calcular la desviacion estandar
    double desL = 0;
    double desA = 0;
    double desB = 0;
    desviacion_estandar(img, x0, z0, x1, z1, promL, promA, promB, &desL, &desA, &desB);
    
    double gamma = (desL + desA + desB)/3;
    //printf("gamma = %f\n",gamma);
    return gamma;
}