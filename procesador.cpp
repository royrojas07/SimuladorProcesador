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
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2]
            * vector_hilos.hilos[actual].registros[x3];
    //aumentar_reloj();
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
    int actual = vector_hilos.puntero_actual;
    int dir = vector_hilos.hilos[actual].registros[x2];
    //hilos[0].registros[x1] = mem( dir ); // x1 <- M[x2]
    vector_hilos.hilos[actual].RL = vector_hilos.hilos[actual].registros[x2]; // RL <- x2
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
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].PC; // x1 <- PC
    vector_hilos.hilos[actual].PC += n; // PC <- PC+n
    //aumentar_reloj();
}

void Controlador::jalr( int x1, int x2, int n )
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].PC; // x1 = PC
    vector_hilos.hilos[actual].PC = vector_hilos.hilos[actual].registros[x2] + n; // PC = x2+n
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
    vector_hilos.puntero_actual = (vector_hilos.puntero_actual+1) % vector_hilos.longitud;
    inst_ejecutadas = 0;
}

void Controlador::cargar_hilos()
{
    //pedir el quantum, ver si cambiar lo de pedir por una ventanilla 
    std::string input = "";
    int quantum = 0;
    std::cout << "De cuantos ciclos de reloj va a ser el quantum? Escriba un número." << std::endl;
    while(true){
        getline(std::cin,input);
        std::stringstream stream(input);
        if(stream >> quantum)
            break;
        else
            std::cout << "Número inválido, por favor trate de nuevo." <<std::endl;
    }

    std::cout << "El quantum va a ser de " << quantum << " ciclos de reloj." << std::endl;

    
    //std::string nombre_archivo; /*arreglo de nombres de archivos? preguntar como manejarlo, parametros?*/
    
    //pedir la cantidad de hilillos que se van a inicializar, ver si cambiar lo de pedir por una ventanilla 
    int num_hilillos = 0;
    int num_valido = 0;
    std::cout << "Cuantos hilillos va a inicializar? Escriba un número." << std::endl;
    while(!num_valido){
        getline(std::cin,input);
        std::stringstream stream(input);
        if(stream >> num_hilillos)
            num_valido = 1;
        else
            std::cout << "Número inválido, por favor trate de nuevo." << std::endl;
    }
    
    std::cout << "Se van a inicializar " << num_hilillos << " hilillos.";
    for(int i = 0; i < num_hilillos; i++){
        int archivo_valido = 0;
        while(!archivo_valido){
            std::cout << "Escriba la ruta del archivo en donde se encuentran las instrucciones del hilillo número " << i + 1 << "." << std::endl;
            std::getline(std::cin,input);
            std::ifstream test(input);
            if(!test){
                std::cout << "Ese archivo no existe, trate de nuevo" << std::endl;
            }
            std::cout << "Ruta de archivo válida" << std::endl;
            test.close();
        }
        char* linea_instruccion;
        std::ifstream archivo_leido(input);
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
        cache.datos[i].num_bloque = -1;
        cache.datos[i].estado = 'I';
        cache.datos[i].ultimo_uso = -1;
    }
    //Falta el init de cache de instrucciones con 0
    for(i = 0; i < 8; ++i)
    {
        for(j = 0; j < 8; ++j)
        {
            cache.instrucciones[i].palabra[j] = 0;
        }
        cache.instrucciones[i].num_bloque = -1;
        cache.instrucciones[i].estado = 'I';
    }
    for(i = 0; i < 8; ++i)
    {
        //init del buffer
        buffer[i].palabra[0] = 0;
        buffer[i].palabra[1] = 0;
        buffer[i].num_bloque = -1;
        buffer[i].estado = 'L'; // L de libre o disponible
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
    // mientras queden hilos por ejecutar
    while( vector_hilos.hilos.size() != 0 )
    {
        // se treabaja con el hilo al que se apunta
        Hilo actual = vector_hilos.hilos[vector_hilos.puntero_actual];
        // se carga al IR la instrucción que apunta el PC
        mem_get( actual.PC, actual.IR, 'I' );
        // se ejecuta la instrucción
        asociar( actual.IR[0], actual.IR[1], actual.IR[2], actual.IR[3] );
        actual.PC += 4;
        // se guardan los cambios realizados al hilillo
        vector_hilos.hilos[vector_hilos.puntero_actual] = actual;
        // se aumenta el reloj
        //aumentar_reloj();
        // ? aca tendria que haber sincronizacion con hilo controlador
        // para seguir con la siguiente inst. o siguiente hilo
    }
}

void mem_get( int direccion, int * palabra_retorno, char memoria='D' )
{
    // se obtienen bloque y palabra a los que pertenece la dir. de memoria
    int num_bloque = floor(direccion/8);
    int num_palabra = floor(direccion/4);
    if( memoria == 'I' ) // cache de instrucciones
    {
        // mapeo directo
        if( cache.instrucciones[num_bloque%8].bloque != num_bloque ) // fallo de lectura
        {
            // se busca en la memoria principal
            BloqueInstruc bloque_instr = mem_principal( num_bloque ); // creo que no es necesario el num_palabra
            // se reemplaza en cache de instrucciones
            // cache.instrucciones[num_bloque%8] = bloque_instr;
            copiar_a_cache( num_bloque, bloque_instr ); // aqui se durarian los 24 ciclos
        }
        // si el numero de palabra es par entonces corresponde a
        // la primer palabra de su bloque, si no, es la segunda.
        int palabra_pos = (num_palabra % 2) ? 4:0;
        // se retorna la palabra
        for( int i = 0; i < 4; ++i )
            palabra_retorno[i] = cache.instrucciones[num_bloque%8].palabra[palabra_pos+i];
    }
    else // cache de datos
    {
        // asociativa por conjuntos de 2 vias
        // num_bloque % cantidad de conjuntos
        int bloque_cache = buscar_en_cache_datos( num_bloque );
        if( bloque_cache == -1 ) // fallo de lectura de cache de datos
        {
            // buscar en el buffer victima
            // aqui se duraria la espera por si el bloque esta siendo copiado a memoria
            int bloque_buffer = buscar_en_buffer( num_bloque );
            if( bloque_buffer != -1 )
            {
                // se realiza la copia
                // 4 ciclos de copiar de buffer a cache (OJO con los estados de los bloques)
                bloque_cache = copiar_a_cache( buffer[bloque_buffer] );
            }
            else // el bloque no estaba en el buffer
            {
                BloqueDatos bloque_datos = mem_principal( num_bloque );
                bloque_cache = copiar_a_cache( bloque_datos ); // aqui se durarian los 24 ciclos
            }
        }
        // acierto de lectura
        // si el numero de palabra es par entonces corresponde a
        // la primer palabra de su bloque, si no, es la segunda
        int palabra_pos = (num_palabra % 2) ? 1:0;
        // se retorna la palabra
        *palabra_retorno = cache.datos[bloque_cache].palabra[palabra_pos];
    }
}

void Controlador::init_hilos()
{
    hilos[0] = std::thread( hilo_controlador, this );
    hilos[1] = std::thread( hilo_hilillo, this );
    hilos[2] = std::thread( hilo_buffer, this );
}

void Controlador::fin_hilos()
{
    hilos[0].join();
    hilos[1].join();
    hilos[2].join();
}

static void hilo_buffer( Controlador * ptr )
{
    ptr->buffer_victima();
}

static void hilo_hilillo( Controlador * ptr )
{
    ptr->ejecutar_hilillo();
}

static void hilo_controlador( Controlador * ptr )
{
    ptr->controlador();
}