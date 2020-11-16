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
    void asociar(int codigo); // para asociar los codigos de operación
    void buffer_victima();
    void cambio_contexto();
    void add(); //fabian
    void addi(); //carlos
    void sub(); //carlos
    void mul( int x1, int x2, int x3 ); //roy
    void div(); //carlos 
    void lw(); //carlos
    void sw(); //fabian
    void beq(); //fabian
    void bne(); //carlos
    void lr( int x1, int x2 ); //roy
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
