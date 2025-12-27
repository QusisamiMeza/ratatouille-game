#include "restaurant.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int NUM_MILANESA = 1;
const int NUM_HAMBURGUESA = 2;
const int NUM_PARRILLA = 3;
const int NUM_RATATOUILLE = 4;
const int INTERVALO_PEDIDO = 4;
const int CANT_MIN_PEDIDO = 1;
const int CANT_MAX_COMENSALES=4;
const int CANT_MIN_COMENSALES =1;
const int CANT_MIN_PACIENCIA =100;
const int RANGO_PACIENCIA=101;
const int TIEMPO_PREPARACION_MILANESA = 30;
const int TIEMPO_PREPARACION_PARRILLA = 20;
const int TIEMPO_PREPARACION_HAMBURGUESA = 15;
const int TIEMPO_PREPARACION_RATATOUILLE = 25;
const int TIEMPO_CUCARACHA = 25;
const int TIEMPO_LLEGADA_COMENSALES = 15;
const int DISMINUCION_PACIENCIA = -1;
const int PACIENCIA_AGOTADA = 0;
const int PRECIO_MESA_4 = 20000;
const int PRECIO_MESA_1 = 5000;
const int VALOR_MONEDA = 1000;
const int DISTANCIA_MOZO_MESAS = 1;
const int DISTANCIA_MESA_CUCARACHA = 2;
const int PENALIZACION_PACIENCIA_CUCARACHA = -2;
const int SIN_COMENSALES = 0;
const int SIN_PATINES = 0;
const int RESIDUO_MULTIPLO = 0;
const int NUEVO_PEDIDO = 1;
const int UN_PLATO = 1;
const int PEDIDO_LISTO = 0;
const int SIN_MESA_ASIGNADA = -1;
const int SIGUIENTE_PEDIDO = 1;
const int TIEMPO_PREPARACION_INICIAL = 0;
const int PACIENCIA_INICIAL = 0;
const int CANTIDAD_INICIAL_COMENSALES = 0;
const int CANTIDAD_INICIAL_PEDIDOS = 0;
const int CANTIDAD_INICIAL_BANDEJA = 0;
const int CANTIDAD_INICIAL_MESAS = 0;
const int CANTIDAD_INICIAL_PATINES = 0;
const int CANTIDAD_INICIAL_HERRAMIENTAS = 0;
const int CANTIDAD_INICIAL_OBSTACULOS = 0;
const int CANTIDAD_INICIAL_DINERO = 0;
const int CANTIDAD_INICIAL_MOVIMIENTOS = 0;
const int CANTIDAD_INICIAL_PLATOS = 0;
const int CANTIDAD_INICIAL_PREPARACION= 0;
const int CANTIDAD_MONEDAS = 8;
const int CANTIDAD_MOPA = 1;
const int CANTIDAD_PATINES = 5;
const int CANTIDAD_CHARCOS = 5;
const int MOVIMIENTO_INICIAL = 0;
const int FILA_ANTERIOR = -1;
const int MESA_LATERAL= 1;
const int ELEMENTO_ELIMINADO = -1;
const int ELEMENTO_AGREGADO = -1;
const int ELEMENTO_NO_ENCONTRADO = -1;
const int ULTIMO_ELEMENTO = -1;
const int CANT_MESAS_DE_4 = 4;
const int CANT_MESAS_DE_1 = 6;
const int ASIENTOS_MESAS_4 = 4;
const int ASIENTOS_MESAS_1 = 1;
const int ANCHO_MESA_1 = 1;
const int ANCHO_MESA_4 = 2;
const int SIN_ANCHO = 0;
const char MILANESA = 'M';
const char HAMBURGUESA = 'H';
const char PARRILLA = 'P';
const char RATATOUILLE = 'R';
const char SIN_PEDIDO = '-';
const char TERRENO_BASE = '.';
const char CARACTER_PARED = '|';
const int MIN_FILA = 0;
const int MIN_COLUMNA = 0;
const char* ENTRADA_RESTAURANTE="╭──────────────────|   |─────────────────╮";
const char* PARED_POSTERIOR="╰────────────────────────────────────────╯";
const int ERROR=1;

#define MOZO 'L'
#define MESA 'T'
#define COCINA 'C'
#define CHARCOS 'H'
#define MONEDAS 'M'
#define PATINES 'P'
#define CUCARACHAS 'U'
#define COMENSAL 'X'

#define SIN_PLATOS 0
#define SIN_PEDIDOS 0
#define ESTADO_JUEGO_GANADO 1
#define ESTADO_JUEGO_PERDIDO -1
#define ESTADO_JUEGO_JUGANDO 0
#define MIN_DINERO_PARA_GANAR 150000
#define TOTAL_MOVIMIENTOS_MAX  200
#define MOPA 'O'
#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define PATINES 'P'
#define TOMAR_PEDIDO 'T'

//PRE: las posiciones deben estar dentro de los limites.
//POST: devuelve true si son iguales, false en caso contrario
bool es_posicion_igual(coordenada_t posicion_nueva, coordenada_t posicion_a_comparar) {
    return (posicion_nueva.fil == posicion_a_comparar.fil && posicion_nueva.col == posicion_a_comparar.col);
}

//PRE: las posiciones deben estar dentro de los limites, mesas deben estar inicializadas
//POST: devuelve true si son iguales, false en caso contrario
bool es_posicion_en_mesas(mesa_t mesas[MAX_MESAS], int cantidad_mesas, coordenada_t posicion_nueva) {
    bool esta_libre = false;
    for (int i = 0; i < cantidad_mesas; i++) {
        for (int j = 0; j < mesas[i].cantidad_lugares; j++) {
            if (es_posicion_igual(posicion_nueva, mesas[i].posicion[j])) {
                esta_libre = true;
            }
        }
    }
    return esta_libre;
}

//PRE: las posiciones deben estar dentro de los limites, objeto debe estar inicializado
//POST: devuelve true si son iguales, false en caso contrario
bool es_posicion_en_objeto(objeto_t* objeto, int cantidad_objetos, coordenada_t posicion_nueva) {
    bool esta_libre = false;
    for (int i = 0; i < cantidad_objetos; i++) {
        if (es_posicion_igual(posicion_nueva, objeto[i].posicion)) {
            esta_libre = true;
        }
    }
    return esta_libre;
}

//PRE: tener inicializado las cantidades de cada objeto (mesas, herramientas, obstaculos), la posicion nueva debe ser una posicion valida (dentro de los limites)
//POST: deuvelve true si posicion_nueva es diferente a alguna ya inicializada, caso contrario devuelve false
bool es_posicion_libre_general (juego_t juego, coordenada_t posicion_nueva){

    if (es_posicion_en_mesas(juego.mesas, juego.cantidad_mesas, posicion_nueva)) {
        return false;
    }
    if (es_posicion_igual(posicion_nueva, juego.cocina.posicion)) {
        return false;
    }
    if (es_posicion_igual(posicion_nueva, juego.mozo.posicion)) {
        return false;
    }
    if (es_posicion_en_objeto(juego.herramientas, juego.cantidad_herramientas, posicion_nueva)) {
        return false;
    }
    if (es_posicion_en_objeto(juego.obstaculos, juego.cantidad_obstaculos, posicion_nueva)) {
        return false;
    }
    return true;
}

//PRE: uso general, excepto para mesas
//POST: genera aleatoriamente una posicion (fila-columna) mientras sea diferente a las demas
coordenada_t generar_posicion (juego_t juego){
    coordenada_t posicion_nueva;
    do{
        posicion_nueva.fil = rand() % (MAX_FILAS);
        posicion_nueva.col = rand() % (MAX_COLUMNAS);
    }while (!es_posicion_libre_general(juego, posicion_nueva));
    return posicion_nueva;
}

