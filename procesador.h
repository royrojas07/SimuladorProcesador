#ifndef PROCESADOR_H
#define PROCESADOR_H
#include "structs.h"
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <pthread.h>
#include <cmath>
#include <vector>
#include <string>

class Controlador
{
    public:
    Controlador(); //Carlos
    ~Controlador(); //Fabian
    void aumentar_reloj(); //Aumentar reloj
    void asociar(int codigo, int x, int y, int z); // para asociar los codigos de operación
    static void * hilo_buffer( Controlador * ptr );
    void buffer_victima(); //Metodo que corre el hilo del buffer // complicado...
    static void * hilo_hilillo( Controlador * ptr );
    void ejecutar_hilillo(); //metodo que esta ejecutando el hilo actual //Complicado
    static void * hilo_controlador( Controlador * ptr );
    void controlador(); // metodo que realiza cambios de contexto, se encarga del manejo del reloj y quantum
    void cambio_contexto(); // No complicado  //Carlos
    /*void add(int x1, int x2, int x3); //fabian
    void addi(int x1, int x2, int n); //carlos
    void sub(int x1, int x2, int x3); //carlos
    void mul( int x1, int x2, int x3 ); //roy
    void div(int x1, int x2, int x3); //carlos 
    void lw(int x1, int x2, int n); //carlos
    void sw(int x2, int x1, int n); //fabian
    void beq(int x1, int x2, int n); //fabian
    void bne(int x1, int x2, int n); //carlos
    void lr( int x1, int x2 ); //roy
    void sc(int x2, int x1, int n); //fabian
    void jal( int x1, int n ); //roy
    void jalr( int x1, int x2, int n );//roy*/
    void FIN(); // En este metodo se deberia restar la longitud del vector de hilos
    void cargar_hilos(); // este puede ser el metodo que lea de los txt y podria ir en el constructor de Controlador //Fabian
    void init_estructuras();// podria llamarse en el constructor igualmente //Carlos
    void init_hilos(); //crea los hilos y los manda a ejecutar sus metodos respectivos //Roy
    void fin_hilos();
    void cargar( int direccion, int * palabra_retorno, char='D' ); // carga de memoria
    void escribir( int direccion, int palabra ); // escribir a memoria
    void cargar_de_mem_principal( int num_bloque, int * bloque_retorno );
    int copiar_a_cache( Bloque * bloque, int retraso );
    void bloque_a_mem();
    int buscar_en_cache_datos( int num_bloque );

    private:
    int reloj;
    int quantum;
    int inst_ejecutadas;
    Buffer buffer_vic; //cambiar nombre a futuro
    Memoria memoria;
    Cache cache;
    Round_robin vector_hilos;
    std::thread hilos[3];
};
#endif
