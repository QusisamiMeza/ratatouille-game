#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "restaurant.h"

#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define MOPA 'O'
#define PATINES 'P'
#define TOMAR_PEDIDO 'T'
#define MOZO 'L'
#define MESA 'T'
#define COCINA 'C'
#define CHARCOS 'H'
#define MONEDAS 'M'
#define PATINES 'P'
#define CUCARACHAS 'U'
#define COMENSAL 'X'
#define NEGRITA "\033[1m"
#define NORMAL "\033[0m"

#define MIN_DINERO_PARA_GANAR 150000
#define TOTAL_MOVIMIENTOS_MAX  200

#define ESTADO_JUEGO_GANADO 1
#define ESTADO_JUEGO_PERDIDO -1
#define ESTADO_JUEGO_JUGANDO 0

#define SIN_PEDIDOS 0
#define SIN_PLATOS 0

const char CARACTER_BIENVENIDA = 'S';
const char CARACTER_INSTRUCCIONES = 'C';

//PRE: -
//POST: verifica si la accion es valida (CARACTER_BIENVENIDA('S'))
bool accion_valida_bienvenida (char accion){
    return accion == CARACTER_BIENVENIDA;
}

//PRE: -
//POST: imprime por pantalla la bienvenida al juego, para continuar pide CARACTER_BIENVENIDA (S)
void dar_bienvenida (){
    char accion;
    printf("\
                                                                                      ████████ ███                   \n \
                                                                                    ██          █                   \n \
                                                                                  ████           ██                 \n \
                                                                                  █               █                 \n \
     ___  _                            _     _                                 ██                 █                \n \
    | _ )(_) ___  _ _ __ __ ___  _ _  (_) __| | ___  ___                        █                  █                \n \
    | _ \\| |/ -_)| ' \\\\ V // -_)| ' \\ | |/ _` |/ _ \\(_-<                        ██                 █                \n \
    |___/|_|\\___||_||_|\\_/ \\___||_||_||_|\\__,_|\\___//__/                         █                  █               \n \
        _                 _                                _                      █                 █ █████         \n \
  __ _ | |  _ _  ___  ___| |_  __ _  _  _  _ _  __ _  _ _ | |_  ___               ██                  █  ██         \n \
 / _` || | | '_|/ -_)(_-<|  _|/ _` || || || '_|/ _` || ' \\|  _|/ -_)              ████                █ ██          \n \
 \\__,_||_| |_|  \\___|/__/ \\__|\\__,_| \\_,_||_|  \\__,_||_||_|\\__|\\___|            ██    ████           █████          \n \
          _        ___                                                         █         ██  ███        ███         \n \
       __| | ___  | _ \\ ___  _ __  _  _                                        █          ██             ██         \n \
      / _` |/ -_) |   // -_)| '  \\| || |                                        █         ██              █         \n \
      \\__,_|\\___| |_|_\\\\___||_|_|_|\\_, |                                         ██  █                    █         \n \
                                   |__/                                            ███                     █        \n \
                                                                                       ██      █████        ██      \n \
                                                                                        ██     █████         ██     \n \
                                                                                        ███                 ███     \n \
                              𝙿𝚛𝚎𝚜𝚜 '%c' 𝚝𝚘 𝚜𝚝𝚊𝚛𝚝                                        ██         ██████████       \n \
                                                                                      ██            ██████          \n \
                                                                                    ███        ████████ ███         \n \
                                                                                  ██                 ███  ██        \n \
                                                                                 ███          ███    ███   ██       \n \
                                                                                █████         ██ ██   ███  ███      \n \
                                                                       █        █████             ████  █████████   \n \
                                                                       ████     ███                █████  ██ █ ███  \n \
                                                                      ████████████████                ███ ██   ███  \n \
                                                                          ████████████████████████████████  ███    \n", CARACTER_BIENVENIDA);
    
    scanf(" %c", &accion);
    while (!accion_valida_bienvenida(accion)){
        printf("𝙿𝚛𝚎𝚜𝚜 '%c' 𝚝𝚘 𝚜𝚝𝚊𝚛𝚝\n", CARACTER_BIENVENIDA);
        scanf(" %c", &accion);
    }                                                                      
}