//PRE:uso exclusivo para mesas, la posicion debe ser valida (dentro de los maximos), 
//POST: verifica que la parte superior e inferior de la mesa esten libres
bool es_pasillo_libre_mesas_fil (coordenada_t posicion_nueva, int posicion_ocupada_fil,int espacio_necesario_mesa_1, int espacio_necesario_mesa_4){
    return posicion_nueva.fil == posicion_ocupada_fil + espacio_necesario_mesa_1 ||
           posicion_nueva.fil == posicion_ocupada_fil - espacio_necesario_mesa_1 ||
           posicion_nueva.fil == posicion_ocupada_fil - espacio_necesario_mesa_4 ||
           posicion_nueva.fil == posicion_ocupada_fil + espacio_necesario_mesa_4 ||
            posicion_nueva.fil == posicion_ocupada_fil;
}

//PRE: uso exclusivo para mesas, la posicion debe ser valida (dentro de los maximos)
//POST: verifica que los costados de la mesa esten libres
bool es_pasillo_libre_mesas_col (coordenada_t posicion_nueva, int posicion_ocupada_col,int espacio_necesario_mesa_1, int espacio_necesario_mesa_4){
    return posicion_nueva.col == posicion_ocupada_col + espacio_necesario_mesa_1||
            posicion_nueva.col == posicion_ocupada_col - espacio_necesario_mesa_1||
            posicion_nueva.col == posicion_ocupada_col + espacio_necesario_mesa_4||
            posicion_nueva.col == posicion_ocupada_col - espacio_necesario_mesa_4||
            posicion_nueva.col == posicion_ocupada_col;
}

//PRE: uso exclusivo para mesas, la posicion debe ser valida (dentro de los maximos)
//POST: retorna true si el pasillo esta libre o false si no esta libre.
bool es_pasillo_libre_mesas (coordenada_t posicion_nueva,int posicion_fil,int posicion_col,int espacio_necesario_mesa_1, int espacio_necesario_mesa_4){
    return es_pasillo_libre_mesas_fil(posicion_nueva, posicion_fil,espacio_necesario_mesa_1, espacio_necesario_mesa_4) && 
            es_pasillo_libre_mesas_col(posicion_nueva, posicion_col,espacio_necesario_mesa_1, espacio_necesario_mesa_4);
}

//PRE: uso exclusivo para mesas, la posicion debe ser valida (dentro de los maximos)
//POST: verifica que la posicion sea libre entre mesas, considerando un espacio de diferencia (pasillo entre mesas)
bool es_posicion_libre_mesa (int cantidad_mesas, mesa_t* mesas, coordenada_t posicion_nueva,int espacio_necesario_mesa_1, int espacio_necesario_mesa_4){
    bool posicion_libre = true;

    for (int i = 0; i < cantidad_mesas; i++){
        for (int j = 0; j < mesas[i].cantidad_lugares; j++){
            int posicion_fil = mesas[i].posicion[j].fil;
            int posicion_col = mesas[i].posicion[j].col;
            if (es_pasillo_libre_mesas (posicion_nueva,posicion_fil, posicion_col, espacio_necesario_mesa_1, espacio_necesario_mesa_4)){
                posicion_libre = false;
            }
        }
    }
    return posicion_libre;
}

//PRE:  si hay mesas, tienen que que estar inicializadas
//POST:  genera una posicion (fila, columna), mientras sea diferente otra posicion de alguna mesa
coordenada_t generar_posicion_mesas (juego_t juego,int espacio_necesario_mesa_1, int espacio_necesario_mesa_4){
    coordenada_t posicion_nueva;
    do{
        posicion_nueva.fil = rand() % (MAX_FILAS + FILA_ANTERIOR);
        posicion_nueva.col = rand() % (MAX_COLUMNAS + FILA_ANTERIOR);
    }while (!es_posicion_libre_mesa(juego.cantidad_mesas, juego.mesas, posicion_nueva, espacio_necesario_mesa_1, espacio_necesario_mesa_4));
    return posicion_nueva;
}

//PRE: -
//POST: inicializará las mesas, carga toda su informacion
void inicializar_mesas(juego_t *juego){
    for (int i = 0; i < CANT_MESAS_DE_1;i++){
        juego->mesas[i].cantidad_comensales = CANTIDAD_INICIAL_COMENSALES;
        juego->mesas[i].cantidad_lugares = ASIENTOS_MESAS_1;
        juego->mesas[i].paciencia = PACIENCIA_INICIAL;
        juego->mesas[i].pedido_tomado = false;
        juego->mesas[i].posicion[0] = generar_posicion_mesas(*juego,ANCHO_MESA_1,SIN_ANCHO);
        juego->cantidad_mesas++;
    }
    for (int i = 0; i < CANT_MESAS_DE_4; i++){
        juego->mesas[juego->cantidad_mesas].cantidad_comensales = CANTIDAD_INICIAL_COMENSALES;
        juego->mesas[juego->cantidad_mesas].cantidad_lugares = ASIENTOS_MESAS_4;
        juego->mesas[juego->cantidad_mesas].paciencia = PACIENCIA_INICIAL;
        juego->mesas[juego->cantidad_mesas].pedido_tomado = false;
        coordenada_t mesa_fija = generar_posicion_mesas(*juego,ANCHO_MESA_1,ANCHO_MESA_4);
        for (int j = 0; j < ASIENTOS_MESAS_4; j++){
            if (j == 0){
                juego->mesas[juego->cantidad_mesas].posicion[j] = mesa_fija;
            }else if (j == 1){
                juego->mesas[juego->cantidad_mesas].posicion[j].fil = mesa_fija.fil;
                juego->mesas[juego->cantidad_mesas].posicion[j].col = mesa_fija.col + MESA_LATERAL; 
            }else if (j == 2){
                juego->mesas[juego->cantidad_mesas].posicion[j].fil = mesa_fija.fil + MESA_LATERAL;
                juego->mesas[juego->cantidad_mesas].posicion[j].col = mesa_fija.col;
            }else if (j == 3){
                juego->mesas[juego->cantidad_mesas].posicion[j].fil = mesa_fija.fil + MESA_LATERAL;
                juego->mesas[juego->cantidad_mesas].posicion[j].col = mesa_fija.col + MESA_LATERAL;
            }
        }
        juego->cantidad_mesas++;
    }
}

//PRE: -
//POST: inicializará las herramientas, carga toda su informacion
void inicializar_herramientas (juego_t *juego){
    for (int i=0; i < CANTIDAD_MOPA; i++){
        juego->herramientas[juego->cantidad_herramientas].tipo = MOPA;
        juego->herramientas[juego->cantidad_herramientas].posicion = generar_posicion(*juego);
        juego->cantidad_herramientas++;
    }
    for (int i=0; i < CANTIDAD_MONEDAS; i++){
        juego->herramientas[juego->cantidad_herramientas].tipo= MONEDAS;
        juego->herramientas[juego->cantidad_herramientas].posicion = generar_posicion (*juego);
        juego->cantidad_herramientas++;
    }
    for (int i=0; i < CANTIDAD_PATINES; i++){
        juego->herramientas[juego->cantidad_herramientas].tipo= PATINES;
        juego->herramientas[juego->cantidad_herramientas].posicion = generar_posicion(*juego);
        juego->cantidad_herramientas++;
    }
}

//PRE:-
//POST: inicializará los pedido cargando toda su informacion
void inicializar_pedidos_platos (pedido_t* pedidos){
    for (int i = 0; i < MAX_PEDIDOS; i++) {
        pedidos[i].id_mesa = SIN_MESA_ASIGNADA;  
        pedidos[i].cantidad_platos = CANTIDAD_INICIAL_PLATOS;
        pedidos[i].tiempo_preparacion = CANTIDAD_INICIAL_PREPARACION;
        for(int j = 0; j < MAX_PLATOS; j++){
            pedidos[i].platos[j] = SIN_PEDIDO;
        }
    }
}

