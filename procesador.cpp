#include "procesador.h"
Controlador::Controlador(){
    
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

void Controlador::asociar(int codigo)
{

}

void Controlador::buffer_victima()
{

}

void Controlador::cambio_contexto()
{

}
