#include <stdio.h>
#include <stdlib.h>
#include "../imagelib/imagelib.h"
#include <stdbool.h>
// https://es.wikibooks.org/wiki/Programaci%C3%B3n_en_C/Algoritmos_y_Estructuras_de_Datos
bool ubicar(int R, int G, int B){
	if (R == 255){
		if(B == 255){
			if(G == 255){
				return true;
			}
		}
	} 
	return false;
}	

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int ordenarBurbuja(int* listaR){
	
	int temp = 0;
	for (int i=1; i< 9 ; i++){
		for (int j=0; j< 9 - i; j++){
			if (listaR[j] > listaR[j+1]){
				temp = listaR[j];
				listaR[j] = listaR[j+1];
				listaR[j+1] = temp;
			}
		}
	}
	
	//qsort(listaR, 9, sizeof(int), cmpfunc);
	return listaR[4];
}

int centroFiltroG(Image *img, int row, int col){
	int listaR[9] = {
					img -> pixels[row-1][col-1].G,
					img -> pixels[row][col-1].G,
					img -> pixels[row+1][col-1].G,
					img -> pixels[row-1][col].G,
					img -> pixels[row][col].G,
					img -> pixels[row+1][col].G,
					img -> pixels[row-1][col+1].G,
					img -> pixels[row][col+1].G,
					img -> pixels[row+1][col+1].G,
					};
	
	return ordenarBurbuja(listaR);
}
int centroFiltroB(Image *img, int row, int col){
	int listaR[9] = {
					img -> pixels[row-1][col-1].B,
					img -> pixels[row][col-1].B,
					img -> pixels[row+1][col-1].B,
					img -> pixels[row-1][col].B,
					img -> pixels[row][col].B,
					img -> pixels[row+1][col].B,
					img -> pixels[row-1][col+1].B,
					img -> pixels[row][col+1].B,
					img -> pixels[row+1][col+1].B,
					};
	return ordenarBurbuja(listaR);
}
int centroFiltroR(Image *img, int row, int col){
	int listaR[9] = {
					img -> pixels[row-1][col-1].R,
					img -> pixels[row][col-1].R,
					img -> pixels[row+1][col-1].R,
					img -> pixels[row-1][col].R,
					img -> pixels[row][col].R,
					img -> pixels[row+1][col].R,
					img -> pixels[row-1][col+1].R,
					img -> pixels[row][col+1].R,
					img -> pixels[row+1][col+1].R,
					};
	return ordenarBurbuja(listaR);
}

int bordeFiltroR0(Image *img, int row){
	int listaR[9] = {
				img -> pixels[row-1][0].R,
				img -> pixels[row][0].R,
				img -> pixels[row+1][0].R,
				img -> pixels[row-1][0].R,
				img -> pixels[row][0].R,
				img -> pixels[row+1][0].R,
				img -> pixels[row-1][1].R,
				img -> pixels[row][1].R,
				img -> pixels[row+1][1].R,
				};
	return ordenarBurbuja(listaR);
}
int bordeFiltroG0(Image *img, int row){
	int listaR[9] = {
				img -> pixels[row-1][0].G,
				img -> pixels[row][0].G,
				img -> pixels[row+1][0].G,
				img -> pixels[row-1][0].G,
				img -> pixels[row][0].G,
				img -> pixels[row+1][0].G,
				img -> pixels[row-1][1].G,
				img -> pixels[row][1].G,
				img -> pixels[row+1][1].G,
				};
	return ordenarBurbuja(listaR);
}
int bordeFiltroB0(Image *img, int row){
	int listaR[9] = {
				img -> pixels[row-1][0].B,
				img -> pixels[row][0].B,
				img -> pixels[row+1][0].B,
				img -> pixels[row-1][0].B,
				img -> pixels[row][0].B,
				img -> pixels[row+1][0].B,
				img -> pixels[row-1][1].B,
				img -> pixels[row][1].B,
				img -> pixels[row+1][1].B,
				};
	return ordenarBurbuja(listaR);
}