//PRE: -
//POST: inicializará el mozo (Linguini), carga toda su informacion
void inicializar_mozo (juego_t * juego){
    juego->mozo.posicion = generar_posicion (*juego);
    juego->mozo.cantidad_patines = CANTIDAD_INICIAL_PATINES;
    juego->mozo.cantidad_pedidos = CANTIDAD_INICIAL_PEDIDOS;
    juego->mozo.cantidad_bandeja = CANTIDAD_INICIAL_BANDEJA;
    juego->mozo.tiene_mopa =false;
    juego->mozo.patines_puestos = false;
    juego->mozo.cantidad_bandeja = CANTIDAD_INICIAL_BANDEJA;
    juego->mozo.cantidad_pedidos = CANTIDAD_INICIAL_PEDIDOS;
    inicializar_pedidos_platos(juego->mozo.pedidos);
    inicializar_pedidos_platos(juego->mozo.bandeja);
}

//PRE: -
//POST: inicializará la cocina, carga toda su informacion
void inicializar_cocina (juego_t *juego){
    juego->cocina.posicion = generar_posicion (*juego);
    juego->cocina.cantidad_listos = CANTIDAD_INICIAL_PLATOS;
    juego->cocina.cantidad_preparacion = CANTIDAD_INICIAL_PREPARACION;
    juego->cocina.platos_listos = NULL;
    juego->cocina.platos_preparacion = NULL;
}

//PRE: -
//POST: inicializará los obstauculos, carga toda su informacion
void inicializar_obstaculos (juego_t *juego){
    for (int i=0; i < CANTIDAD_CHARCOS; i++){
        juego->obstaculos[juego->cantidad_obstaculos].tipo= CHARCOS;
        juego->obstaculos[juego->cantidad_obstaculos].posicion = generar_posicion(*juego);
        juego->cantidad_obstaculos++;
    }
}
//PRE: -
//POST: inicializará las cantidades iniciales de Linguini , las mesas , las herramientas, los obstáculos, los movimientos y el dinero.
void inicializar_cantidades (juego_t *juego){
    juego->cantidad_mesas = CANTIDAD_INICIAL_MESAS;
    juego->cantidad_herramientas = CANTIDAD_INICIAL_HERRAMIENTAS;
    juego->cantidad_obstaculos = CANTIDAD_INICIAL_OBSTACULOS;
    juego->movimientos = CANTIDAD_INICIAL_MOVIMIENTOS;
    juego->dinero = CANTIDAD_INICIAL_DINERO;
}

void inicializar_juego(juego_t *juego){
    inicializar_cantidades(juego);
    inicializar_mesas(juego);
    inicializar_cocina(juego);
    inicializar_mozo(juego);
    inicializar_herramientas(juego);
    inicializar_obstaculos(juego);
}

//PRE: las coordenadas deben estar inicializadas
//POST: calcula la distancia manhattan
int calcular_distancia_manhattan (coordenada_t primera_coordenada, coordenada_t segunda_coordenada){
    int resta_filas = primera_coordenada.fil - segunda_coordenada.fil;
    int resta_columnas = primera_coordenada.col - segunda_coordenada.col;
    if (resta_filas < 0) {
        resta_filas = -resta_filas;
    }
    if (resta_columnas < 0) {
        resta_columnas= -resta_columnas;
    }
    return resta_filas + resta_columnas;
}

//PRE:-
//POST: retorna un numero aleatorio del 1 al 4 inclusive.
int generar_cantidad_aleatoria_comensales (){
    return (rand() % CANT_MAX_COMENSALES) + CANT_MIN_COMENSALES;
}

//PRE:-
//POST: retorna un numero aleatorio del 100 al 200 inclusive.
int generar_cantidad_aleatoria_paciencia (){
    return rand() % RANGO_PACIENCIA + CANT_MIN_PACIENCIA;
}

//PRE: mesa debe estar inicializado
//POST: si la mesa está vacia y tiene la cantidad de lugares suficientes, asigna los comensales a esa mesa y rellena con la informacion
void localizar_comensales(bool *comensales_asignados, int cantidad_comensales,int cantidad_paciencia, mesa_t* mesas){
    if (mesas->cantidad_comensales == SIN_COMENSALES && mesas->cantidad_lugares >= cantidad_comensales){
        mesas->cantidad_comensales = cantidad_comensales;
        mesas->paciencia = cantidad_paciencia;
        *comensales_asignados=true;
        return;
    }
    *comensales_asignados=false;
}

//PRE: -
//POST: reinicia la mesa con valores iniciales
void desocupar_mesa (mesa_t* mesa){
    mesa->cantidad_comensales = SIN_COMENSALES;
    mesa->paciencia = PACIENCIA_INICIAL;
    mesa->pedido_tomado = false;
}

//PRE: meza y obstaculos deben estar inicializados
//POST: baja la paciencia de las mesas de dos en dos si hay una cucaracha con distancia manhattan de dos 
void penalizacion_cucaracha(int cantidad_obstaculos, mesa_t* mesa, objeto_t* obstaculos){
    for (int i = 0; i < cantidad_obstaculos; i++){
        if (obstaculos[i].tipo == CUCARACHAS && calcular_distancia_manhattan(*mesa->posicion, obstaculos[i].posicion) <= DISTANCIA_MESA_CUCARACHA){
            mesa->paciencia += PENALIZACION_PACIENCIA_CUCARACHA;
        }
    }
}

//PRE: mesas y obstaculos deben estar inicializados
//POST: por cada movimiento le resta una unidad de paciencia a las mesas que tienen comensales, si hay una cucaracha cerca, se les resta dos unidades
//      más extras.
void actualizar_paciencia (int cantidad_obstaculos,mesa_t* mesa, objeto_t* obstaculos){
    if (mesa->cantidad_comensales > SIN_COMENSALES){
        penalizacion_cucaracha(cantidad_obstaculos, mesa, obstaculos);
        mesa->paciencia+=DISMINUCION_PACIENCIA;
    }
}

//PRE: -
//POST: devuelve true si todavia los comensales no fueron asignados y la mesas no deben tener ningun comensal. Ademas la 
//      cantidad lugares en la mesa debe ser suficiente para todos los comensales 
bool es_localizar_comensales (bool comensales_asignados, int cantidad_comensales, int cantidad_lugares){
    return (!comensales_asignados && cantidad_comensales == SIN_COMENSALES && cantidad_lugares >= cantidad_comensales);
}

//PRE: el juego debe estar inicializado.
//POST: Actualiza las mesas:
//      - Si el movimiento actual es multiplo de 15 y hay alguna mesa disponible, se generan asignan en esa mesa
//      - Si hubo un movimiento, se disminuye la paciencia en uno, ademas se le resta dos extra si hay alguna cucaracha cerca
void actualizar_mesas (int numero_movimiento, juego_t* juego, int movimiento_anterior){
    bool es_comensales_asignados = false;
    if (numero_movimiento % TIEMPO_LLEGADA_COMENSALES == RESIDUO_MULTIPLO){
        int cantidad_comensales = generar_cantidad_aleatoria_comensales();
        int cantidad_paciencia = generar_cantidad_aleatoria_paciencia();
        for (int i = 0; i < juego->cantidad_mesas; i++){
            if (es_localizar_comensales(es_comensales_asignados,juego->mesas[i].cantidad_comensales,juego->mesas[i].cantidad_lugares)){
                localizar_comensales(&es_comensales_asignados,cantidad_comensales,cantidad_paciencia, &juego->mesas[i]);
            }
        }
    }
    if (numero_movimiento != movimiento_anterior){
        for (int i = 0; i < juego->cantidad_mesas; i++){
            actualizar_paciencia(juego->cantidad_obstaculos,&juego->mesas[i], juego->obstaculos);
            if (juego->mesas[i].paciencia == PACIENCIA_AGOTADA && juego->mesas[i].cantidad_comensales != SIN_COMENSALES){
                desocupar_mesa(&juego->mesas[i]);
            }
        }
    }
}

