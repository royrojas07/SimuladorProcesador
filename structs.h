#include <vector>
#include <pthread.h>
#define INVALIDO 'I'
#define LIBRE 'L'
#define ESCRIBIENDO 'E'
#define COMPARTIDO 'C'
#define VALIDO 'V'
#define MODIFICADO 'M'
#define SUBIENDO 'S'
#define MERGING 'W'
#define NUM_THREADS 3

struct Hilo
{
    int PC;
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
    std::vector<Hilo> hilos;
};

struct Buffer //lo trabajo como arreglo circular para ahorrar los corrimientos
{
    int longitud;
    int inicio; //puntero a la celda del array que es el inicio 
    int fin; //puntero donde se tiene que hacer el siguiente push 
    //bool solicitud_hilo; //me indica si un hilo quiere un bloque que esta en el buffer 
    BloqueDatos buffer[8];
    pthread_mutex_t candado[8];
    pthread_barrier_t * barrera;
    
    void inicializar( pthread_barrier_t * barrera )
    {
        this->barrera = barrera;
        for( int i = 0; i < 8; ++i )
            pthread_mutex_init( &candado[i], NULL );
        longitud = 0;
    }

    void insertar(BloqueDatos bloque)
    {
        bloque.estado = VALIDO; //valido 
        buffer[fin++] = bloque;
        fin = fin % 8; //por propiedades de arreglo circular
        longitud += 1;
    }

    BloqueDatos sacar()
    {
        pthread_mutex_lock( &candado[inicio] );
        while(buffer[inicio].estado == SUBIENDO || buffer[inicio].estado == MERGING) //Magiver, comentarselo a los brooos
        {
            pthread_barrier_wait(barrera);
        }
        buffer[inicio].estado = ESCRIBIENDO; //significa que se esta escribiendo 
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
                while( true )
                {
                    if( pthread_mutex_trylock( &candado[i] ) == 0 ) // se logra tomar el candado
                    {
                        if( buffer[i].estado == LIBRE )
                            return -1;
                        else if( buffer[i].estado == VALIDO )
                            return i;
                    }
                    else // no se logra tomar el candado
                    {
                        // se espera en sincronizacion con el buffer victima
                        pthread_barrier_wait( barrera );
                    }
                }
            }
        }
        return -1;
    }
};
