#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#include <pthread.h>

struct _sb {
pthread_cond_t cond;
pthread_mutex_t mutex;
int runners; // numero de hebras que aun no llegan a la barrera
};

typedef struct {
int maxcnt; // numero de hebras participando en la barrera
struct _sb sb[2];
struct _sb *sbp;
} barrier_t;

typedef struct ParamConsola{
	int N; //ancho de matriz
	int T; // pasos
	int H; //numero de hebras
	int t; //iteracion de salida
	char *fvalue; //archivo de salida
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
	int posicionInicio;
	int cantidadPosiciones;
	Coordenada coordenadaInicio;
}Hebra;

//****VARIABLES GLOBALES****/

extern Hebra* dHebras;
extern Grilla* grilla;
extern barrier_t* barreras;
extern int t, T; //tiempo

/***********************/
#endif