//PRE:-
//POST: retorna un numero aleatorio del 1 al 4 inclusive.
int generar_pedido_aleatorio (){
    return (rand() % INTERVALO_PEDIDO) + CANT_MIN_PEDIDO;
}

//PRE:-
//POST: segun al numero aleatorio se le asigna su equivalente:
//      - Milanesa napolitana: Se representa con el número 1
//      - Hamburguesa: Se representa con el número 2
//      - Parrilla: Se representa con el número 3.
//      - Ratatouille: Se representa con el número 4.
char asignar_platos (){
    int numero_plato = generar_pedido_aleatorio();
    if (numero_plato == NUM_MILANESA){
        return MILANESA;
    }
    if (numero_plato == NUM_HAMBURGUESA){
        return HAMBURGUESA;
    }
    if (numero_plato == NUM_PARRILLA){
        return PARRILLA;
    }
    if (numero_plato == NUM_RATATOUILLE){
        return RATATOUILLE;
    }
    return SIN_PEDIDO;
}

//PRE: platos debe contener valores validos:
//      MILANESA  'M'
//      HAMBURGUESA  'H'
//      PARRILLA  'P'
//      RATATOUILLE  'R'
//POST: segun los platos, retorna el mayor tiempo entre estos
int calcular_tiempo_preparacion (char platos[MAX_PLATOS], int cantidad_comensales){
    int tiempo_preparacion_maximo = TIEMPO_PREPARACION_INICIAL;
    for (int i = 0; i < cantidad_comensales; i++){
        int tiempo_preparacion_actual = TIEMPO_PREPARACION_INICIAL;
        if (platos[i] == MILANESA ){
            tiempo_preparacion_actual = TIEMPO_PREPARACION_MILANESA;
        }else if (platos[i] == HAMBURGUESA){
            tiempo_preparacion_actual = TIEMPO_PREPARACION_HAMBURGUESA;
        }else if (platos[i] == PARRILLA){
            tiempo_preparacion_actual = TIEMPO_PREPARACION_PARRILLA;
        }else if (platos[i] == RATATOUILLE){
            tiempo_preparacion_actual = TIEMPO_PREPARACION_RATATOUILLE;
        }
        if (tiempo_preparacion_actual > tiempo_preparacion_maximo){
            tiempo_preparacion_maximo = tiempo_preparacion_actual;
        }
    }
    return tiempo_preparacion_maximo;
}

//PRE: pedidos debe estar inicializado
//POST: rellena el pedido con su informacion 
void asignar_pedidos(pedido_t *pedido, int id_mesa, int cantidad_comensales){
    pedido->id_mesa = id_mesa;
    for (int i = 0; i < cantidad_comensales; i++){
        pedido->platos[i] = asignar_platos();
    }
    pedido->cantidad_platos = cantidad_comensales;
    pedido->tiempo_preparacion = calcular_tiempo_preparacion(pedido->platos,cantidad_comensales);
}

//PRE: mesas y mozo deben estar inicializados
//POST: crea un nuevo pedido y se lo agrega al mozo->pedidos.
void tomar_pedido(mesa_t* mesa, mozo_t* mozo, int id_mesa){
    asignar_pedidos(&mozo->pedidos[mozo->cantidad_pedidos], id_mesa, mesa->cantidad_comensales);
    mozo->cantidad_pedidos++;
    mesa->pedido_tomado=true;
}

//PRE:-
//POST: retorna true si la accion es igual a accion_a_comparar, false en caso contrario
bool es_accion_valida (char accion, char accion_a_comparar){
    return accion == accion_a_comparar;
}

//PRE:-
//POST: retorna true si la posicion del mozo es igual a la posicion_elemento
bool es_misma_posicion_mozo_elemento (coordenada_t posicion_elemento, coordenada_t posicion_mozo){
    return (posicion_elemento.fil == posicion_mozo.fil && posicion_elemento.col == posicion_mozo.col);
}

//PRE: cantidad_lugares debe ser valido (ASIENTOS_MESAS_1 o ASIENTOS_MESAS_4)
//POST: suma el precio del servicio segun la cantidad de lugares en la mesa
void pagar_cuenta (int cantidad_lugares, int* dinero){
    if (cantidad_lugares == ASIENTOS_MESAS_1){
        *dinero += PRECIO_MESA_1;
    }else{
        *dinero += PRECIO_MESA_4;
    }
}

//PRE: pedido_origen debe tener valores validos
//POST: se copia la informacion de pedido_origen a pedido_destino
void pasar_platos(pedido_t pedido_origen, pedido_t* pedido_destino){
    pedido_destino->id_mesa = pedido_origen.id_mesa;
    pedido_destino->cantidad_platos = pedido_origen.cantidad_platos;
    pedido_destino->tiempo_preparacion = pedido_origen.tiempo_preparacion;
    for (int i = 0; i < pedido_origen.cantidad_platos; i++){
        pedido_destino->platos[i] = pedido_origen.platos[i];
    }
}

//PRE: mozo debe estar inicializado, id_mesa debe ser valido (entre 0 y 8), posicion_mesa y posicion_mozo deben estar dentro de los limites
//POST: entrega los pedidos de la bandeja a la mesa segun el id_mesa
void entregar_pedido_comensales (int id_mesa, mozo_t* mozo, bool* es_pedido_entregado, coordenada_t posicion_mesa, coordenada_t posicion_mozo){
    for (int i = 0; i < mozo->cantidad_bandeja; i++){
        if (mozo->bandeja[i].id_mesa == id_mesa && calcular_distancia_manhattan(posicion_mesa,posicion_mozo) == DISTANCIA_MOZO_MESAS){
            pasar_platos(mozo->bandeja[mozo->cantidad_bandeja + ULTIMO_ELEMENTO], &mozo->bandeja[i]);
            mozo->cantidad_bandeja--;
            *es_pedido_entregado = true;
        }
    }
}

//PRE: mesas y mozo deben estar inicializados 
//POST: retorna true si mozo y alguna mesa tiene distancia manhattan igual a 1, false en caso contrario
bool es_mesa_cercana (mesa_t* mesas, coordenada_t posicion_mozo, int cantidad_mesas){
    bool es_cerca = false;
    for (int i = 0; i < cantidad_mesas; i++){
        for (int j = 0; j < mesas[i].cantidad_lugares; j++){
            if (calcular_distancia_manhattan(mesas[i].posicion[j], posicion_mozo) == DISTANCIA_MOZO_MESAS){
                es_cerca = true;
            }
        }
    }
    return es_cerca;
}

//PRE: mozo y mesas deben estar inicializados, posicion_mozo debe ser valido
//POST: si hay alguna mesa con distancia manhattan igual a 1 y hay algun plato en la bandeja con su id, entonces
//      le entrega el pedido, se paga la cuenta y se desocupa la mesa
void finalizar_pedido (mozo_t* mozo, int *dinero, mesa_t* mesas, int cantidad_mesas, coordenada_t posicion_mozo){
    if (mozo->cantidad_bandeja > SIN_PLATOS && es_mesa_cercana(mesas, posicion_mozo, cantidad_mesas)){
        for (int i = 0; i < cantidad_mesas; i++){
            bool es_pedido_entregado = false;
            for (int j = 0; j < mesas[i].cantidad_lugares; j++){
                entregar_pedido_comensales(i, mozo, &es_pedido_entregado, mesas[i].posicion[j], posicion_mozo); 
            }
            if (es_pedido_entregado){
                pagar_cuenta(mesas[i].cantidad_lugares, dinero);
                desocupar_mesa(&mesas[i]);
            }
        }
    }
}

//PRE: -
//POST: se libera la memoria de platos si es que es diferente a NULL
void liberar_memoria_platos (pedido_t** platos){
    if (*platos != NULL){
        free (*platos);
        *platos = NULL;
    }
}

