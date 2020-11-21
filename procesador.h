#ifndef PROCESADOR_H
#define PROCESADOR_H
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

struct Bloque
{
    int bloque;
    char estado;
    virtual ~Bloque(){}
};

struct BloqueDatos : public Bloque
{
    int palabra[2];
    int ultimo_uso;
};

struct BloqueInstruc : public Bloque
{
    int palabra[8];
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
    std::vector<Hilo> hilos;
};

struct Buffer //lo trabajo como arreglo circular para ahorrar los corrimientos
{
    int longitud;
    int inicio; //puntero a la celda del array que es el inicio 
    int fin; //puntero donde se tiene que hacer el siguiente push 
    //bool solicitud_hilo; //me indica si un hilo quiere un bloque que esta en el buffer 
    BloqueDatos buffer[8];
    void insertar(BloqueDatos bloque)
    {
        bloque.estado = 'V'; //valido 
        buffer[fin++] = bloque;
        fin = fin % 8; //por propiedades de arreglo circular
        longitud += 1;
    }

    BloqueDatos sacar()
    {
        buffer[inicio].estado = 'E'; //significa que se esta escribiendo 
        BloqueDatos bloque = buffer[inicio++];
        inicio = inicio % 8; //por las propiedades de arreglo circular
        longitud -= 1;
        return bloque;
    }

    bool llena()
    {
        if(longitud == 8)
        {
            return true;
        }
        return false;
    }

    bool vacia()
    {
        if(longitud == 0)
        {
            return true;
        }
        return false;
    }

    int buscar(int num_bloque) //retorna la pos en la que se encuentra lo buscado, en caso de no encontrarlo retorna -1
    {
        for(int i = 0; i < 8; ++i)
        {
            if(buffer[i].bloque == num_bloque)
            {
                return i;
            }
        }
        return -1;
    }
};

class Controlador
{
    public:
    Controlador(); //Carlos
    ~Controlador(); //Fabian
    void aumentar_reloj(); //Aumentar reloj
    void asociar(int codigo, int x, int y, int z); // para asociar los codigos de operación
    static void hilo_buffer( Controlador * ptr );
    void buffer_victima(); //Metodo que corre el hilo del buffer // complicado...
    static void hilo_hilillo( Controlador * ptr );
    void ejecutar_hilillo(); //metodo que esta ejecutando el hilo actual //Complicado
    static void hilo_controlador( Controlador * ptr );
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
    void fin_hilos();
    void cargar( int direccion, int * palabra_retorno, char='D' ); // carga de memoria
    void cargar_de_mem_principal( int num_bloque, int * bloque_retorno );
    int copiar_a_cache( Bloque * bloque, int retraso );

    private:
    int reloj;
    int quantum;
    int inst_ejecutadas;
    Buffer buffer; //cambiar nombre a futuro
    Memoria memoria;
    Cache cache;
    Round_robin vector_hilos;
    std::thread hilos[3];
};
#endif