int bordeFiltroR1(Image *img, int row){
	int col = img -> width - 1;
	int listaR[9] = {
				img -> pixels[row-1][col].R,
				img -> pixels[row][col].R,
				img -> pixels[row+1][col].R,
				img -> pixels[row-1][col].R,
				img -> pixels[row][col].R,
				img -> pixels[row+1][col].R,
				img -> pixels[row-1][col-1].R,
				img -> pixels[row][col-1].R,
				img -> pixels[row+1][col-1].R,
				};
	return ordenarBurbuja(listaR);
}
int bordeFiltroG1(Image *img, int row){
	int col = img -> width - 1;
	int listaR[9] = {
				img -> pixels[row-1][col].G,
				img -> pixels[row][col].G,
				img -> pixels[row+1][col].G,
				img -> pixels[row-1][col].G,
				img -> pixels[row][col].G,
				img -> pixels[row+1][col].G,
				img -> pixels[row-1][col-1].G,
				img -> pixels[row][col-1].G,
				img -> pixels[row+1][col-1].G,
				};
	return ordenarBurbuja(listaR);
}
int bordeFiltroB1(Image *img, int row){
	int col = img -> width - 1;
	int listaR[9] = {
				img -> pixels[row-1][col].B,
				img -> pixels[row][col].B,
				img -> pixels[row+1][col].B,
				img -> pixels[row-1][col].B,
				img -> pixels[row][col].B,
				img -> pixels[row+1][col].B,
				img -> pixels[row-1][col-1].B,
				img -> pixels[row][col-1].B,
				img -> pixels[row+1][col-1].B,
				};
	return ordenarBurbuja(listaR);
}

int superFiltroR0(Image *img, int col){
	int listaR[9] = {
				img -> pixels[0][col-1].R,
				img -> pixels[0][col].R,
				img -> pixels[0][col+1].R,
				img -> pixels[0][col-1].R,
				img -> pixels[0][col].R,
				img -> pixels[0][col+1].R,
				img -> pixels[1][col-1].R,
				img -> pixels[1][col].R,
				img -> pixels[1][col+1].R,
				};
	return ordenarBurbuja(listaR);
}
int superFiltroB0(Image *img, int col){
	int listaR[9] = {
				img -> pixels[0][col-1].B,
				img -> pixels[0][col].B,
				img -> pixels[0][col+1].B,
				img -> pixels[0][col-1].B,
				img -> pixels[0][col].B,
				img -> pixels[0][col+1].B,
				img -> pixels[1][col-1].B,
				img -> pixels[1][col].B,
				img -> pixels[1][col+1].B,
				};
	return ordenarBurbuja(listaR);
}
int superFiltroG0(Image *img, int col){
	int listaR[9] = {
				img -> pixels[0][col-1].G,
				img -> pixels[0][col].G,
				img -> pixels[0][col+1].G,
				img -> pixels[0][col-1].G,
				img -> pixels[0][col].G,
				img -> pixels[0][col+1].G,
				img -> pixels[1][col-1].G,
				img -> pixels[1][col].G,
				img -> pixels[1][col+1].G,
				};
	return ordenarBurbuja(listaR);
}

int superFiltroR1(Image *img, int col){
	int row = img -> height -1;

	int listaR[9] = {
				img -> pixels[row][col-1].R,
				img -> pixels[row][col].R,
				img -> pixels[row][col+1].R,
				img -> pixels[row][col-1].R,
				img -> pixels[row][col].R,
				img -> pixels[row][col+1].R,
				img -> pixels[row-1][col-1].R,
				img -> pixels[row-1][col].R,
				img -> pixels[row-1][col+1].R,
				};
	return ordenarBurbuja(listaR);
}
int superFiltroG1(Image *img, int col){
	int row = img -> height -1;

	int listaR[9] = {
				img -> pixels[row][col-1].G,
				img -> pixels[row][col].G,
				img -> pixels[row][col+1].G,
				img -> pixels[row][col-1].G,
				img -> pixels[row][col].G,
				img -> pixels[row][col+1].G,
				img -> pixels[row-1][col-1].G,
				img -> pixels[row-1][col].G,
				img -> pixels[row-1][col+1].G,
				};
	return ordenarBurbuja(listaR);
}
int superFiltroB1(Image *img, int col){
	int row = img -> height -1;

	int listaR[9] = {
				img -> pixels[row][col-1].B,
				img -> pixels[row][col].B,
				img -> pixels[row][col+1].B,
				img -> pixels[row][col-1].B,
				img -> pixels[row][col].B,
				img -> pixels[row][col+1].B,
				img -> pixels[row-1][col-1].B,
				img -> pixels[row-1][col].B,
				img -> pixels[row-1][col+1].B,
				};
	return ordenarBurbuja(listaR);
}

