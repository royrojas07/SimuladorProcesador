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
#include <climits>
#include <semaphore.h>
#include <mutex>

class Controlador
{
    public:
    Controlador(); //Carlos
    ~Controlador(); //Fabian
    void aumentar_reloj(); //Aumentar reloj
    void asociar(int codigo, int x, int y, int z); // para asociar los codigos de operación
    static void * hilo_buffer( Controlador * ptr );
    void buffer_victima(); //Metodo que corre el hilo del buffer
    static void * hilo_hilillo( Controlador * ptr );
    void ejecutar_hilillo(); //metodo que esta ejecutando el hilo actual
    static void * hilo_controlador( Controlador * ptr );
    void controlador(); // metodo que realiza cambios de contexto, se encarga del manejo del reloj y quantum
    void cambio_contexto(); // No complicado  //Carlos
    void add(int x1, int x2, int x3); //fabian
    void addi(int x1, int x2, int n); //carlos
    void sub(int x1, int x2, int x3); //carlos
    void mul( int x1, int x2, int x3 );
    void div(int x1, int x2, int x3); //carlos 
    void lw(int x1, int x2, int n); //carlos
    void sw(int x2, int x1, int n); //fabian
    void beq(int x1, int x2, int n); //fabian
    void bne(int x1, int x2, int n); //carlos
    void lr( int x1, int x2 );
    void sc(int x2, int x1, int n); //fabian
    void jal( int x1, int n );
    void jalr( int x1, int x2, int n );
    void FIN(); // En este metodo se deberia restar la longitud del vector de hilos
    void cargar_hilos(); // este puede ser el metodo que lea de los txt y podria ir en el constructor de Controlador //Fabian
    void init_estructuras();// podria llamarse en el constructor igualmente //Carlos
    void init_hilos(); //crea los hilos
    void fin_hilos();
    void cargar( int direccion, int * palabra_retorno, char='D' ); // carga de memoria
    void escribir( int direccion, int palabra ); // escribir a memoria
    void cargar_de_mem_principal( int num_bloque, int * bloque_retorno );
    int copiar_a_cache( Bloque * bloque, int retraso, int num_bloque_en_buffer=0 );
    void buffer_a_mem();
    int buscar_en_cache_datos( int num_bloque );
    int menos_recien_usado( int conjunto );

    private:
    int reloj;
    int quantum;
    int inst_ejecutadas;
    // bandera para indicar que terminaron las inst. de un hilillo
    bool fin_de_hilillo;
    // para indicar que se da cambio de reloj debido a que terminó una instr.
    bool se_ejecuto_ins;
    Buffer buffer_vic;
    Memoria memoria;
    Cache cache;
    // cola round robin de hilillos en ejecución
    Round_robin vector_hilos;
    std::thread hilos[NUM_THREADS];
    pthread_barrier_t  barrera; 
    sem_t senal_hilo_a_buffer;
    sem_t senal_ejecutar_a_controlador;
    // vector de hilillos que finalizaron
    std::vector<Hilo> historial_hilos;
};
#endif