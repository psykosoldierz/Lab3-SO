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

/* Distribuye la cantidad de posiciones de la matriz, dependiendo de la cantidad
de hilos declaradas
Entrada: numero de hilos, dimension de la matriz y la estructura hebra
*/
void distribuirMatriz(int numeroHilos, int dimension, Hebra* dHebra);

/* Procedimiento que realiza la condicion inicial de la ecuacion de Schroedinger
Enetrada: La Grilla, la coordenada de inicio de la hebra y la cantidad de
posiciones
*/
void condicionInicialParalelizado(Grilla* g, Coordenada coorInicio, int cantidadPosiciones);

/* Procedimiento que realiza la condicion tiempo uno de la ecuacion de Schroedinger
Enetrada: La Grilla, la coordenada de inicio de la hebra y la cantidad de
posiciones
*/
void condicionTiempoUnoParalelizado(Grilla* g, Coordenada coorInicial, int cantidadPosiciones);

/* Procedimiento que realiza la condicion tiempo uno de la ecuacion de Schroedinger
Enetrada: La Grilla, la coordenada de inicio de la hebra y la cantidad de
posiciones
*/
void algoritmoSchoedingerParalelizado(Grilla* g, Coordenada coorInicial, int cantidadPosiciones);

/* Realiza la ejecucion completa de la ecuacion de Schroedinger sincronizando
la ejecucion de los hilos
Entrada: el id de un hilo
*/
void* ecuacionSchoedingerHebra(void* id);

/* Espera que todas las hebras hayan realizado la ejecucion planificada
Entrada: la cantidad de hilos
*/
void esperarHilos(int numeroHilos);

/* Procedimiento que inicializa las barreras necesarias para la ejecucion del
programa
Entrada: Cantidad de barreras que seran utilizadas en la ejecucion de el Procedimiento
ecuacionSchoedingerHebra y el numero de hilos
*/
void inicializarBarreras(int numeroBarreras, int numHilos);

/* Procedimiento que inicializa las variables globales para la ejecucion del programa
principal
Entrada: cantidad de hilos, dimension de la matriz
*/
void inicializarVariablesGlobales(int numHilos, int dimension, int valorIteraciones, int valorIteracionSalida);

/* Procedimiento que inicia la ejecucion de los hijos del programa
Entrada: la cantidad de hilos a lanzar
*/
void lanzarHilos(int numHilos);

/* Procedimiento que copia la matriz resultante en un archivo en formato no-binario
Entrada: la matriz, la cantidad de filas de la matriz y el nombre del archivo de
salida
*/
void matrizArchivo(float **matriz, int n, char *nombreArchivo);

#endif