//PRE: cocina debe estar inicializada, cantidad_platos debe ser valido
//POST: se agrega un plato al vector platos haciendo un realloc y se le aunmenta la cantidad de esos platos
void agregar_plato (pedido_t** platos, int* cantidad_platos, cocina_t* cocina){
    size_t total_platos = (size_t)(*cantidad_platos + NUEVO_PEDIDO);
    pedido_t* temp = realloc(*platos, sizeof(pedido_t) * total_platos);
    if (temp == NULL){
        printf("Error al reasignar memoria para platos");
        liberar_memoria_platos(&cocina->platos_listos);
        liberar_memoria_platos(&cocina->platos_preparacion);
        exit(ERROR);
    }
    *platos = temp;
    (*cantidad_platos)++;
}

//PRE: cocina debe estar inicializada, cantidad_platos debe ser valido
//POST: se elimina un plato al vector platos haciendo un realloc y se le disminuye la cantidad de esos platos
void eliminar_plato (pedido_t** platos, int* cantidad_platos, cocina_t* cocina){
    if (*cantidad_platos > SIN_PEDIDOS){
        (*cantidad_platos)--;
        if (*cantidad_platos > SIN_PEDIDOS){
            size_t total_platos = (size_t)(*cantidad_platos);
            *platos = realloc(*platos,total_platos * sizeof (pedido_t));
            if (*platos == NULL){
                printf("Error al reasignar memoria para platos");
                liberar_memoria_platos(&cocina->platos_listos);
                liberar_memoria_platos(&cocina->platos_preparacion);
                exit(ERROR);
            }
        }else if (*cantidad_platos == SIN_PEDIDOS){
            free(*platos);
            *platos = NULL;
        }
    }   
}



//PRE: cocina debe estar inicializado, nuevo_pedido debe estar inicializado
//POST: se agregan un plato y se lo coloca en la ultima posicion de ese vector platos (osea en el espacio plato agregado)
void agregar_pedidos_preparacion (cocina_t* cocina, pedido_t nuevo_pedido){
    agregar_plato(&cocina->platos_preparacion, &cocina->cantidad_preparacion, cocina);
    pasar_platos(nuevo_pedido, &cocina->platos_preparacion[cocina->cantidad_preparacion + ULTIMO_ELEMENTO]);
}

//PRE:  cocina y mozo deben estar inicializados
//POST: si mozo y cocina estan en la misma posicion y mozo tiene pedidos, entonces pasa los pedidos a la cocina en preparacion
void entregar_pedidos_cocina (mozo_t* mozo, cocina_t* cocina){
    if (es_misma_posicion_mozo_elemento(cocina->posicion, mozo->posicion) && mozo->cantidad_pedidos > SIN_PEDIDOS){
        int i = 0; 
        while (i < mozo->cantidad_pedidos){
            agregar_pedidos_preparacion(cocina, mozo->pedidos[i]);
            for (int j = i; j < mozo->cantidad_pedidos + ULTIMO_ELEMENTO; j++){
                pasar_platos(mozo->pedidos[j + SIGUIENTE_PEDIDO], &mozo->pedidos[j]);
            }
            mozo->cantidad_pedidos--;
        }
    }
}

//PRE: pedidos debe estar inicializado
//POST: disminuye el tiempo de preparacion de cada pedido
void disminuir_tiempo_pedido (pedido_t* pedidos, int cantidad_platos_preparacion){
    for (int i = 0; i < cantidad_platos_preparacion; i++){
        if (pedidos[i].tiempo_preparacion != PEDIDO_LISTO){
            pedidos[i].tiempo_preparacion--;
        }
    }
}

//PRE: cocina debe estar inicializado
//POST: si algun pedido acabo su tiempo de preparacion, entonces pasa al vector de platos listos
void transferir_pedido_a_listo(cocina_t* cocina){
    for (int i = 0; i < cocina->cantidad_preparacion; i++){
        if (cocina->platos_preparacion[i].tiempo_preparacion == PEDIDO_LISTO){
            pedido_t plato_aux;
            pasar_platos(cocina->platos_preparacion[i], &plato_aux);
            pasar_platos(cocina->platos_preparacion[cocina->cantidad_preparacion + ULTIMO_ELEMENTO], &cocina->platos_preparacion[i]);
            eliminar_plato (&cocina->platos_preparacion, &cocina->cantidad_preparacion, cocina);
            agregar_plato(&cocina->platos_listos, &cocina->cantidad_listos, cocina);
            pasar_platos(plato_aux, &cocina->platos_listos[cocina->cantidad_listos + ULTIMO_ELEMENTO]);
        }
    }
}

//PRE: cocina y mozo deben estar inicializados
//POST: si mozo y cocina tienen la misma posicion y hay platos listos en cocina, entonces se transfiere a la bandeja de mozo
void transferir_platos_a_bandeja(mozo_t* mozo, cocina_t* cocina){
    if (es_misma_posicion_mozo_elemento(cocina->posicion, mozo->posicion) && cocina->cantidad_listos > UN_PLATO && mozo->cantidad_bandeja < MAX_BANDEJA){
        int i = 0;
        while (cocina->cantidad_listos != UN_PLATO && mozo->cantidad_bandeja < MAX_BANDEJA){
            pedido_t plato_aux;
            pasar_platos(cocina->platos_listos[i], &plato_aux);
            pasar_platos(cocina->platos_listos[cocina->cantidad_listos + ULTIMO_ELEMENTO], &cocina->platos_listos[i]);
            pasar_platos(plato_aux, &cocina->platos_listos[cocina->cantidad_listos + ULTIMO_ELEMENTO]);
            mozo->bandeja[mozo->cantidad_bandeja] = cocina->platos_listos[cocina->cantidad_listos + ULTIMO_ELEMENTO];
            mozo->cantidad_bandeja++;
            eliminar_plato(&cocina->platos_listos,&cocina->cantidad_listos, cocina);
            i++;
        }
    }
    if (es_misma_posicion_mozo_elemento(cocina->posicion, mozo->posicion) && cocina->cantidad_listos == UN_PLATO && mozo->cantidad_bandeja < MAX_BANDEJA){ 
        mozo->bandeja[mozo->cantidad_bandeja] = cocina->platos_listos[cocina->cantidad_listos + ULTIMO_ELEMENTO];
        mozo->cantidad_bandeja++;
        eliminar_plato(&cocina->platos_listos,&cocina->cantidad_listos, cocina);
    }
}

//PRE: ultimo_objeto debe estar inicializado
//POST: elimina el objeto del array
void eliminar_objeto (objeto_t* objeto_eliminar, objeto_t ultimo_objeto, int* cantidad_objetos){
    objeto_eliminar->posicion = ultimo_objeto.posicion;
    objeto_eliminar->tipo = ultimo_objeto.tipo;
    (*cantidad_objetos)+=ELEMENTO_ELIMINADO;
}

//PRE: tener mesas inicializadas, id_mesas debe estar inicializado
//POST: recorre los arrays de mesas e id_mesas buscando si hay platos para esa mesa, si hay, reinicia la mesa que pidio ese plato
void perder_comensales (mesa_t* mesas, int* id_mesas, int cantidad_mesas, int tope_id_mesas){
    for (int i = 0; i < cantidad_mesas; i++){
        for (int j = 0; j < tope_id_mesas; j++){
            if (i == id_mesas[j]){
                desocupar_mesa(&mesas[i]);
            }
        }
    }
}

