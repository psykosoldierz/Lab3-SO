#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include <pthread.h>

typedef struct ParamConsola{
	char *ivalue; //nombre archivo de entrada
	int h; //numero de hebras
	int c; //cantidad de palabras
	int n; //ancho de matriz
	int m; //largo de matriz
	char *svalue; //nombre archivo de salida
	int d,statusCode; // d-> bandera si indica mostrar resultado por pantalla
	//statusCode : indica si parametros fueron ingresados correctamente
}ParamConsola;

typedef struct Grilla{
	float ***matriz;
	pthread_mutex_t*** mutexsPos;
	int n;
	float c,dd,dt;
}Grilla;

typedef struct Coordenada{
	int x, y;
}Coordenada;

typedef struct Hebra{
	int idSimple;
	pthread_t idReal;
	int palabraInicio;
	int cantidadPalabras;
	int *posiciones;
}Hebra;

//****VARIABLES GLOBALES****/

extern Hebra* dHebras;
extern char** listaPalabras;
extern Grilla* grilla;

/***********************/
#endif