//PRE: -
//POST: imprime por pantalla el como se juega de manera resumida.
void mostrar_contexto_juego(){
    printf("\
     █▄░█ █▀▀ █▀▀ █▀▀ █▀ █ ▀█▀ ▄▀█ █▀▄▀█ █▀█ █▀   █░█ █▄░█   █▀▀ ▄▀█ █▀▄▀█ ▄▀█ █▀█ █▀▀ █▀█ █▀█\n \
    █░▀█ ██▄ █▄▄ ██▄ ▄█ █ ░█░ █▀█ █░▀░█ █▄█ ▄█   █▄█ █░▀█   █▄▄ █▀█ █░▀░█ █▀█ █▀▄ ██▄ █▀▄ █▄█\n ");
    printf("\n");
    printf("\
            Con la ayuda de Linguini trabaja en el restaurante de Remy atendiendo comensales.\n \
           En el camino encontraras monedas y patines  que te ayudaran a tener un dia exitoso,\n \
            pero ¡cuidado! las cucarachas y los charcos podran hacerte el dia mas complicado.\n");
    printf("\n");
}

//PRE: -
//POST: imprime por pantalla los controles disponibles del juego
void mostrar_controles (){
    printf(NEGRITA"\t\t⸻   > CONTROLES:\n");
    printf(NORMAL"\
            Para mover a Linguini(L) use:           \n \
            - %c para mover hacia arriba            \n \
            - %c para mover hacia abajo             \n \
            - %c para mover hacia derecha           \n \
            - %c para mover hacia izquierda         \n \
            - Para agarrar la mopa(O) use %c        \n \
            - Para activar los patines use %c       \n \
            - Para tomar pedidos use %c\n", ARRIBA,ABAJO,DERECHA,IZQUIERDA,MOPA,PATINES,TOMAR_PEDIDO);
    printf("\n");
}

//PRE: -
//POST: imprime por pantalla las instrucciones de como ganar el juego.
void mostrar_como_ganar () {
    printf(NEGRITA"\t\t⸻   > ¿COMO GANAR EL JUEGO?\n");
    printf(NORMAL"\
            Antes de que lleguen los comensales, puedes ir limpiando los charcos o agarrando \n \
                                         monedas y patines.\n \
         Cuando lleguen los comensales, atiendelos antes que se les pase la paciencia. Primero\n \
            toma su pedido, despues lleva el pedido a la cocina y cuando el pedido este listo\n \
                                lleva los platos a su respectiva mesa.\n \
            En el proceso agarra monedas para ganar mas dinero y ayudate con los patines para \n \
            moverte mas rapido; sin embargo cuidate de no pisar algun charco con platos en la \n \
            bandeja porque perderas los platos y tambien los comensales de esos. Tambien mata \n \
                    a las cucarachas que veas, estas pueden espantar a los comensales. \n \
                Al finalizar el dia, debe tener un minimo de %i de dinero para ganar.\n \
                \n \
                                    "NEGRITA"₍ᐢ. , .ᐢ₎๋⠀ ﹙ ¡Buena suerte!⠀♡᳢⠀─── ۫ ✧⠀\n", MIN_DINERO_PARA_GANAR);
    printf(NORMAL"\n");

}

//PRE: 
//POST: verifica si la accion es valida (CARACTER_INSTRUCCIONES('C'))
bool accion_valida_instrucciones (char accion){
    return accion == CARACTER_INSTRUCCIONES;
}

//PRE: -
//POST: imprime por pantalla las intrucciones del juego:
//       -Flujo de juego, controles, como se gana.
//       Para continuar pide CARACTER_INSTRUCCIONES (C)
void dar_instrucciones(){
    system ("clear"); 
    char accion;
    mostrar_contexto_juego();
    mostrar_controles();
    mostrar_como_ganar();
    do{
        printf("Ingrese '%c' para continuar\n",CARACTER_INSTRUCCIONES);
        scanf(" %c", &accion);
    }while (!accion_valida_instrucciones(accion));
}

//PRE: -
//POST: verifica si la accion es valida (ARRIBA(W), ABAJO(S), DERECHA(A), IZQUIERDA(D),MOPA, TOMAR_PEDIDO,PATINES)
bool accion_valida (char accion){
    return accion == ARRIBA ||
            accion == ABAJO ||
            accion == DERECHA ||
            accion == IZQUIERDA ||
            accion == MOPA ||
            accion == TOMAR_PEDIDO ||
            accion == PATINES||
            accion == CARACTER_INSTRUCCIONES;
}

