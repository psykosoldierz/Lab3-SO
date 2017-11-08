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


int barrier_init(barrier_t *bp, int count)
{
	int i;
	if (count < 1) {
		printf("Error: numero de hebras debe ser mayor que 1\n");
		exit(-1);
	}
	bp->maxcnt = count;
	bp->sbp = &bp->sb[0];
	for (i=0; i < 2; i++) {
		struct _sb *sbp = &(bp->sb[i]);
		sbp->runners = count;
		pthread_mutex_init(&sbp->mutex, NULL);
		pthread_cond_init(&sbp->cond, NULL);
	}
	return(0);
}

int barrier_wait(barrier_t *bp)
{
	struct _sb *sbp = bp->sbp;
	pthread_mutex_lock(&sbp->mutex);
	if (sbp->runners == 1) {
		if (bp->maxcnt != 1) {
		sbp->runners = bp->maxcnt;
		bp->sbp = (bp->sbp == &bp->sb[0])? &bp->sb[1] : &bp->sb[0];
		pthread_cond_broadcast(&sbp->cond);
		}
	} else {
	sbp->runners--;
	while (sbp->runners != bp->maxcnt)
		pthread_cond_wait(&sbp->cond, &sbp->mutex);
	}
	pthread_mutex_unlock(&sbp->mutex);
}

