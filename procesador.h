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
    Controlador(); 
    ~Controlador();
    void aumentar_reloj(); 
    void asociar(int codigo, int x, int y, int z); 
    static void * hilo_buffer( Controlador * ptr );
    void buffer_victima(); 
    static void * hilo_hilillo( Controlador * ptr );
    void ejecutar_hilillo(); 
    static void * hilo_controlador( Controlador * ptr );
    void controlador(); 
    void cambio_contexto(); 
    void add(int x1, int x2, int x3); 
    void addi(int x1, int x2, int n); 
    void sub(int x1, int x2, int x3); 
    void mul( int x1, int x2, int x3 ); 
    void div(int x1, int x2, int x3);
    void lw(int x1, int x2, int n); 
    void sw(int x2, int x1, int n); 
    void beq(int x1, int x2, int n); 
    void bne(int x1, int x2, int n); 
    void lr( int x1, int x2 ); 
    void sc(int x2, int x1, int n); 
    void jal( int x1, int n ); 
    void jalr( int x1, int x2, int n );
    void FIN(); 
    void cargar_hilos(); 
    void init_estructuras();
    void init_hilos(); 
    void fin_hilos();
    void cargar( int direccion, int * palabra_retorno, char='D' ); 
    void escribir( int direccion, int palabra ); 
    void cargar_de_mem_principal( int num_bloque, int * bloque_retorno );
    int copiar_a_cache( Bloque * bloque, int retraso, int num_bloque_en_buffer=0 );
    void buffer_a_mem();
    int buscar_en_cache_datos( int num_bloque );
    int menos_recien_usado( int conjunto );
    void impresion_final();
    void reemplazo_bloq_modif(BloqueDatos * bloq_datos,int direc_reemplazo);

    private:
    int reloj;
    int quantum;
    int inst_ejecutadas;
    bool buffer_termino;
    bool hillilo_termino;
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