#include "procesador.h"
#include <cstdlib>
#include <stdio.h>

int main(){
    Controlador controlador;
    controlador.init_hilos();
    controlador.fin_hilos();
    return 0;
};