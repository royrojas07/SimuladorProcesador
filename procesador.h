#ifndef PROCESADOR_H
#define PROCESADOR_H
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

//#include <pthread.h>

struct Hilo
{
    int PC;
    int instruc_ptr; // para saber desde donde empezaban las intrucciones de un hilo
    int registros[32];
    int RL;
    int IR[4];
    int reloj_inicio; //valor del reloj cuando empezó ejecución el hilo
    int reloj_fin; //valor del reloj cuando terminó ejecución el hilo
    int tiempo_en_ejecucion;
};

struct BloqueDatos
{
    int palabra[2];
    int bloque;
    char estado;
};

struct BloqueInstruc
{
    int palabra[8];
    int bloque;
    char estado; // ? int o char
};

struct Memoria
{
    int datos[96];
    int instrucciones[640];
};

struct Cache
{
    BloqueDatos datos[4];
    BloqueInstruc instrucciones[8];
};

struct Round_robin // Carlos
{
    int puntero_actual;
    int longitud;
    Hilo * hilos;
};

class Controlador
{
    public:
    Controlador(); //Carlos
    ~Controlador(); //Fabian
    void aumentar_reloj(); //Aumentar reloj
    void asociar(int codigo, int x, int y, int z); // para asociar los codigos de operación
    void buffer_victima(); //Metodo que corre el hilo del buffer // complicado...
    void ejecutar_hilillo(); //metodo que esta ejecutando el hilo actual //Complicado
    void controlador(); // metodo que realiza cambios de contexto, se encarga del manejo del reloj y quantum
    void cambio_contexto(); // No complicado  //Carlos
    void add(int x1, int x2, int x3); //fabian
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
    void jalr( int x1, int x2, int n );//roy
    void FIN(); // En este metodo se deberia restar la longitud del vector de hilos
    void cargar_hilos(); // este puede ser el metodo que lea de los txt y podria ir en el constructor de Controlador //Fabian
    void init_estructuras();// podria llamarse en el constructor igualmente //Carlos
    void init_hilos(); //crea los hilos y los manda a ejecutar sus metodos respectivos //Roy
    void fin_hilos(); // estaria bien asi?
    
    private:
    int reloj;
    int quantum;
    BloqueDatos buffer[8];
    Memoria memoria;
    Cache cache;
    Round_robin vector_hilos;
    std::thread hilos[3];
};
#endif