//PRE: juego debe estar inicializado, posicion_mozo debe estar dentro de los limites
//POST: si mozo y algun charco tiene la misma posicion, reinicia la cantidad de la bandeja y tambien las mesas que pidieron los platos que estaban en la bandeja.
void caer_bandeja (juego_t* juego, coordenada_t posicion_mozo){
    for (int i = 0; i < juego->cantidad_obstaculos; i++){
        if (juego->obstaculos[i].tipo == CHARCOS && es_misma_posicion_mozo_elemento(juego->obstaculos[i].posicion, posicion_mozo)){
            int id_mesas[MAX_BANDEJA];
            int tope_id_mesas = juego->mozo.cantidad_bandeja;
            for (int i = 0; i < juego->mozo.cantidad_bandeja; i++){
                id_mesas[i] = juego->mozo.bandeja[i].id_mesa;
            }
            juego->mozo.cantidad_bandeja = SIN_PLATOS;
            perder_comensales(juego->mesas, id_mesas,juego->cantidad_mesas,tope_id_mesas);
        }
    }     
}

//PRE: juego debe estar inicializado, posicion_mozo debe estar dentro de los limites
//POST: si el mozo y alguna cucaracha tienen la misma posicion, entonces elimina a la cucaracha
void matar_cucharacha(juego_t* juego, coordenada_t posicion_mozo){
    int i = 0;
    bool es_cucaracha_muerta = false;
    while (i < juego->cantidad_obstaculos && !es_cucaracha_muerta){
        if (juego->obstaculos[i].tipo == CUCARACHAS && es_misma_posicion_mozo_elemento(juego->obstaculos[i].posicion, posicion_mozo)){
            eliminar_objeto (&juego->obstaculos[i], juego->obstaculos[juego->cantidad_obstaculos + ULTIMO_ELEMENTO], &juego->cantidad_obstaculos);
            es_cucaracha_muerta = true;
        }
        i++;
    }
}

//PRE: juego debe estar inicializado
//POST: cada 15 movimientos se genera una cucaracha de manera aleatoria
void generar_cucarachas (juego_t* juego, int movimiento){
    if (movimiento != MOVIMIENTO_INICIAL && movimiento % TIEMPO_CUCARACHA == RESIDUO_MULTIPLO){
        juego->obstaculos[juego->cantidad_obstaculos].tipo= CUCARACHAS;
        juego->obstaculos[juego->cantidad_obstaculos].posicion = generar_posicion(*juego);
        juego->cantidad_obstaculos++;
    }
}

//PRE: juego debe estar inicializado, posicion_mozo debe estar dentro de los limites
//POST: si mozo y alguna moneda tienen la misma posicion entonces se suma 1000 de dinero y se elimina la moneda del array de objetos
void recolectar_moneda (juego_t* juego, coordenada_t posicion_mozo){
    for (int i = 0; i < juego->cantidad_herramientas; i++){
        if (juego->herramientas[i].tipo == MONEDAS && es_misma_posicion_mozo_elemento(juego->herramientas[i].posicion, posicion_mozo)){
            juego->dinero += VALOR_MONEDA;
            eliminar_objeto (&juego->herramientas[i], juego->herramientas[juego->cantidad_herramientas + ULTIMO_ELEMENTO], &juego->cantidad_herramientas);
        }
    }
}

//PRE: juego debe estar inicializado, posicion_mozo debe estar dentro de los limites
//POST: si mozo y algun patin tienen la misma posicion entonces se suma 1 a cantidad_patines y se elimina el patin del array de objetos
void recolectar_patines(juego_t* juego, coordenada_t posicion_mozo){
    for (int i = 0; i < juego->cantidad_herramientas; i++){
        if (juego->herramientas[i].tipo == PATINES && es_misma_posicion_mozo_elemento(juego->herramientas[i].posicion, posicion_mozo)){
            eliminar_objeto(&juego->herramientas[i],juego->herramientas[juego->cantidad_herramientas + ULTIMO_ELEMENTO],&juego->cantidad_herramientas);
            juego->mozo.cantidad_patines++;
        }
    }
}

//PRE: juego debe estar inicializado, posicion_mozo debe estar dentro de los limites
//POST: si mozo y algun charco tienen la misma posicion y mozo tiene mopa, entonces se elimina el charco del array de obstaculos
void limpiar_charco(juego_t* juego, coordenada_t posicion_mozo){
    if (juego->mozo.tiene_mopa){
        for (int i = 0; i < juego->cantidad_obstaculos; i++){
            if (juego->obstaculos[i].tipo == CHARCOS && es_misma_posicion_mozo_elemento(juego->obstaculos[i].posicion, posicion_mozo)){
                eliminar_objeto(&juego->obstaculos[i],juego->obstaculos[juego->cantidad_obstaculos + ULTIMO_ELEMENTO],&juego->cantidad_obstaculos);
            }
        }
    }
}

//PRE: -
//POST: retorna true si accion es igual a (ARRIBA(W), ABAJO(S), DERECHA(A), IZQUIERDA(D)), caso contrario, false.
bool es_accion_valida_mover (char accion){
    return accion == ARRIBA || accion == ABAJO || accion == DERECHA || accion == IZQUIERDA;
}

//PRE: -
//POST: verifica que la nueva posicion este dentro de los maximos y minimo (0, MAX_FILAS y MAX_COLUMNAS), caso contrario retorna false.
bool es_limite_juego (coordenada_t posicion){
    return posicion.fil >= MIN_FILA && posicion.fil < MAX_FILAS && posicion.col >= MIN_COLUMNA && posicion.col < MAX_COLUMNAS;
}

//PRE: El juego deberá estar inicializado previamente con `inicializar_juego `, la posicion debe estar dentro de los maximos y minimos
//POST:  verifica que la nueva posicion no sea igual que la posicion de alguna mesa
bool es_posicion_mesa (juego_t juego, coordenada_t posicion_nueva){
    bool posicion_desocupada = true;
    for (int i = 0; i < juego.cantidad_mesas; i++){
        for (int j = 0; j < juego.mesas[i].cantidad_lugares; j++){
            if (posicion_nueva.fil == juego.mesas[i].posicion[j].fil && posicion_nueva.col == juego.mesas[i].posicion[j].col){
                posicion_desocupada = false;
            }
        }
    }
    return posicion_desocupada;
}

//PRE: El juego deberá estar inicializado previamente con `inicializar_juego `, la accion debe ser valida
//POST: mueve de coordenada al mozo dentro de los limites del juego
coordenada_t mover_mozo_matriz (coordenada_t posicion_mozo, char accion){
    coordenada_t nueva_posicion_mozo = posicion_mozo;
    if (accion == ARRIBA) {
        nueva_posicion_mozo.fil--;
    } else if (accion == ABAJO) {
        nueva_posicion_mozo.fil++;
    } else if (accion == DERECHA) {
        nueva_posicion_mozo.col++;
    } else if (accion == IZQUIERDA) {
        nueva_posicion_mozo.col--;
    }
    return nueva_posicion_mozo;
}

//PRE: herramientas deben estar inicializadas
//POST: busca el indice de MOPA en el vector de herramientas, si no lo encuentraa devuelve ELEMENTO_NO_ENCONTRADO (-1)
int buscar_mopa (int cantidad_herramientas, objeto_t* herramientas){
    int indice_mopa_encontrada = ELEMENTO_NO_ENCONTRADO;
    for (int i = 0; i < cantidad_herramientas;i++){
        if (herramientas[i].tipo == MOPA){
            indice_mopa_encontrada = i ;
        }
    }
    return indice_mopa_encontrada;
}

//PRE: uso exclusivo para mozo, inicializar juego
//POST:verifica que el mozo y la mopa tengan la misma posicion
bool es_misma_posicion_mopa_mozo (juego_t juego){
    int indice_mopa = buscar_mopa(juego.cantidad_herramientas, juego.herramientas);
    return juego.mozo.posicion.fil == juego.herramientas[indice_mopa].posicion.fil && juego.mozo.posicion.col == juego.herramientas[indice_mopa].posicion.col;
}

