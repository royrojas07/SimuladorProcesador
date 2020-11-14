#ifndef PROCESADOR_H
#define PROCESADOR_H
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <iostream>

struct Hilo
{
    int PC;
    int instruc_ptr; // para saber desde donde empezaban las intrucciones de un hilo
    int registros[32];
    int RL;
    int reloj_inicio; //valor del reloj cuando empezó ejecución el hilo
    int reloj_fin; //valor del reloj cuando terminó ejecución el hilo
    int tiempo_en_ejecucion;
};


struct Bloque // Roy 
{
    // 2 palabras
    // # bloque
    // estado
};

struct Memoria
{
    int datos[96];
    int instrucciones[640];
};

struct Cache
{
    Bloque datos[4];
    Bloque instrucciones[32]; //? Revisar si en verdad son 16 // Roy
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
    void cambio_contexto(); // No complicado  //Carlos
    void add(int x1, int x2, int x3); //fabian
    void addi(int x1, int x2, int n); //carlos
    void sub(int x1, int x2, int x3); //carlos
    void mul( int x1, int x2, int x3 ); //roy
    void div(int x1, int x2, int x3); //carlos 
    void lw(int x1, int x2, int n); //carlos
    void sw(int x1, int x2, int n); //fabian
    void beq(int x1, int x2, int n); //fabian
    void bne(int x1, int x2, int n); //carlos
    void lr(); //roy
    void sc(); //fabian
    void jal( int x1, int n ); //roy
    void jalr( int x1, int x2, int n );//roy
    void FIN(); // En este metodo se deberia restar la longitud del vector de hilos
    void cargar_hilos(); // este puede ser el metodo que lea de los txt y podria ir en el constructor de Controlador //Fabian
    void init_estructuras();// podria llamarse en el constructor igualmente //Carlos
    void init_hilos(); //crea los hilos y los manda a ejecutar sus metodos respectivos //Roy
    
    
    private:
    int reloj;
    int quantum;
    Bloque buffer[8];
    Memoria memoria;
    Cache cache;
    Round_robin vector_hilos;


};
#endif
