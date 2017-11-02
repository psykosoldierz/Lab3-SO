#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "structs.h"
#include "wave.h"

ParamConsola recibirParametrosEntrada(int argc, char **argv){
	ParamConsola param;
	param.ivalue = NULL;
	param.svalue = NULL;
	param.n = 0;
	param.h = 0;
	param.m = 0;
	param.c = 0;
	param.d = 0; //bandera: indica si mostrar resultados por pantalla
	opterr = 0;
	int d;

	while ((d = getopt (argc, argv, "i:n:c:p:d")) != -1){
		switch (d){
			case 'i':
				param.ivalue = optarg;
				break;
			case 'h':
				sscanf(optarg, "%d", &param.h);
				break;
			case 'n':
				sscanf(optarg, "%d", &param.n);
				break;
			case 'c':
				sscanf(optarg, "%d", &param.c);
				break;
			case 'm':
				sscanf(optarg, "%d", &param.m);
				break;
			case 's':
				param.svalue = optarg;
				break;
			case 'd':
				param.d = 1;
				break;
			case '?':
				if (optopt == 'i')
					fprintf(stderr, "Opcion -%c requiere un argumento\n", optopt);
				else if (optopt == 's')
					fprintf(stderr, "Opcion -%c requiere un argumento\n", optopt);
				else if (optopt == 'n')
					fprintf(stderr, "Opcion -%c requiere un argumento\n", optopt);
				else if (optopt == 'm')
					fprintf(stderr, "Opcion -%c requiere un argumento\n", optopt);
				else if (optopt == 'c')
					fprintf(stderr, "Opcion -%c requiere un argumento\n", optopt);
				else if (optopt == 'p')
					fprintf(stderr, "Opcion -%c requiere un argumento\n", optopt);
				else if (isprint(optopt))
					fprintf(stderr, "Opcion desconocida '-%c' \n", optopt);
				else
					fprintf(stderr,
							"Opcion con caracter desconocido '\\x%x' \n",
							optopt);
				param.statusCode=-1;
				printf("Error con parametros de entrada\n");
				return param;
			default:
				printf("Error con parametros de entrada\n");
				abort ();
		}
	}
	param.statusCode=0;
	return param;
}
void distribuirGrilla(int n, int cHebras)
{
	int base,i,k,celdas = n * n;
	int resto = celdas % cHebras;
	int celdasPorHebra = celdas/cHebras;
	int cCeldas[cHebras];
	for (k = 0; k < cHebras; k++)
	{
		// SI SOBRAN CELDAS DE LE SUMARA 1 CELDA A CADA HEBRA
		if(k < resto)
		{
			cCeldas[k] = celdasPorHebra + 1;
		}else
		{
			cCeldas[k] = celdasPorHebra;
		}
	}
	for(i = 0 ; i < cHebras; i++)
	{
		if(i == 0)
		{
			base = 0;
		}
		else
		{
			base = base + cCeldas[i-1];
		}
		int posiciones[celdasPorHebra];

		for(j = 0 ; j < cCeldas[i] ; j++)
		{
			posiciones[j] = base + j;
		}
		//asignar el arreglo de celdas a la hebra que seria "posiciones"
	}
}

void inicializarMutexs(Grilla* sp){
	sp->mutexsPos = (pthread_mutex_t***)malloc(sizeof(pthread_mutex_t**)*sp->n);
	int i,j;
	for (i=0 ; i < sp->n ; i++){
		sp->mutexsPos[i] = (pthread_mutex_t**)malloc(sizeof(pthread_mutex_t*)*sp->m);
		for (j=0 ; j < sp->n ; j++){
			sp->mutexsPos[i][j] = (pthread_mutex_t*)malloc(3*sizeof(pthread_mutex_t));
			pthread_mutex_init(&sp->mutexsPos[i][j][0],NULL);
			pthread_mutex_init(&sp->mutexsPos[i][j][1],NULL);
			pthread_mutex_init(&sp->mutexsPos[i][j][2],NULL);
		}
	}
}

int* desparametrizar(int numero, int columnas)
{
	int par[2];
	int aux = numero;
	int x = 0, y;
	while (aux > columnas)
	{
		aux = aux - columnas;
		x++;
	}
	y = aux - 1;
	par[0] = x;
	par[1] = y;
	return par;
}


Grilla* crearMatriz(int n){
	Grilla *b = (Grilla*)malloc(sizeof(Grilla));

	if (b != NULL){
		b->n = n;
		b->matriz = (float***)malloc(n*sizeof(float**));

		if (b->matriz!=NULL)
		{
			int i,j,k;
			for (i=0;i<n;i++){
				b->matriz[i] = (float**)malloc(n*sizeof(float*));
				if (b->matriz[i] != NULL){
					for(j=0 ; j<n ; j++){
						b->matriz[i][j] = (float*)malloc(3*sizeof(float));
						if (b->matriz[i][j] != NULL){
							for (k=0 ; k < 3; k++){
								b->matriz[i][j][k] = 0;
							}
						}else{
							printf("Error al asignar memoria capas\n");
						}
					}
				}else
					printf("Error al asignar memoria columna\n");
			}

		}else
			printf("Error al asignar memoria fila\n");

	}else
		printf("Error al asignar memoria\n");
	return b;
}
/*
float** copiarMatriz(Grilla* grilla){
	float** matriz = (float**)malloc(sizeof(float*)*grilla->n);
	int i,j;
	if (matriz != NULL){
		for (int i=0 ; i < grilla->n ; i++){
			matriz[i] = (float*)malloc(grilla->n*sizeof(float));
			if (matriz[i] != NULL){
				for (j=0 ; j < grilla->n ; j++){
					matriz[i][j] = grilla->matriz[i][j];
				}
			}else{
				printf("Error al asignar memoria\n");
			}
		}
	}else{
		printf("Error al asignar memoria\n");
	}
	return matriz;
}*/