ParamConsola recibirParametrosEntrada(int argc, char **argv){
	ParamConsola param;
	param.fvalue = NULL;
	param.N = 0;
	param.T = 0;
	param.H = 0;
	param.t = 0;
	param.d = 0; //bandera: indica si mostrar resultados por pantalla
	opterr = 0;
	int d;

	while ((d = getopt (argc, argv, "N:T:H:t:f:d")) != -1){
		switch (d){
			case 'f':
				param.fvalue = optarg;
				break;
			case 'N':
				sscanf(optarg, "%d", &param.N);
				break;
			case 'T':
				sscanf(optarg, "%d", &param.T);
				break;
			case 'H':
				sscanf(optarg, "%d", &param.H);
				break;
			case 't':
				sscanf(optarg, "%d", &param.t);
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

int validarArchivoSalida(char* nombreArchivo){
	if (nombreArchivo != NULL && nombreArchivo[0] != '-'){
		return 1;
	}else{
		printf("ERROR ARCHIVO DE SALIDA NO ESPECIFICADO\n");
		return 0;
	}
}

int validarDimensionesMatriz(int n){
	if (n > 1){
		return 1;
	}else{
		printf("Dimensiones mínimas 8x8\n");
		return 0;
	}
}

int validarNumeroHilos(int numeroHilos){
	if (numeroHilos > 0){
		return 1;
	}
	else{
		printf("CANTIDAD DE HILOS NO VÁLIDA, MINIMO 1 HILO\n");
		return 0;
	}
}

int validarNumeroPasos(int numeroPasos){
	if (numeroPasos >= 0){
		return 1;
	}
	else{
		printf("CANTIDAD DE PASOS NO VÁLIDO, MINIMO 0 \n");
		return 0;
	}
}

int validarNumeroIteracion(int numeroIteracion, int numeroPasos){
	if (numeroIteracion >= 0 && numeroIteracion <= numeroPasos){
		return 1;
	}
	else{
		printf("CANTIDAD DE ITERACIONES NO VÁLIDA\n");
		return 1;
	}
}

int validarEntradas(ParamConsola param)
{
		int archivo = validarArchivoSalida(param.fvalue);
		int hilos = validarNumeroHilos(param.H);
		int palabras = validarNumeroPasos(param.T);
		int dimension = validarNumeroIteracion(param.t, param.T);
		int salida = validarDimensionesMatriz(param.N);
		if(archivo == 1 && hilos == 1 && palabras == 1 && dimension == 1 && salida == 1)
		{
			return 1;
		}else
		{
			return 0;
		}
}


Grilla* crearMatriz(int n){
	Grilla *b = (Grilla*)malloc(sizeof(Grilla));

		b->n = n;
		b->matriz = (float***)malloc(n*sizeof(float**));
		if (b != NULL){

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
void copiarCapaMatrizParalelizado(Grilla* grilla, int desde, int hacia, Coordenada coorInicial, int cantidadPosiciones){
	int i=coorInicial.x, j=coorInicial.y, contador=0;
	for (i ; i<grilla->n-1 && contador<cantidadPosiciones; i++){
		for (j ; j<grilla->n -1 && contador<cantidadPosiciones; j++){
			grilla->matriz[i][j][hacia] = grilla->matriz[i][j][desde];
			contador++;
		}
		j=1;
	}
}

void mostrarMatriz(float*** matriz, int n){
	int i,j;

	printf("Instante actual\n");
	for (i=0 ; i < n ; i++){
		for (j = 0 ; j < n ; j++){
			printf("%f|", matriz[i][j][0]);
		}
		printf("\n");
	}
	/*
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
	}*/
}

int calcularPosicionComienzo(int* posicionesPorHilo, int numeroHilo){
	int i;
	int suma=0;
	for (i=0 ; i<numeroHilo - 1 ; i++){
		suma += posicionesPorHilo[i];
	}
	return suma;
}

Coordenada obtenerPosicionInicial(int posicionInicial, int dimension){
	Coordenada coor;
	coor.x = (posicionInicial / (dimension-2))+1;
	coor.y = (posicionInicial % (dimension-2))+1;
	return coor;
}

void distribuirMatriz(int numeroHilos, int dimension, Hebra* dHebra){
	int numeroPosiciones = dimension*dimension - 4* (dimension-1);
	int i;
	int* posicionesPorHilo = (int*)malloc(sizeof(int)*numeroHilos);
	for (i=0 ; i<numeroHilos ; i++){
		posicionesPorHilo[i] = 0;
	}

	int cuociente = numeroPosiciones/numeroHilos;
	int sobrantes = numeroPosiciones - cuociente*numeroHilos;
	i=0;
	while (sobrantes > 0){
		posicionesPorHilo[i]=cuociente+1;
		sobrantes--;
		i++;
	}
	while (i<numeroHilos){
		posicionesPorHilo[i]=cuociente;
		i++;
	}

	for (i=0 ; i < numeroHilos ; i++){
		dHebra[i].posicionInicio = calcularPosicionComienzo(posicionesPorHilo, i+1);
		dHebra[i].cantidadPosiciones = posicionesPorHilo[i];
		dHebras[i].coordenadaInicio = obtenerPosicionInicial(dHebras[i].posicionInicio, dimension);
	}
}

void condicionInicialParalelizado(Grilla* g, Coordenada coorInicio, int cantidadPosiciones){
	int n = g->n;
	int i = coorInicio.x;
	int j = coorInicio.y;
	int contador = 0;
	while (i < n-1 && contador < cantidadPosiciones){
		if (0.4*n < i && i < 0.6*n){
			while (j < n-1 && contador < cantidadPosiciones){
				if (0.4*n < j && j < 0.6*n){
					g->matriz[i][j][0] = 20;
				}
				j++;
				contador++;
			}

			//actualizar contador
		}else{
			contador += n-j-1;
		}

		j=1;
		i++;
	}
}

void condicionTiempoUnoParalelizado(Grilla* g, Coordenada coorInicial, int cantidadPosiciones){
	//float** matriz = copiarMatriz(grilla);
	int i=coorInicial.x, j=coorInicial.y, contador=0;
	float polinomio;
	float c=1.0, dt=0.1, dd=2.0;
	for (i ; i<g->n -1 && contador<cantidadPosiciones; i++){
		for (j ; j<g->n -1  && contador < cantidadPosiciones; j++){
			polinomio =  g->matriz[i+1][j][1]+ g->matriz[i-1][j][1] + g->matriz[i][j-1][1] + g->matriz[i][j+1][1] - 4*g->matriz[i][j][1];
			g->matriz[i][j][0] = g->matriz[i][j][1]+ (pow(c,2)/2) * (pow(dt/dd,2)) * polinomio;
			contador++;
		}
		j=1;
	}
}

void algoritmoSchoedingerParalelizado(Grilla* g, Coordenada coorInicial, int cantidadPosiciones){
	//float** matriz = copiarMatriz(grilla);
	int i=coorInicial.x,j=coorInicial.y, contador=0;
	float polinomio;
	float c=1.0, dt=0.1, dd=2.0;
	for (i ; i<g->n -1 && contador<cantidadPosiciones; i++){
		for (j ; j<g->n -1  && contador < cantidadPosiciones; j++){
			polinomio =  g->matriz[i+1][j][1]+ g->matriz[i-1][j][1] + g->matriz[i][j-1][1] + g->matriz[i][j+1][1] - 4*g->matriz[i][j][1];
			g->matriz[i][j][0] =2* g->matriz[i][j][1] - g->matriz[i][j][2] + (pow(c,2)) * (pow(dt/dd,2)) * polinomio;
			contador++;
		}
		j=1;
	}
}

void imprimirFila(float* fila, int n){
	int i;
	for (i=0 ; i <n ; i++){
		printf("%f|", fila[i]);
	}
	printf("\n");

}

void matrizArchivo(float ***matriz,int n, char *nombreArchivo)
{
	float* fila;
	int i,j;
	FILE* archivo = fopen(nombreArchivo, "w");
	for(i = 0; i < n; i++)
	{
		fila = (float*)malloc(sizeof(float)*n);
		for (j=0 ; j < n; j++){
			fila[j] = matriz[i][j][0];
		}
		fwrite(fila,sizeof(float),n,archivo);
		//imprimirFila(fila, grilla->n);
		free(fila);	
	}
}

void* ecuacionSchoedingerHebra(void* id){
	int* id_h = (int*) id;
	condicionInicialParalelizado(grilla,dHebras[*id_h].coordenadaInicio, dHebras[*id_h].cantidadPosiciones);
	barrier_wait(&barrera);
	if(t==0){
			//HEBRA UNO SE ENCARGA DE COPIAR LA MATRIZ
			if(*id_h == 0){
				mostrarMatriz(grilla->matriz, grilla->n);
				matrizArchivo(grilla->matriz,grilla->n, nombreSalida);
			}
			barrier_wait(&barrera);
	}
	int contador = 1;
	int numBarrera = 1;

	//mostrarMatriz(grilla->matriz, grilla->n);
	
	//t=1
	if (contador <= T){
		printf("t=1\n");
		copiarCapaMatrizParalelizado(grilla,1,2,dHebras[*id_h].coordenadaInicio, dHebras[*id_h].cantidadPosiciones);
		copiarCapaMatrizParalelizado(grilla,0,1,dHebras[*id_h].coordenadaInicio, dHebras[*id_h].cantidadPosiciones);
		barrier_wait(&barrera);
		condicionTiempoUnoParalelizado(grilla,dHebras[*id_h].coordenadaInicio, dHebras[*id_h].cantidadPosiciones);		
		barrier_wait(&barrera);
		if(t==contador)
		{
			//HEBRA UNO SE ENCARGA DE COPIAR LA MATRIZ
			if(*id_h == 0){
				mostrarMatriz(grilla->matriz, grilla->n);
				matrizArchivo(grilla->matriz,grilla->n, nombreSalida);
			}
			barrier_wait(&barrera);
		}

		//mostrarMatriz(grilla->matriz, grilla->n);
		contador++;
		numBarrera+=2;
	}
	while(contador <= T){
		printf("t=%d\n",contador);
		copiarCapaMatrizParalelizado(grilla,1,2,dHebras[*id_h].coordenadaInicio, dHebras[*id_h].cantidadPosiciones);
		copiarCapaMatrizParalelizado(grilla,0,1,dHebras[*id_h].coordenadaInicio, dHebras[*id_h].cantidadPosiciones);
		barrier_wait(&barrera);
		algoritmoSchoedingerParalelizado(grilla,dHebras[*id_h].coordenadaInicio, dHebras[*id_h].cantidadPosiciones);
		barrier_wait(&barrera);
		if(t==contador)
		{
			//HEBRA UNO SE ENCARGA DE COPIAR LA MATRIZ
			if(*id_h == 0){
				mostrarMatriz(grilla->matriz, grilla->n);
				matrizArchivo(grilla->matriz,grilla->n, nombreSalida);
			}
			barrier_wait(&barrera);
		}
		contador++;
		numBarrera+=2;
	}
}

void esperarHilos(int numeroHilos){
	int i=0;
	while(i < numeroHilos)
	{
		pthread_join(dHebras[i].idReal,NULL);
		i++;
	}
}

void inicializarBarreras(int numeroBarreras, int numHilos){
	barrier_init(&barrera, numHilos);
}

void inicializarVariablesGlobales(int numHilos, int dimension, int valorIteraciones, int valorIteracionSalida, char* salida){
	T = valorIteraciones;
	t = valorIteracionSalida;
	nombreSalida = salida;
	dHebras = (Hebra*)malloc(sizeof(Hebra)*numHilos);
	inicializarBarreras(2*t+1, numHilos);
	grilla = crearMatriz(dimension);
}

void lanzarHilos(int numHilos){
	int i, stat;
	pthread_t* thLanzados=(pthread_t*)malloc(sizeof(pthread_t)*numHilos);
	for (i=0 ; i<numHilos ; i++){
		dHebras[i].idSimple = i;
		stat = pthread_create(&thLanzados[i], NULL, ecuacionSchoedingerHebra, &dHebras[i].idSimple);
		//printf("Se lanza hebra %d, stat %d\n",i,stat);
		dHebras[i].idReal = thLanzados[i];
		//printf("state i%d = %d\n",i,stat);
	}
}
