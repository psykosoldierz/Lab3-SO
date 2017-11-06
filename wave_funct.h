#ifndef _WAVE_FUNCT_H
#define _WAVE_FUNCT_H
/*Inicializa una barrera de count hebras
Entrada: barrier bp, count: cantidad de hebras a detener
*/
int barrier_init(barrier_t *bp, int count);

/*Funcion para esperar en la barrera. Si llegan todas
las hebras, entonces se levanta barrera.
Entrada: barrier bp
Salida: int-> estado
*/
int barrier_wait(barrier_t *bp);

/*Funcion: recibe los parametros por consola
Entrada: struct ParamConsola
Salida:struct ParamConsola con valores guardados
*/
ParamConsola recibirParametrosEntrada(int argc, char **argv);


/*Verifica si numero de hilos es minimo 1
Entrada: n->numero de hilos ingresados por consola
Salida: 1-> numero valido
		0-> numero invalido
*/
int validarNumeroHilos(int numeroHilos);

/*Verificar si el nombre del archivo de salida existe
Etrada: nombre de arhcivo de entrada
Salida: 1-> archivo existe
		0-> archivo no existe
*/
int validarArchivoSalida(char* nombreArchivo);

/*Verifica si las dimensiones de la matriz son validas
debe ser mayor o igual a 2x2
Entrada: n numFIla y m numColumna
Salida: 1-> dimensiones validas
0-> dimensiones no validas
*/
int validarDimensionesMatriz(int n);

/*
Entrda: numero de pasos
Salida: 1->valido
		0->no valido
*/
int validarNumeroPasos(int numeroPasos);

/*Valida que valor t (iteraciones) sea al menos uno
Etrada: numero de interaciones
Salida:1->valido
		0-> no valido
*/
int validarNumeroIteracion(int numeroIteracion);

/*Verifica que todos los parametros pro consola sean validos
Entrada: parametros recibidos por consola
Salida: 1-> parametros son validos
		0-> parametros no son validos
*/
int validarEntradas(ParamConsola param);

/*Crea la matriz de la grilla con dimension n
Entrada: dimension de grilla n
Salida: stuct Grilla
*/
Grilla* crearMatriz(int n);

/*Copia una capa de la matriz en otra, de forma paralelizada.
Entrada: grilla, capa desde donde copiar, capa hacia donde copiar,
coordenada de inicio, cantidad de posiciones a copiar
*/
void copiarCapaMatrizParalelizado(Grilla* grilla, int desde, int hacia, Coordenada coorInicial, int cantidadPosiciones);

/*Muestra la matriz de dimension n, en sus 3 capas
Entrada: matriz y dimension
*/
void mostrarMatriz(float*** matriz, int n);

/*Calcula la posicion de inicio (de 0 a n^2 -1) en la que
el hilo debe comenzar a escribir.
Entrada: catidad de posiciones por hilo, numero de hilo a calcular
Salida: posicion en la que debe comenzar.
*/
int calcularPosicionComienzo(int* posicionesPorHilo, int numeroHilo);

/*Obtiene la coordenada (x,y) donde el hilo debe comenzar a trabajar
entrada: posicion inicial y dimension de matriz
Salida: coordenada inicial (x,y)
*/
Coordenada obtenerPosicionInicial(int posicionInicial, int dimension);

void distribuirMatriz(int numeroHilos, int dimension, Hebra* dHebra);

void condicionInicialParalelizado(Grilla* g, Coordenada coorInicio, int cantidadPosiciones);

void condicionTiempoUnoParalelizado(Grilla* g, Coordenada coorInicial, int cantidadPosiciones);

void algoritmoSchoedingerParalelizado(Grilla* g, Coordenada coorInicial, int cantidadPosiciones);

void* ecuacionSchoedingerHebra(void* id);

void esperarHilos(int numeroHilos);

void inicializarBarreras(int numeroBarreras, int numHilos);

void inicializarVariablesGlobales(int numHilos, int dimension, int valorIteraciones);

void lanzarHilos(int numHilos);

#endif