void copiarCapaMatriz(Grilla* grilla, int desde, int hacia){
	int i,j;
	for (i=0 ; i<grilla->n ; i++){
		for (j=0 ; j<grilla->n ; j++){
			grilla->matriz[i][j][hacia] = grilla->matriz[i][j][desde];
		}
	}
}
/*
void liberarMatriz(float*** matriz, int n){
	int i,j;
	for (i=0 ; i<n ; i++){
		for (j=0 ; j<n ; j++){
			free(matriz[i][j]);
		}
	}
	free(matriz);
}*/
void mostrarMatriz(float*** matriz, int n){
	int i,j;

	printf("Instante actual\n");
	for (i=0 ; i < n ; i++){
		for (j = 0 ; j < n ; j++){
			printf("%f|", matriz[i][j][0]);
		}
		printf("\n");
	}

	printf("Instante t-1\n");
	for (i=0 ; i < n ; i++){
		for (j = 0 ; j < n ; j++){
			printf("%f|",matriz[i][j][1]);
		}
		printf("\n");
	}

	printf("Instante t-2\n");
	for (i=0 ; i < n ; i++){
		for (j = 0 ; j < n ; j++){
			printf("%f|",matriz[i][j][2]);
		}
		printf("\n");
	}
}

void ponerUnos(Grilla* grilla){
	int i,j;
	int polinomio;
	float c=1.0, dt=0.1, dd=2.0;
	for (i=1 ; i<grilla->n -1 ; i++){
		for (j=1 ; j<grilla->n -1 ; j++){
			//polinomio =  matriz[i+1][j]+ matriz[i-1][j] + matriz[i][j-1] + matriz[i][j+1] - 4*matriz[i][j];
			grilla->matriz[i][j][0] = 1;
		}
	}
}

void condicionInicial(Grilla* grilla){
	int n = grilla->n;
	int i = 0.4 * n ;
	int j;
	for (i ; i < 0.6 * n - 1 ; i++){
		for (j = 0.4*n; j < 0.6*n -1; j++){
			grilla->matriz[i][j][0] = 20;
		}
	}
}

void condicionTiempoUno(Grilla* g){
	//float** matriz = copiarMatriz(grilla);
	int i,j;
	float polinomio;
	float c=1.0, dt=0.1, dd=2.0;
	for (i=1 ; i<g->n -1 ; i++){
		for (j=1 ; j<g->n -1 ; j++){
			polinomio =  g->matriz[i+1][j][1]+ g->matriz[i-1][j][1] + g->matriz[i][j-1][1] + g->matriz[i][j+1][1] - 4*g->matriz[i][j][1];
			g->matriz[i][j][0] = g->matriz[i][j][1]+ (pow(c,2)/2) * (pow(dt/dd,2)) * polinomio;
		}
	}
}

void algoritmoSchoedinger(Grilla* g){
	//float** matriz = copiarMatriz(grilla);
	int i,j;
	float polinomio;
	float c=1.0, dt=0.1, dd=2.0;
	for (i=1 ; i<g->n -1 ; i++){
		for (j=1 ; j<g->n -1 ; j++){
			polinomio =  g->matriz[i+1][j][1]+ g->matriz[i-1][j][1] + g->matriz[i][j-1][1] + g->matriz[i][j+1][1] - 4*g->matriz[i][j][1];
			g->matriz[i][j][0] =2* g->matriz[i][j][1] - g->matriz[i][j][2] + (pow(c,2)) * (pow(dt/dd,2)) * polinomio;
		}
	}
}

void ecuacionSchoedinger(Grilla* grilla, int t){
	//t = 0
	printf("t=0\n");
	condicionInicial(grilla);
	int contador = 1;
	mostrarMatriz(grilla->matriz, grilla->n);
	//t=1
	if (contador <= t){
		printf("t=1\n");
		copiarCapaMatriz(grilla,1,2);
		copiarCapaMatriz(grilla,0,1);
		condicionTiempoUno(grilla);
		mostrarMatriz(grilla->matriz, grilla->n);
		contador++;
	}
	while(contador <= t){
		printf("t=%d\n",contador);
		copiarCapaMatriz(grilla,1,2);
		copiarCapaMatriz(grilla,0,1);
		algoritmoSchoedinger(grilla);
		mostrarMatriz(grilla->matriz, grilla->n);
		contador++;
	}

}


void main(){
	Grilla* grilla = crearMatriz(14);
	//condicionInicial(grilla);
	ecuacionSchoedinger(grilla, 20000);
	//condicionTiempoUno(grilla);
	//mostrarMatriz(grilla->matriz, grilla->n,0);
	//printf("\n");
	//mostrarMatriz(grilla->matriz, grilla->n,1);
	/*int i;
	for (i=0 ; i<grilla->n ; i++){
		free(matriz[i]);
	}
	free(matriz);*/

	//float c=1.0, dt=0.1, dd=2.0;
	//printf("%f\n",(pow(c,2)/2) * (pow(dt/dd,2)));
}
