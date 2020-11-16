#include "procesador.h"
Controlador::Controlador(){
    // inicialización de memoria de datos
    for(int i = 0; i < 96; ++i)
        memoria.datos[i] = 1;
    // inicialización de memoria de instrucciones
    for(int i = 0; i < 640; ++i)
        memoria.instrucciones[i] = 0;
}

Controlador::~Controlador(){
    
}

void Controlador::add(){

}

void Controlador::addi()
{

}

void Controlador::sub()
{

}

void Controlador::mul( int x1, int x2, int x3 )
{
    hilos[0].registros[x1] = hilos[0].registros[x2] * hilos[0].registros[x3];
    aumentar_reloj();
}

void Controlador::div()
{

}

void Controlador::lw()
{

}

void Controlador::sw()
{

}

void Controlador::beq()
{

}

void Controlador::bne()
{

}

void Controlador::lr( int x1, int x2 )
{
    int dir = hilos[0].registros[x2];
    //hilos[0].registros[x1] = mem( dir ); // x1 <- M[x2]
    hilos[0].RL = hilos[0].registros[x2]; // RL <- x2
    aumentar_reloj();
}

void Controlador::sc()
{

}

void Controlador::jal( int x1, int n )
{
    hilos[0].registros[x1] = hilos[0].PC; // x1 <- PC
    hilos[0].PC += n; // PC <- PC+n
    aumentar_reloj();
}

void Controlador::jalr( int x1, int x2, int n )
{
    hilos[0].registros[x1] = hilos[0].PC; // x1 = PC
    hilos[0].PC = hilos[0].registros[x2] + n; // PC = x2+n
    aumentar_reloj();
}

void Controlador::FIN()
{

}

void Controlador::aumentar_reloj()
{

}

void Controlador::asociar(int codigo)
{

}

void Controlador::buffer_victima()
{

}

void Controlador::cambio_contexto()
{

}