int esquinaSupDR(Image *img){
	int listaR[9] = {
				img -> pixels[0][0].R,
				img -> pixels[0][0].R,
				img -> pixels[0][0].R,
				img -> pixels[0][0].R,
				img -> pixels[0][1].R,
				img -> pixels[0][1].R,
				img -> pixels[1][0].R,
				img -> pixels[1][0].R,
				img -> pixels[1][1].R,
				};
	return ordenarBurbuja(listaR);
}
int esquinaSupDG(Image *img){
	int listaR[9] = {
				img -> pixels[0][0].G,
				img -> pixels[0][0].G,
				img -> pixels[0][0].G,
				img -> pixels[0][0].G,
				img -> pixels[0][1].G,
				img -> pixels[0][1].G,
				img -> pixels[1][0].G,
				img -> pixels[1][0].G,
				img -> pixels[1][1].G,
				};
	return ordenarBurbuja(listaR);
}
int esquinaSupDB(Image *img){
	int listaR[9] = {
				img -> pixels[0][0].B,
				img -> pixels[0][0].B,
				img -> pixels[0][0].B,
				img -> pixels[0][0].B,
				img -> pixels[0][1].B,
				img -> pixels[0][1].B,
				img -> pixels[1][0].B,
				img -> pixels[1][0].B,
				img -> pixels[1][1].B,
				};
	return ordenarBurbuja(listaR);
}

int esquinaSupIR(Image *img){
	int r = 0;
	int c = img -> width - 1;

	int listaR[9] = {
				img -> pixels[r][c].R,
				img -> pixels[r][c].R,
				img -> pixels[r][c].R,
				img -> pixels[r][c].R,
				img -> pixels[r][c-1].R,
				img -> pixels[r][c-1].R,
				img -> pixels[r+1][c].R,
				img -> pixels[r+1][c].R,
				img -> pixels[r+1][c-1].R,
				};
	return ordenarBurbuja(listaR);
}
int esquinaSupIG(Image *img){
	int r = 0;
	int c = img -> width - 1;

	int listaR[9] = {
				img -> pixels[r][c].G,
				img -> pixels[r][c].G,
				img -> pixels[r][c].G,
				img -> pixels[r][c].G,
				img -> pixels[r][c-1].G,
				img -> pixels[r][c-1].G,
				img -> pixels[r+1][c].G,
				img -> pixels[r+1][c].G,
				img -> pixels[r+1][c-1].G,
				};
	return ordenarBurbuja(listaR);
}
int esquinaSupIB(Image *img){
	int c = img -> width - 1;

	int listaR[9] = {
				img -> pixels[0][c].B,
				img -> pixels[0][c].B,
				img -> pixels[0][c].B,
				img -> pixels[0][c].B,
				img -> pixels[0][c-1].B,
				img -> pixels[0][c-1].B,
				img -> pixels[1][c].B,
				img -> pixels[1][c].B,
				img -> pixels[1][c-1].B,
				};
	return ordenarBurbuja(listaR);
}

