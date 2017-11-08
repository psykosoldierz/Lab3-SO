#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#include "structs.h"
#include "wave_funct.h"

Hebra* dHebras;
Grilla* grilla;
barrier_t barrera;
int t; //uteracion de salida
int T; // Numero de pasos
char* nombreSalida;//archivo de salida

void main(int argc, char **argv){

	//clock_t start = clock();
	ParamConsola param = recibirParametrosEntrada(argc, argv);
	printf("salida: %s\n",param.fvalue);
	printf("t: %d\n",param.t);
	//VERIFICAR SI ENTRADAS SON VÁLIDAS
	if(validarEntradas(param)){

		printf("Parametros validos\n");
		inicializarVariablesGlobales(param.H, param.N, param.T,param.t, param.fvalue);
		distribuirMatriz(param.H, param.N, dHebras);
		lanzarHilos(param.H);
		esperarHilos(param.H);
		//mostrarMatriz(grilla->matriz, grilla->n);
		//printf("Tiempo transcurrido: %f", ((double)clock() - start) / CLOCKS_PER_SEC);
	}else{
		printf("Parametros invalidos, programa abortado\n");

	}

}