//PRE: tener el juego inicializado
//POST: pregunta al usuario que accion desea realizar hasta que sea valida
void preguntar_accion(char* accion, int num_movimiento_actual, int dinero_actual){
    printf("\n");
    printf("Ingrese la accion que desea hacer:\n");
    scanf(" %c", accion);
    while (!accion_valida(*accion)){
        printf("INGRESA UNA ACCION VALIDA.\n");
        scanf(" %c",accion);
    }
}

//PRE:-
//POST: imprime los valores equivalentes de cada elemento
void imprimir_leyenda (){
    printf("\n");
    printf("\tLinguini: %c | Mesas: %c | Cocina: %c | Charcos: %c | Cucarachas: %c\n", MOZO, MESA, COCINA,CHARCOS,CUCARACHAS);
    printf("\t    Mopa: %c | Monedas: %c | Patines: %c | Comensales: %c\n", MOPA, MONEDAS,PATINES,COMENSAL);
    printf("\n");
}

//PRE: juego debe estar inicializado
//POST: imprime comentarios de ayuda
void mostrar_comentarios (juego_t juego){
    printf("\n");
    printf("\t\t\t\tʚ¡ Ojito !ɞ\n");
    if (juego.mozo.tiene_mopa){
        printf(NEGRITA"\t\t¡Tienes mopa! Solo puedes limpiar charcos.\n");
    }else{
        printf(NEGRITA"\t\tSi quieres limpiar charcos, ve a buscar la mopa!\n");
    }
    if (juego.mozo.patines_puestos){
        printf("\t\t¡Patines puestos! Cuidado, solo puedes moverte una vez.\n");
    }
    if (juego.mozo.cantidad_pedidos != SIN_PEDIDOS){
        printf("\t\tTienes pedidos pendientes, ve a dejarlos a la cocina.\n");
        for (int i = 0; i < juego.mozo.cantidad_pedidos; i++){
            printf("\t\t%iº pedido es de la mesa: %i\n", i,juego.mozo.pedidos[i].id_mesa);
        }
    }
    if (juego.cocina.cantidad_preparacion > SIN_PLATOS){
        printf("\t\tLos platos se están preparando. . .\n");
        for (int i = 0; i < juego.cocina.cantidad_preparacion; i++){
            printf("\t\t%iº plato en preparacion es de la mesa: %i, tiempo %i\n", i,juego.cocina.platos_preparacion[i].id_mesa, juego.cocina.platos_preparacion[i].tiempo_preparacion);
        }
    }
    if (juego.cocina.cantidad_listos > SIN_PLATOS){
        printf("\t\tTienes platos listos, recogelos\n");
        for (int i = 0; i < juego.cocina.cantidad_listos; i++){
            printf("\t\t%iº plato listo es de la mesa: %i\n", i,juego.cocina.platos_listos[i].id_mesa);
        }
    }
    if (juego.mozo.cantidad_bandeja > SIN_PLATOS){
        printf("\t\tTienes platos en la bandeja, ve a dejarlos a su mesa.\n");
        for (int i = 0; i < juego.mozo.cantidad_bandeja; i++){
            printf("\t\t%iº plato en bandeja es de la mesa: %i\n", i,juego.mozo.bandeja[i].id_mesa);
        }
    }
    printf(NORMAL"\t\tPara ver las instrucciones de nuevo usa %c\n", CARACTER_INSTRUCCIONES);
    printf(NORMAL"\n");
}