//PRE: uso exclusivo para mozo, tener inicializado el juego
//POST: verifica que el mozo tenga distinta posicion de cocina, monedas, patines y charcos
bool es_misma_posicion_mozo (juego_t juego){
    bool misma_posicion = true;
    if (juego.mozo.posicion.fil == juego.cocina.posicion.fil && juego.mozo.posicion.col == juego.cocina.posicion.col){
        misma_posicion = false;
    }

    int i = 0;
    while (i < juego.cantidad_herramientas && misma_posicion) {
        if (juego.mozo.posicion.fil == juego.herramientas[i].posicion.fil &&
            juego.mozo.posicion.col == juego.herramientas[i].posicion.col) {
            misma_posicion = false;
        }
        i++;
    }

    int j = 0;
    while (j < juego.cantidad_obstaculos && misma_posicion) {
        if (juego.mozo.posicion.fil == juego.obstaculos[j].posicion.fil &&
            juego.mozo.posicion.col == juego.obstaculos[j].posicion.col) {
            misma_posicion = false;
        }
        j++;
    }
    return misma_posicion;
}

//PRE: uso exclusivo para mozo, tener inicializado el juego, el mozo debe tener la mopa.
//POST: cambia el estado de tiene_mopa del mozo. Agrega la mopa en la ultima posicion del array de herramientas y rellena sus datos. 
void dejar_mopa (juego_t *juego){
    juego->mozo.tiene_mopa = false;
    juego->cantidad_herramientas++;
    juego->herramientas[(juego->cantidad_herramientas) + ELEMENTO_AGREGADO].posicion = juego->mozo.posicion;
    juego->herramientas[(juego->cantidad_herramientas) + ELEMENTO_AGREGADO].tipo = MOPA;
}

//PRE: uso exclusivo para mozo, tener inicializado el juego
//POST: cambia el estado de tiene_mopa del mozo. Elimina la mopa del array de herramientas. 
void agarrar_mopa (juego_t * juego){
    juego->mozo.tiene_mopa = true;
    int indice_mopa = buscar_mopa(juego->cantidad_herramientas,juego->herramientas);
    juego->herramientas[indice_mopa] = juego->herramientas[(juego->cantidad_herramientas) + ELEMENTO_ELIMINADO];
    juego->cantidad_herramientas--;
}

//PRE: uso exclusivo para mozo, tener inicializado el juego, la coordenada debe estar dentro de los limites
//POST: cambia de posicion al mozo y aumenta los movimientos.
void mover_mozo (coordenada_t* posicion_mozo, int* movimientos, coordenada_t nueva_posicion_mozo){
    *posicion_mozo = nueva_posicion_mozo;
    (*movimientos)++;
}

//PRE: posicion_mozo y posicion_mesa deben ser validos
//POST: devuelve true si la distancia manhattan es igual a 1, si el estado de pedido es false, si hay comensales y si la cantidad de pedidos es menor a la de MAX_pedidos.
//      devuelve false en caso contrario
bool es_posible_tomar_pedido (coordenada_t posicion_mozo, coordenada_t posicion_mesa,bool estado_pedido, int cantidad_comensales, int cantidad_pedidos){
    return (calcular_distancia_manhattan(posicion_mozo, posicion_mesa) == DISTANCIA_MOZO_MESAS 
                                && !estado_pedido 
                                && cantidad_comensales != SIN_COMENSALES
                                && cantidad_pedidos < MAX_PEDIDOS);
}

//PRE: mozo debe estar inicalizado
//POST: activa los patines y se consume uno
void consumir_patines (mozo_t* mozo){
    if (!mozo->tiene_mopa){
        mozo->patines_puestos = true;
        mozo->cantidad_patines--;
    }
}

//PRE: juego debe estar inicializado, posicion_mozo debe estar dentro de los limites
//POST: si tiene mopa solo puede limpiar charcos, caso contrario puede hacer cualquier accion
void realizar_acciones_mozo(juego_t* juego, coordenada_t posicion_mozo){
    if (juego->mozo.tiene_mopa){
        limpiar_charco(juego, posicion_mozo);
    }else{
        matar_cucharacha(juego, posicion_mozo);
        recolectar_moneda(juego, posicion_mozo);
        recolectar_patines(juego, posicion_mozo);
        caer_bandeja(juego, posicion_mozo);
        finalizar_pedido(&juego->mozo, &juego->dinero,juego->mesas, juego->cantidad_mesas, posicion_mozo);
        entregar_pedidos_cocina(&juego->mozo, &juego->cocina);
        transferir_platos_a_bandeja(&juego->mozo, &juego->cocina);
    }
}

//PRE: juego debe estar inicializado, nueva_posicion_mozo debe ser valido
//POST: si tiene patines y esta activado, entonces se mueve hast chocar alguna mesa o limite
void mover_con_patines (coordenada_t nueva_posicion_mozo, char accion, juego_t* juego){
    bool puede_moverse = true;
    while (puede_moverse) {
        coordenada_t siguiente_posicion = mover_mozo_matriz(nueva_posicion_mozo, accion);
        if (!es_limite_juego(siguiente_posicion) || !es_posicion_mesa(*juego, siguiente_posicion)) {
            puede_moverse = false;
        } else {
            nueva_posicion_mozo = siguiente_posicion;
            realizar_acciones_mozo(juego,nueva_posicion_mozo);
        }
    }
    mover_mozo(&juego->mozo.posicion, &juego->movimientos, nueva_posicion_mozo);
}

//PRE: juego debe estar inicializado
//POST: actualiza el juego 
void actualizar_juego (juego_t* juego, int movimiento_actual){
    actualizar_mesas(juego->movimientos, juego, movimiento_actual);
    if (juego->movimientos != movimiento_actual){
        disminuir_tiempo_pedido(juego->cocina.platos_preparacion,juego->cocina.cantidad_preparacion);
    }
    transferir_pedido_a_listo(&juego->cocina);
    generar_cucarachas(juego,juego->movimientos);
}

//PRE: juego debe estar inicializado
//POST: si la distancia manhattan es igual a 1, si el estado de pedido es false, si hay comensales y si la cantidad de pedidos es menor a la de MAX_pedidos,
//      entonces toma el pedido, caso contrario no lo toma.
void intentar_tomar_pedidos(juego_t* juego){
    for (int i = 0; i < juego->cantidad_mesas; i++){
        for (int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            if (es_posible_tomar_pedido(juego->mozo.posicion,juego->mesas[i].posicion[j],juego->mesas[i].pedido_tomado, 
                                        juego->mesas[i].cantidad_comensales, juego->mozo.cantidad_pedidos)){
                tomar_pedido(&juego->mesas[i], &juego->mozo,i);
            }
        }
    }
}

//PRE: juego debe estar inicializado
//POST: si el mozo y la mopa tienen la misma posicion, entonces si tiene mopa, la deja; si no tiene mopa, la agarra.
void interactuar_mopa_mozo(juego_t* juego){
    if (juego->mozo.tiene_mopa && es_misma_posicion_mozo(*juego)){
        dejar_mopa(juego);
    }else if (!juego->mozo.tiene_mopa && es_misma_posicion_mopa_mozo (*juego)){
        agarrar_mopa(juego);
    }
}

//PRE: juego debe estar inicializado, la accion debe ser valida (izquierda 'A', arriba 'W', abajo 'S', derecha 'D')
//POST: el mozo se mueve segun la accion y si tiene o no patines puestos.
void mover_mozo_juego(juego_t* juego, char accion){
    coordenada_t nueva_posicion_mozo = juego->mozo.posicion;
    if (juego->mozo.patines_puestos){
        mover_con_patines(nueva_posicion_mozo,accion,juego);
        juego->mozo.patines_puestos = false;
    }else{
        nueva_posicion_mozo = mover_mozo_matriz (juego->mozo.posicion, accion);
        if (es_limite_juego(nueva_posicion_mozo) && es_posicion_mesa (*juego, nueva_posicion_mozo)){
            mover_mozo(&juego->mozo.posicion, &juego->movimientos, nueva_posicion_mozo);
        }
    }
}