int esquinaInfIR(Image *img){
	int r = img -> height - 1;
	int c = img -> width - 1;

	int listaR[9] = {
				img -> pixels[r][c].R,
				img -> pixels[r][c].R,
				img -> pixels[r][c].R,
				img -> pixels[r][c].R,
				img -> pixels[r][c-1].R,
				img -> pixels[r][c-1].R,
				img -> pixels[r-1][c].R,
				img -> pixels[r-1][c].R,
				img -> pixels[r-1][c-1].R,
				};
	return ordenarBurbuja(listaR);
}
int esquinaInfIG(Image *img){
	int r = img -> height - 1;
	int c = img -> width - 1;

	int listaR[9] = {
				img -> pixels[r][c].G,
				img -> pixels[r][c].G,
				img -> pixels[r][c].G,
				img -> pixels[r][c].G,
				img -> pixels[r][c-1].G,
				img -> pixels[r][c-1].G,
				img -> pixels[r-1][c].G,
				img -> pixels[r-1][c].G,
				img -> pixels[r-1][c-1].G,
				};
	return ordenarBurbuja(listaR);
}
int esquinaInfIB(Image *img){
	int r = img -> height - 1;
	int c = img -> width - 1;

	int listaR[9] = {
				img -> pixels[r][c].B,
				img -> pixels[r][c].B,
				img -> pixels[r][c].B,
				img -> pixels[r][c].B,
				img -> pixels[r][c-1].B,
				img -> pixels[r][c-1].B,
				img -> pixels[r-1][c].B,
				img -> pixels[r-1][c].B,
				img -> pixels[r-1][c-1].B,
				};
	return ordenarBurbuja(listaR);
}

int esquinaInfDR(Image *img){
	int r = img -> height - 1;
	int listaR[9] = {
				img -> pixels[r][0].R,
				img -> pixels[r][0].R,
				img -> pixels[r][0].R,
				img -> pixels[r][0].R,
				img -> pixels[r][1].R,
				img -> pixels[r][1].R,
				img -> pixels[r-1][0].R,
				img -> pixels[r-1][0].R,
				img -> pixels[r-1][1].R,
				};
	return ordenarBurbuja(listaR);
}
int esquinaInfDG(Image *img){
	int r = img -> height - 1;

	int listaR[9] = {
				img -> pixels[r][0].G,
				img -> pixels[r][0].G,
				img -> pixels[r][0].G,
				img -> pixels[r][0].G,
				img -> pixels[r][1].G,
				img -> pixels[r][1].G,
				img -> pixels[r-1][0].G,
				img -> pixels[r-1][0].G,
				img -> pixels[r-1][1].G,
				};
	return ordenarBurbuja(listaR);
}
int esquinaInfDB(Image *img){
	int r = img -> height - 1;

	int listaR[9] = {
				img -> pixels[r][0].B,
				img -> pixels[r][0].B,
				img -> pixels[r][0].B,
				img -> pixels[r][0].B,
				img -> pixels[r][1].B,
				img -> pixels[r][1].B,
				img -> pixels[r-1][0].B,
				img -> pixels[r-1][0].B,
				img -> pixels[r-1][1].B,
				};
	return ordenarBurbuja(listaR);
}

