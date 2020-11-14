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

struct Memoria
{
    int datos[96];
    int instrucciones[640];
};

struct Cache
{
    int datos[8];
    int instrucciones[64];
};

class Controlador
{
    public:
    Controlador();
    ~Controlador();
    void aumentar_reloj();
    void asociar(int codigo, int x, int y, int z); // para asociar los codigos de operación
    void buffer_victima();
    void cambio_contexto();
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
    void FIN();
    void cargar_hilillos();
    
    private:
    int reloj;
    int quantum;
    Hilo * hilos;
    int buffer[16];
    Memoria memoria;

};
#endif
