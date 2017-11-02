#ifndef _FUNCT_MAIN_H
#define _FUNCT_MAIN_H

/*Muestra la matriz durante la ubicacion de palabras
Salida: void
*/
void* mostrarMatrizH();

/*Funcion: recibe los parametros por consola
Entrada: struct ParamConsola
Salida:struct ParamConsola con valores guardados
*/
ParamConsola recibirParametrosEntrada(int argc, char **argv);

/*Procedimiento: Distribuye las casillas de la grilla para cada hebras
Entrada: dimension de la grilla y la cantidad de hebras
*/
void distribuirGrilla(int n, int cHebras);

/*Procedimiento: Inicia los mutex de la grilla
Entrada: la estructura grilla.
*/
void inicializarMutexs(Grilla* sp);

/* Funcion: transforma una posicion numerica de la grilla en una
coordenada
Entrada: la posicion numerica y la cantidad de columnas de la grilla
Salida: el par ordenado correspondiente a la posicion numerica entregada
*/
int* desparametrizar(int numero, int columnas);

/*Verifica si numero de hilos es minimo 1
Entrada: n->numero de hilos ingresados por consola
Salida: 1-> numero valido
		0-> numero invalido
*/
int validarNumeroHilos(int numeroHilos);

/*Verificar si archivo de entrada existe
Etrada: nombre de arhcivo de entrada
Salida: 1-> archivo existe
		0-> archivo no existe
*/
int validarArchivoEntrada(char* nombrearchivo);

/*Verifica si las dimensiones de la matriz son validas
debe ser mayor o igual a 2x2
Entrada: n numFIla y m numColumna
Salida: 1-> dimensiones validas
0-> dimensiones no validas
*/
int validarDimensionesMatriz(int n, int m);

/*Se espera que al menos el numero de palabras sea 1
Este parametro por consola luego no es tomado en cuenta
Entrda: numero de palabras ingresado por consola
Salida: 1->valido
		0->no valido
*/
int validarNumeroPalabras(int n);

/*Verifica si el nombre del archivo de salida fue ingresado
Se verifica si es distinto de null
Entrada: char* que contiene nombre del archivo de salida
Salida: 1-> nombre es distinto de null y valido
		0-> no se ingresó
*/
int validarNombreArchivoSalida(char* nombreSalida);

/*Verifica que todos los parametros pro consola sean validos
Entrada: parametros recibidos por consola
Salida: 1-> parametros son validos
		0-> parametros no son validos
*/
int validarEntradas(ParamConsola param);

Grilla* crearMatriz(int n);

void copiarCapaMatriz(Grilla* grilla, int desde, int hacia);

void mostrarMatriz(float*** matriz, int n);

void ponerUnos(Grilla* grilla);

void condicionInicial(Grilla* grilla);

void condicionTiempoUno(Grilla* g);

void algoritmoSchoedinger(Grilla* g);

void ecuacionSchoedinger(Grilla* grilla, int t);

#endif