int main(int argc, char *argv[])
{
	/* El programa recibe 2 parametros */
	if(argc != 3)
	{
		printf("Modo de uso: %s <input.png> <output.png>\n", argv[0]);
		printf("\t<input.png> es la imagen a filtrar\n");
		printf("\t<output.png> es donde se guardará la imagen resultante\n");
		return 1;
	}

	/* Leemos la imagen a partir del archivo */
	char* input_file = argv[1];
	Image* img = img_png_read_from_file(input_file);
	Image* Out = img_png_read_from_file(input_file);
	
	/* TODO: Filtro de mediana */
    // Centros supongo
	for (int row = 1; row < img -> height - 1; row++)
	{
		for (int col = 1; col < img -> width - 1; col++)
		{
			if (ubicar(img -> pixels[row][col].R,img -> pixels[row][col].G,img -> pixels[row][col].B)){
				Out -> pixels[row][col].R = centroFiltroR(img, row, col);
				Out -> pixels[row][col].G = centroFiltroG(img, row, col);
				Out -> pixels[row][col].B = centroFiltroB(img, row, col);
			}
		}
	}
	
	//Bordes laterales ahora supongo
	for (int row = 1; row < img -> height-1; row++){
		if (ubicar(img -> pixels[row][0].R,img -> pixels[row][0].G,img -> pixels[row][0].B)){
			Out -> pixels[row][0].R = bordeFiltroR0(img, row);
			Out -> pixels[row][0].G = bordeFiltroG0(img, row);
			Out -> pixels[row][0].B = bordeFiltroB0(img, row);
		}
	}
	
	for (int row = 1; row < img -> height-1; row++){
		if (ubicar(img -> pixels[row][img -> width-1].R,img -> pixels[row][img -> width-1].G,img -> pixels[row][img -> width-1].B)){
			Out -> pixels[row][img -> width-1].R = bordeFiltroR1(img, row);
			Out -> pixels[row][img -> width-1].G = bordeFiltroG1(img, row);
			Out -> pixels[row][img -> width-1].B = bordeFiltroB1(img, row);
		}
	}
	
	// Bordes superior e inferior ahora -suspiro-
	for (int col = 1; col < img -> width-1; col++){
		if (ubicar(img -> pixels[0][col].R,img -> pixels[0][col].G,img -> pixels[0][col].B)){
			Out -> pixels[0][col].R = superFiltroR0(img, col);
			Out -> pixels[0][col].B = superFiltroB0(img, col);
			Out -> pixels[0][col].G = superFiltroG0(img, col);
		}
	}
	
	for (int col = 1; col < img -> width-1; col++){
		if (ubicar(img -> pixels[img -> height-1][col].R,img -> pixels[img -> height-1][col].G,img -> pixels[img -> height-1][col].B)){
			Out -> pixels[img -> height-1][col].R = superFiltroR1(img, col);
			Out -> pixels[img -> height-1][col].G = superFiltroG1(img, col);
			Out -> pixels[img -> height-1][col].B = superFiltroB1(img, col);
		}
	}
	
	// Esquinaaas
	//SupD
	if (ubicar(img -> pixels[0][0].R,img -> pixels[0][0].G,img -> pixels[0][0].B)){
			Out -> pixels[0][0].R = esquinaSupDR(img);
			Out -> pixels[0][0].G = esquinaSupDG(img);
			Out -> pixels[0][0].B = esquinaSupDB(img);
		}
	//SupI
	if (ubicar(img -> pixels[0][img -> width-1].R,img -> pixels[0][img -> width-1].G,img -> pixels[0][img -> width-1].B)){
			Out -> pixels[0][img -> width-1].R = esquinaSupIR(img);
			Out -> pixels[0][img -> width-1].G = esquinaSupIG(img);
			Out -> pixels[0][img -> width-1].B = esquinaSupIB(img);
		}
	//InfD
	if (ubicar(img -> pixels[img -> height-1][0].R,img -> pixels[img -> height-1][0].G,img -> pixels[img -> height-1][0].B)){
			Out -> pixels[img -> height-1][0].R = esquinaInfDR(img);
			Out -> pixels[img -> height-1][0].G = esquinaInfDG(img);
			Out -> pixels[img -> height-1][0].B = esquinaInfDB(img);
		}
	//InfI
	if (ubicar(img -> pixels[img -> height-1][img -> width-1].R,img -> pixels[img -> height-1][img -> width-1].G,img -> pixels[img -> height-1][img -> width-1].B)){
			Out -> pixels[img -> height-1][img -> width-1].R = esquinaInfIR(img);
			Out -> pixels[img -> height-1][img -> width-1].G = esquinaInfIG(img);
			Out -> pixels[img -> height-1][img -> width-1].B = esquinaInfIB(img);
		}
	/* Guardamos la imagen como un archivo png */
	
	char* output_file = argv[2];
	img_png_write_to_file (Out, output_file);

	/* Liberamos los recursos asociados de la imagen */
	img_png_destroy(img);
	// Tambien debemos liberar la copia que hago
    img_png_destroy(Out);
	return 0;
}
