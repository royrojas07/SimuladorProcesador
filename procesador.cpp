#include "procesador.h"
Controlador::Controlador()
{
    //cargar hilos
    //init estructuras
}

Controlador::~Controlador()
{
    
}

void Controlador::add(){

}

void Controlador::addi(int x1, int x2, int n)
{
    hilos[0].registros[x1] = hilos[0].registros[x2] + n; //?? Ese 0 se tendria que sustituir por un entero que apunte al hilo actual
}

void Controlador::sub(int x1, int x2, int x3)
{
    hilos[0].registros[x1] = hilos[0].registros[x2] - hilos[0].registros[x3];
}

void Controlador::mul( int x1, int x2, int x3 )
{
    hilos[0].registros[x1] = hilos[0].registros[x2] * hilos[0].registros[x3];
}

void Controlador::div(int x1, int x2, int x3)
{
    hilos[0].registros[x1] = hilos[0].registros[x2] / hilos[0].registros[x3];
}

void Controlador::lw(int x1, int x2, int n)
{
    int direccion = hilos[0].registros[x2] + n;
    hilos[0].registros[x1] = memoria.datos[direccion]; // x1 <- M[x2 + n]
}

void Controlador::sw()
{

}

void Controlador::beq()
{

}

void Controlador::bne(int x1, int x2, int n)
{
    if(hilos[0].registros[x1] != hilos[0].registros[x2])
        hilos[0].PC += n * 4; // PC <- n * 4
}

void Controlador::lr()
{

}

void Controlador::sc()
{

}

void Controlador::jal( int x1, int n )
{
    hilos[0].registros[x1] = hilos[0].PC; // x1 <- PC
    hilos[0].PC += n; // PC<-PC+n
}

void Controlador::jalr( int x1, int x2, int n )
{
    hilos[0].registros[x1] = hilos[0].PC; // x1 = PC
    hilos[0].PC = hilos[0].registros[x2] + n; // PC = x2+n
}

void Controlador::FIN()
{

}

void Controlador::aumentar_reloj() 
{

}

void Controlador::asociar(int codigo, int x, int y, int z) //Si no se ocupa un parametro para un metodo se pasa un cero y no se hace nada con el 
{
    switch (codigo)
    {
    case 19:
        addi(x,y,z);
        break;
    case 71:
        add(x,y,z);
        break;
    case 83:
        sub(x,y,z);
        break;
    case 72:
        mul(x,y,z);
        break;
    case 56:
        div(x,y,z);
        break;
    case 5:
        lw(x,y,z);
        break;
    case 37:
        sw(x,y,z);
        break;
    case 99:
        beq(x,y,z);
        break;
    case 100:
        bne(x,y,z);
        break;
    case 51:
        lr(x,y,z);
        break;
    case 52:
        sc(x,y,z);
        break;
    case 111:
        jal(x,z);
        break;
    case 103:
        jalr(x,y,z);
        break;
    case 999:
        FIN();
        break;
    default:
        break;
    }
}

void Controlador::buffer_victima()
{

}

void Controlador::cambio_contexto()
{

}

void Controlador::cargar_hilos()
{
    //lee archivos de texto dados por el usuario
    //conforme se leen se va cargando su contenido a la memoria de instrucciones y al arreglo de hilos.
    // Se pide el quantum
}

void Controlador::init_estructuras()
{
    // memoria de datos con un 1 en cada una de las palabras
    // caches con un 0 en cada una de las palabras y -1 en las direcciones y estados
    // Memoria de instrucciones con 0 
    // buffer victima con 0 y -1 en la identificacion de bloque
    //registro de cada hilo inicializados con 0
}

void Controlador::ejecutar_hilillo()
{
    //el hilo que pasa en ejecucion 
}