void realizar_jugada(juego_t *juego, char accion){
    int movimiento_actual = juego->movimientos;
    bool consumio_patines = juego->mozo.patines_puestos;
    if (es_accion_valida_mover(accion)){
        mover_mozo_juego(juego, accion);
    }else if(es_accion_valida(accion, MOPA)){
        interactuar_mopa_mozo(juego);
    }else if (es_accion_valida(accion, TOMAR_PEDIDO) && !juego->mozo.tiene_mopa){
        intentar_tomar_pedidos(juego);
    }else if (es_accion_valida(accion, PATINES) && juego->mozo.cantidad_patines != SIN_PATINES && !consumio_patines){
        consumir_patines(&juego->mozo);
    }
    actualizar_juego(juego, movimiento_actual);
    realizar_acciones_mozo(juego,juego->mozo.posicion);
}

//PRE: elemento_matriz debe ser valido (dentro de los maximos)
//POST: rellena la matriz con '.'
void inicializar_matriz_juego (char elemento_matriz[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
            elemento_matriz [i][j] = TERRENO_BASE;
        }
    }
}

//PRE: elemento_matriz debe ser valido (dentro de los maximos)
//POST: rellena la matriz con X por cada comensal de cada mesa
void rellenar_comensales(mesa_t mesa, int fila_mesa, int col_mesa, char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS],int comensales_rellenados){
    if (mesa.cantidad_comensales != SIN_COMENSALES && comensales_rellenados < mesa.cantidad_comensales){
        elemento_restaurante[fila_mesa][col_mesa] = COMENSAL;
    }
}


//PRE: el vector mesa debe estar inicializado,el elemento_restaurante y cantidad_mesas deben ser validos (dentro de los maximos)
//POST: asigna su representacion correspondiente (MESA (T)), según su coordenada
void rellenar_mesas (char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS], int cantidad_mesas, mesa_t* mesa){
    for (int i = 0; i < cantidad_mesas; i++){
        int comensales_rellenados = SIN_COMENSALES;
        for (int j = 0; j < mesa[i].cantidad_lugares; j++){
            int fila_mesa= mesa[i].posicion[j].fil;
            int columna_mesa = mesa[i].posicion[j].col;
            elemento_restaurante[fila_mesa][columna_mesa] = MESA;
            rellenar_comensales(mesa[i],fila_mesa,columna_mesa,elemento_restaurante,comensales_rellenados);
            comensales_rellenados++;
        }
    }
}

//PRE:  cocina_fil, elemento_restaurante y cocina_col deben ser validos (dentro de los maximos)
//POST: asigna su representacion correspondiente (COCINA(C)), según su coordenada
void rellenar_cocina (char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS], int cocina_fil, int cocina_col){
    elemento_restaurante [cocina_fil][cocina_col] = COCINA;
}

//PRE: el vector herramientas debe estar inicializado,el elemento_restaurante debe ser valido (dentro de los maximos)
//POST: asigna su representacion correspondiente (MOPA(O), MONEDAS(M), PATINES(P)), según su coordenada
void rellenar_herramientas (char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS], int cantidad_herramientas, objeto_t* herramientas){
    for (int i=0; i < cantidad_herramientas; i++){
        if (herramientas[i].tipo == MOPA){
            elemento_restaurante [herramientas[i].posicion.fil][herramientas[i].posicion.col] = MOPA;
        }else if (herramientas[i].tipo == MONEDAS){
            elemento_restaurante [herramientas[i].posicion.fil][herramientas[i].posicion.col] = MONEDAS;
        }else if (herramientas[i].tipo == PATINES){
            elemento_restaurante [herramientas[i].posicion.fil][herramientas[i].posicion.col] = PATINES;
        }   
    }
}

//PRE: el vector obstaculos debe estar inicializado, el elemento_restaurante debe ser valido (dentro de los maximos)
//POST: asigna su representacion correspondiente (CHARCOS(H)), según su coordenada
void rellenar_obstaculos (char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS], int cantidad_obstaculos, objeto_t* obstaculos){
    for (int i=0; i < cantidad_obstaculos; i++){
        if (obstaculos[i].tipo == CHARCOS){
            elemento_restaurante [obstaculos[i].posicion.fil][obstaculos[i].posicion.col] = CHARCOS;
        }else if (obstaculos[i].tipo == CUCARACHAS){
            elemento_restaurante [obstaculos[i].posicion.fil][obstaculos[i].posicion.col] = CUCARACHAS;
        }
    }
}

//PRE:  posicion_mozo_fil, elemento_restaurante y posicion_mozo_col deben ser validos (dentro de los maximos)
//POST: asigna su representacion correspondiente (MOZO(L)), según su coordenada
void rellenar_mozo (char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS], int posicion_mozo_fil,int posicion_mozo_col){
    elemento_restaurante [posicion_mozo_fil][posicion_mozo_col] = MOZO;
}

//PRE: El juego deberá estar inicializado previamente con `inicializar_juego `
//POST: relaciona la posicion de cada elemento del juego con su respectiva inicial
void rellenar_elementos (juego_t juego, char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS]){
    rellenar_mesas (elemento_restaurante, juego.cantidad_mesas, juego.mesas);
    rellenar_cocina(elemento_restaurante, juego.cocina.posicion.fil, juego.cocina.posicion.col);
    rellenar_herramientas(elemento_restaurante, juego.cantidad_herramientas, juego.herramientas);
    rellenar_obstaculos(elemento_restaurante, juego.cantidad_obstaculos,juego.obstaculos);
    rellenar_mozo(elemento_restaurante, juego.mozo.posicion.fil, juego.mozo.posicion.col);
}

void mostrar_juego(juego_t juego){
    char elemento_restaurante[MAX_FILAS][MAX_COLUMNAS];
    inicializar_matriz_juego (elemento_restaurante);
    rellenar_elementos (juego, elemento_restaurante);
    printf ("\n");
    printf("\t\t%s\n",ENTRADA_RESTAURANTE);
    for (int i = 0; i < MAX_FILAS; i++){
        printf ("\t\t");
        printf("%c", CARACTER_PARED);
        for (int j = 0; j < MAX_COLUMNAS; j++){
            printf("%c ",elemento_restaurante[i][j]);  
        }
        printf("%c", CARACTER_PARED);
        printf("\n");
    }
    printf("\t\t%s",PARED_POSTERIOR);
    printf("\n");
}

//PRE: valores inicializados para los movimientos realizados y el dinero
//POST: retorna true si los movimientos es igual al total y el dinero es mayor al minimo para ganar, false en caso contrario
bool es_estado_juego_ganado (int movimientos, int dinero){
    return movimientos == TOTAL_MOVIMIENTOS_MAX && dinero >= MIN_DINERO_PARA_GANAR;
}
//PRE: valores inicializados para los movimientos y el dinero
//POST: retorna true si los movimientos es igual al total y el dinero es menor al minimo para ganar, false en caso contrario
bool es_estado_juego_perdido (int movimientos, int dinero){
    return movimientos == TOTAL_MOVIMIENTOS_MAX && dinero < MIN_DINERO_PARA_GANAR;
}

int estado_juego(juego_t juego){
    int estado_juego = ESTADO_JUEGO_JUGANDO;
    if (es_estado_juego_ganado(juego.movimientos, juego.dinero)){
        estado_juego = ESTADO_JUEGO_GANADO;
    }else if (es_estado_juego_perdido (juego.movimientos, juego.dinero)){
        estado_juego = ESTADO_JUEGO_PERDIDO;
    }
    return estado_juego;
}

void destruir_juego(juego_t *juego){
    liberar_memoria_platos(&juego->cocina.platos_listos);
    liberar_memoria_platos(&juego->cocina.platos_preparacion);
}