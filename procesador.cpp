#include "procesador.h"
Controlador::Controlador()
{
    cargar_hilos();
    init_estructuras();
    reloj = 0;
}

Controlador::~Controlador()
{
    
}

void Controlador::add(int x1, int x2, int x3){
    hilos[0].registros[x1] = hilos[0].registros[x2] + hilos[0].registros[x3];
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
    aumentar_reloj();
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

void Controlador::sw(int x2, int x1, int n)
{
    int direccion = hilos[0].registros[x2] + n;
    memoria.datos[direccion] = hilos[0].registros[x1];
}

void Controlador::beq(int x1, int x2, int n)
{
    if(hilos[0].registros[x1] == hilos[0].registros[x2])
        hilos[0].PC += n*4;
}

void Controlador::bne(int x1, int x2, int n)
{
    if(hilos[0].registros[x1] != hilos[0].registros[x2])
        hilos[0].PC += n * 4; // PC <- n * 4
}

void Controlador::lr( int x1, int x2 )
{
    int dir = hilos[0].registros[x2];
    //hilos[0].registros[x1] = mem( dir ); // x1 <- M[x2]
    hilos[0].RL = hilos[0].registros[x2]; // RL <- x2
    //aumentar_reloj();
}

void Controlador::sc(int x2, int x1, int n)
{
    if(hilos[0].RL == n+x2){
        memoria.datos[n + x2] = hilos[0].registros[x1];
    }else{
        hilos[0].registros[x1]=0;
    }
}

void Controlador::jal( int x1, int n )
{
    hilos[0].registros[x1] = hilos[0].PC; // x1 <- PC
    hilos[0].PC += n; // PC <- PC+n
    //aumentar_reloj();
}

void Controlador::jalr( int x1, int x2, int n )
{
    hilos[0].registros[x1] = hilos[0].PC; // x1 = PC
    hilos[0].PC = hilos[0].registros[x2] + n; // PC = x2+n
    //aumentar_reloj();
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
    vector_hilos.puntero_actual = vector_hilos.puntero_actual % vector_hilos.longitud;
    reloj = 0;
}

void Controlador::cargar_hilos()
{
    std::string nombre_archivo; /*arreglo de nombres de archivos? preguntar como manejarlo, parametros?*/ 
    //pedir el quantum 
    //pedir el nombre de los archivos e ir preguntando si hay otro hilillo hasta que no haya otro hilillo(?)
    char* linea_instruccion;
    std::ifstream archivo_leido(nombre_archivo);
    int puntero_memoria_instrucciones = 0;
    //registrar el numero del puntero de memoria de instrucciones en el que empiezan las instrucciones por hilillo ****
    while (std::getline (archivo_leido, std::string(linea_instruccion))) {
        std::cout << linea_instruccion; // para pruebas
        char* parte_instruccion= strtok(linea_instruccion, " ");
        while(parte_instruccion != nullptr){
            int parte_instruccion_int = stoi(std::string(parte_instruccion));
            memoria.instrucciones[puntero_memoria_instrucciones] = parte_instruccion_int;
            parte_instruccion = strtok(nullptr, " "); //si no sirve probar con NULL
            puntero_memoria_instrucciones++;
        }
    }
    //lee archivos de texto dados por el usuario
    //conforme se leen se va cargando su contenido a la memoria de instrucciones y al arreglo de hilos.
    // Se pide el quantum
}

void Controlador::init_estructuras()
{
    int i, j;
    for(i = 0; i < 96; ++i) 
        memoria.datos[i] = 1; //Init de memoria de datos

    for(i = 0; i < 640; ++i) 
        memoria.instrucciones[i] = 0; //init de memoria de instrucciones

    for(i = 0; i < 4; ++i)
    {
        //Init de cache de datos
        cache.datos[i].palabra[0] = 0;
        cache.datos[i].palabra[1] = 0;
        cache.datos[i].bloque = -1;
        cache.datos[i].estado = -1;
    }
    //Falta el init de cache de instrucciones con 0
    for(i = 0; i < 8; ++i)
    {
        for(j = 0; j < 8; ++j)
        {
            cache.instrucciones[i].palabra[j] = 0;
        }
        cache.instrucciones[i].bloque = -1;
        cache.instrucciones[i].estado = -1;
    }
    for(i = 0; i < 8; ++i)
    {
        //init del buffer
        buffer[i].palabra[0] = 0;
        buffer[i].palabra[1] = 0;
        buffer[i].bloque = -1;
    }
    for(i = 0; i < vector_hilos.longitud; ++i)
    {
        for(int j = 0; j < 32; ++j)
            //Init de registros de cada hilo
            vector_hilos.hilos[i].registros[j] = 0;
    }
}

void Controlador::ejecutar_hilillo()
{
    //el hilo que pasa en ejecucion 
}

void Controlador::init_hilos()
{
    hilos[0] = std::thread( controlador );
    hilos[1] = std::thread( ejecutar_hilillo );
    hilos[2] = std::thread( buffer_victima );
}

void Controlador::fin_hilos()
{
    hilos[0].join();
    hilos[1].join();
    hilos[2].join();
}