//PRE:
//POST: imprime "felicidades, remy pudo abrir otro dia mas"
void imprimir_jugada_ganada(){
    system ("clear"); 
    printf("\
 ▗▄▄▄▖▗▄▄▄▖▗▖   ▗▄▄▄▖ ▗▄▄▖▗▄▄▄▖▗▄▄▄  ▗▄▖ ▗▄▄▄ ▗▄▄▄▖ ▗▄▄▖    ▗▄▄▖ ▗▄▄▄▖▗▖  ▗▖▗▖  ▗▖                              \n \
▐▌   ▐▌   ▐▌     █  ▐▌     █  ▐▌  █▐▌ ▐▌▐▌  █▐▌   ▐▌       ▐▌ ▐▌▐▌   ▐▛▚▞▜▌ ▝▚▞▘                               \n \
▐▛▀▀▘▐▛▀▀▘▐▌     █  ▐▌     █  ▐▌  █▐▛▀▜▌▐▌  █▐▛▀▀▘ ▝▀▚▖    ▐▛▀▚▖▐▛▀▀▘▐▌  ▐▌  ▐▌                                \n \
▐▌   ▐▙▄▄▖▐▙▄▄▖▗▄█▄▖▝▚▄▄▖▗▄█▄▖▐▙▄▄▀▐▌ ▐▌▐▙▄▄▀▐▙▄▄▖▗▄▄▞▘    ▐▌ ▐▌▐▙▄▄▖▐▌  ▐▌  ▐▌                                \n \
                                                                                                               \n \
                                                                                                               \n \
                                                                                                               \n \
▗▄▄▖ ▗▖ ▗▖▗▄▄▄  ▗▄▖      ▗▄▖ ▗▄▄▖ ▗▄▄▖ ▗▄▄▄▖▗▄▄▖      ▗▄▖▗▄▄▄▖▗▄▄▖  ▗▄▖     ▗▄▄▄ ▗▄▄▄▖ ▗▄▖     ▗▖  ▗▖ ▗▄▖  ▗▄▄▖\n \
▐▌ ▐▌▐▌ ▐▌▐▌  █▐▌ ▐▌    ▐▌ ▐▌▐▌ ▐▌▐▌ ▐▌  █  ▐▌ ▐▌    ▐▌ ▐▌ █  ▐▌ ▐▌▐▌ ▐▌    ▐▌  █  █  ▐▌ ▐▌    ▐▛▚▞▜▌▐▌ ▐▌▐▌   \n \
▐▛▀▘ ▐▌ ▐▌▐▌  █▐▌ ▐▌    ▐▛▀▜▌▐▛▀▚▖▐▛▀▚▖  █  ▐▛▀▚▖    ▐▌ ▐▌ █  ▐▛▀▚▖▐▌ ▐▌    ▐▌  █  █  ▐▛▀▜▌    ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖\n \
▐▌   ▝▚▄▞▘▐▙▄▄▀▝▚▄▞▘    ▐▌ ▐▌▐▙▄▞▘▐▌ ▐▌▗▄█▄▖▐▌ ▐▌    ▝▚▄▞▘ █  ▐▌ ▐▌▝▚▄▞▘    ▐▙▄▄▀▗▄█▄▖▐▌ ▐▌    ▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘\n \
                                                                                                               \n \
                                                                                                               \n \
                                                                                                               \n");
}

//PRE:
//POST: imprime "estas despedido buuu"
void imprimir_jugada_perdida(){
    system ("clear"); 
    printf("\
▓█████   ██████ ▄▄▄█████▓ ▄▄▄        ██████    ▓█████▄ ▓█████   ██████  ██▓███  ▓█████ ▓█████▄  ██▓▓█████▄  ▒█████  \n \
▓█   ▀ ▒██    ▒ ▓  ██▒ ▓▒▒████▄    ▒██    ▒    ▒██▀ ██▌▓█   ▀ ▒██    ▒ ▓██░  ██▒▓█   ▀ ▒██▀ ██▌▓██▒▒██▀ ██▌▒██▒  ██▒\n \
▒███   ░ ▓██▄   ▒ ▓██░ ▒░▒██  ▀█▄  ░ ▓██▄      ░██   █▌▒███   ░ ▓██▄   ▓██░ ██▓▒▒███   ░██   █▌▒██▒░██   █▌▒██░  ██▒\n \
▒▓█  ▄   ▒   ██▒░ ▓██▓ ░ ░██▄▄▄▄██   ▒   ██▒   ░▓█▄   ▌▒▓█  ▄   ▒   ██▒▒██▄█▓▒ ▒▒▓█  ▄ ░▓█▄   ▌░██░░▓█▄   ▌▒██   ██░\n \
░▒████▒▒██████▒▒  ▒██▒ ░  ▓█   ▓██▒▒██████▒▒   ░▒████▓ ░▒████▒▒██████▒▒▒██▒ ░  ░░▒████▒░▒████▓ ░██░░▒████▓ ░ ████▓▒░\n \
░░ ▒░ ░▒ ▒▓▒ ▒ ░  ▒ ░░    ▒▒   ▓▒█░▒ ▒▓▒ ▒ ░    ▒▒▓  ▒ ░░ ▒░ ░▒ ▒▓▒ ▒ ░▒▓▒░ ░  ░░░ ▒░ ░ ▒▒▓  ▒ ░▓   ▒▒▓  ▒ ░ ▒░▒░▒░ \n \
 ░ ░  ░░ ░▒  ░ ░    ░      ▒   ▒▒ ░░ ░▒  ░ ░    ░ ▒  ▒  ░ ░  ░░ ░▒  ░ ░░▒ ░      ░ ░  ░ ░ ▒  ▒  ▒ ░ ░ ▒  ▒   ░ ▒ ▒░ \n \
   ░   ░  ░  ░    ░        ░   ▒   ░  ░  ░      ░ ░  ░    ░   ░  ░  ░  ░░          ░    ░ ░  ░  ▒ ░ ░ ░  ░ ░ ░ ░ ▒  \n \
   ░  ░      ░                 ░  ░      ░        ░       ░  ░      ░              ░  ░   ░     ░     ░        ░ ░  \n \
                                                ░                                       ░           ░               \n \
 ▄▄▄▄    █    ██  █    ██  █    ██  █    ██                                                                         \n \
▓█████▄  ██  ▓██▒ ██  ▓██▒ ██  ▓██▒ ██  ▓██▒                                                                        \n \
▒██▒ ▄██▓██  ▒██░▓██  ▒██░▓██  ▒██░▓██  ▒██░                                                                        \n \
▒██░█▀  ▓▓█  ░██░▓▓█  ░██░▓▓█  ░██░▓▓█  ░██░                                                                        \n \
░▓█  ▀█▓▒▒█████▓ ▒▒█████▓ ▒▒█████▓ ▒▒█████▓                                                                         \n \
░▒▓███▀▒░▒▓▒ ▒ ▒ ░▒▓▒ ▒ ▒ ░▒▓▒ ▒ ▒ ░▒▓▒ ▒ ▒                                                                         \n \
▒░▒   ░ ░░▒░ ░ ░ ░░▒░ ░ ░ ░░▒░ ░ ░ ░░▒░ ░ ░                                                                         \n \
 ░    ░  ░░░ ░ ░  ░░░ ░ ░  ░░░ ░ ░  ░░░ ░ ░                                                                         \n \
 ░         ░        ░        ░        ░                                                                             \n \
      ░                                                                                                             \n");
}

//PRE: juego debe estar inicializado
//POST: imprime los informacion (movimiento, patines, dinero) hasta el momento
void dar_informacion_actual(juego_t juego){
    printf("\
                 "NEGRITA"Movimiento     |   Patines   |    Dinero  \n \
                "NORMAL"%i de 200           %i              %i\n", juego.movimientos,juego.mozo.cantidad_patines,juego.dinero);
}

//PRE: juego debe estar inicializado
//POST: realiza y muestra la jugada
void gestionar_turno(juego_t* juego){
    system ("clear");
    imprimir_leyenda ();
    mostrar_juego(*juego);
    char accion;
    mostrar_comentarios(*juego);
    dar_informacion_actual(*juego);
    preguntar_accion(&accion, juego->movimientos, juego->dinero);
    if(accion == CARACTER_INSTRUCCIONES){
        dar_instrucciones();
    }else{
        realizar_jugada(juego, accion);
    }
}

int main (){
    srand ((unsigned)time(NULL));
    system ("clear");    
    dar_bienvenida();
    juego_t juego;
    inicializar_juego(&juego);
    system ("clear"); 
    dar_instrucciones();
    do{
        gestionar_turno(&juego);
    }while (estado_juego(juego) == ESTADO_JUEGO_JUGANDO);
    if (estado_juego(juego) == ESTADO_JUEGO_GANADO){
        imprimir_jugada_ganada();
        destruir_juego(&juego);
    }else if (estado_juego(juego) == ESTADO_JUEGO_PERDIDO){
        imprimir_jugada_perdida();
        destruir_juego(&juego);
    }
    return 0;
}
