#include "procesador.h"
Controlador::Controlador()
{
    init_estructuras();
    cargar_hilos();
    sem_init( &senal_hilo_a_buffer, 0, 0 );
    sem_init( &senal_ejecutar_a_controlador, 0, 0 );
    reloj = 0;
    inst_ejecutadas = 0;
}

Controlador::~Controlador()
{
    
}

void Controlador::add(int x1, int x2, int x3)
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2] + vector_hilos.hilos[actual].registros[x3];
}

void Controlador::addi(int x1, int x2, int n)
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2] + n; //?? Ese 0 se tendria que sustituir por un entero que apunte al hilo actual
}

void Controlador::sub(int x1, int x2, int x3)
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2] - vector_hilos.hilos[actual].registros[x3];
}

void Controlador::mul( int x1, int x2, int x3 )
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2]
            * vector_hilos.hilos[actual].registros[x3];
}

void Controlador::div(int x1, int x2, int x3)
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2] / vector_hilos.hilos[actual].registros[x3];
}

void Controlador::lw(int x1, int x2, int n)
{
    int actual = vector_hilos.puntero_actual;
    int direccion = vector_hilos.hilos[actual].registros[x2] + n;
    int* palabra;
    cargar(direccion,palabra);
    vector_hilos.hilos[actual].registros[x1] = *palabra; // x1 <- M[x2 + n]
}

void Controlador::sw(int x2, int x1, int n)
{
    int actual = vector_hilos.puntero_actual;
    int direccion = vector_hilos.hilos[actual].registros[x2] + n;
    escribir( direccion, vector_hilos.hilos[actual].registros[x1] );
}

void Controlador::beq(int x1, int x2, int n)
{
    int actual = vector_hilos.puntero_actual;
    if(vector_hilos.hilos[actual].registros[x1] == vector_hilos.hilos[actual].registros[x2])
        vector_hilos.hilos[actual].PC += n*4;
}

void Controlador::bne(int x1, int x2, int n)
{
    int actual = vector_hilos.puntero_actual;
    if(vector_hilos.hilos[actual].registros[x1] != vector_hilos.hilos[actual].registros[x2])
        vector_hilos.hilos[actual].PC += n * 4; // PC <- n * 4
}

void Controlador::lr( int x1, int x2 )
{
    int actual = vector_hilos.puntero_actual;
    int dir = vector_hilos.hilos[actual].registros[x2];
    int palabra;
    cargar( dir, &palabra );
    vector_hilos.hilos[actual].registros[x1] = palabra; // x1 <- M[x2]
    vector_hilos.hilos[actual].RL = vector_hilos.hilos[actual].registros[x2]; // RL <- x2
}

void Controlador::sc(int x2, int x1, int n)
{
    int actual = vector_hilos.puntero_actual;
    if(vector_hilos.hilos[actual].RL == n+vector_hilos.hilos[actual].registros[x2]){
        escribir( n+vector_hilos.hilos[actual].registros[x2], vector_hilos.hilos[actual].registros[x1] );
        //memoria.datos[n + x2] = vector_hilos.hilos[actual].registros[x1];
    }else{
        vector_hilos.hilos[actual].registros[x1]=0;
    }
}

void Controlador::jal( int x1, int n )
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].PC; // x1 <- PC
    vector_hilos.hilos[actual].PC += n; // PC <- PC+n
}

void Controlador::jalr( int x1, int x2, int n )
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].PC; // x1 = PC
    vector_hilos.hilos[actual].PC = vector_hilos.hilos[actual].registros[x2] + n; // PC = x2+n
}

void Controlador::FIN()
{
    fin_de_hilillo = true;
}

void Controlador::aumentar_reloj() 
{
    pthread_barrier_wait(&barrera);
    ++reloj;
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
        lr(x,y);
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
    int longitud_buffer;
    while(true)
    {
        if( !sem_trywait( &senal_hilo_a_buffer ) )
        {
            while(!buffer_vic.vacia())
            {
                buffer_a_mem();
            }
        }
        else
            pthread_barrier_wait( &barrera );
    }
}

void Controlador::buffer_a_mem()
{
    BloqueDatos victima = buffer_vic.sacar();
    int direccion;
    int retrasos = 0;
    while(retrasos < 24)
    {
        pthread_barrier_wait(&barrera);
    }
    direccion = victima.bloque * 2;
    memoria.datos[direccion] = victima.palabra[0];
    memoria.datos[direccion + 1] = victima.palabra[1];
    buffer_vic.buffer[victima.bloque].estado = LIBRE;
    buffer_vic.buffer[victima.bloque].bloque = -1;
    pthread_mutex_unlock( &buffer_vic.candado[victima.bloque] );
}
    

void Controlador::cambio_contexto()
{
    vector_hilos.puntero_actual = (vector_hilos.puntero_actual+1) % vector_hilos.hilos.size();
    inst_ejecutadas = 0;
}

void Controlador::cargar_hilos()
{
    //pedir el quantum
    std::string input = ""; //variable que va a guardar el input del usuario
    int quantum = 0;
    std::cout << "De cuantos ciclos de reloj va a ser el quantum? Escriba un número." << std::endl;
    while(true)
    {
        getline(std::cin,input);
        std::stringstream stream(input);
        if(stream >> quantum) //si el input es un numero valido
            break;
        else
            std::cout << "Número inválido, por favor trate de nuevo." <<std::endl;
    }
    this->quantum = quantum;
    std::cout << "El quantum va a ser de " << quantum << " ciclos de reloj." << std::endl; //print para confirmar el quantum
    
    //pedir la cantidad de hilillos que se van a inicializar
    int num_hilillos = 0;
    std::cout << "Cuantos hilillos va a inicializar? Escriba un número." << std::endl; 
    while(true)
    {
        getline(std::cin,input);
        std::stringstream stream(input);
        if(stream >> num_hilillos) // si el input es un numero valido
        {
            Hilo h[num_hilillos];
            for( int i = 0; i < num_hilillos; ++i )
                vector_hilos.hilos.push_back( h[i] );
            break;
        }
        else
            std::cout << "Número inválido, por favor trate de nuevo." << std::endl; 
    }
    
    std::cout << "Se van a inicializar " << num_hilillos << " hilillos." << std::endl; //print para confirmar la cantidad de hilillos
    int puntero_memoria_instrucciones = 0;
    for(int i = 0; i < num_hilillos; i++)
    {
        int archivo_valido = 0;
        while(!archivo_valido)
        {
            std::cout << "Escriba la ruta del archivo en donde se encuentran las instrucciones del hilillo número " << i + 1 << "." << std::endl; //pide la ruta del archivo al usuario
            std::getline(std::cin,input); //para que el usuario digite la ruta
            std::ifstream test(input); //se trata de abrir un archivo con esa ruta
            if(!test) //si la ruta no abre ningun archivo
            {
                std::cout << "Ese archivo no existe, trate de nuevo" << std::endl; //le dice al usuario que no hay ningun archivo con esa ruta
            }
            else//si la ruta abre un archivo
            {
                std::cout << "Ruta de archivo válida" << std::endl; //se avisa que es una ruta valida
                archivo_valido = 1; //se pone esta variable en 1 para salir del ciclo
                test.close(); //se cierra el archivo que se usó para probar
            }
        }
        std::string linea_instruccion; //variable donde se va a guardar una linea de instruccion
        std::ifstream archivo_leido(input); // se abre el archivo que el usuario especificó anteriormente
        Hilo hilo_actual; //se crea el hilo 
        //ver si hay que inicializar más los hilos o si hay que usar hilos que ya existen
        vector_hilos.hilos.push_back(hilo_actual);
        vector_hilos.hilos[i].PC = puntero_memoria_instrucciones+384; //registra el numero del puntero de memoria de instrucciones en el que empiezan las instrucciones del hilillo ****
        while (std::getline (archivo_leido, linea_instruccion))  //mientras que haya lineas que leer en el archivo
        {
            char *cstr = new char[linea_instruccion.length() + 1]; //se crea una variable cstr para guardar el string linea_instruccion convertido a char*
            strcpy(cstr,linea_instruccion.c_str()); //se copia a esa variable porque el metodo linea_instruccio.c_str devuelve un char * estático
            char* parte_instruccion= strtok(cstr, " "); //se separa la instrucción por espacios blancos
            while(parte_instruccion != NULL){ //mientras que haya partes de instruccion que separar
                int parte_instruccion_int = atoi(parte_instruccion); //se convierte esa parte de instruccion a int
                memoria.instrucciones[puntero_memoria_instrucciones] = parte_instruccion_int;// se guarda en la memoria de instrucciones
                parte_instruccion = strtok(NULL, " "); // se sigue separando la instruccion en partes de instrucción
                puntero_memoria_instrucciones++; //se aumenta el puntero de memoria de instrucciones para guardar el siguiente pedazo de instrucción
            }
            delete cstr; //se borra la variable inicializada anteriormente
        }
    }
    // init de registros de hilillos
    for( int i = 0; i < num_hilillos; ++i )
    {
        for( int j = 0; j < 32; ++j )
            vector_hilos.hilos[i].registros[j] = 0;
    }
    vector_hilos.puntero_actual = 0;
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
        cache.datos[i].estado = INVALIDO;
        cache.datos[i].ultimo_uso = -1;
    }
    //Falta el init de cache de instrucciones con 0
    for(i = 0; i < 8; ++i)
    {
        for(j = 0; j < 8; ++j)
        {
            cache.instrucciones[i].palabra[j] = 0;
        }
        cache.instrucciones[i].bloque = -1;
        cache.instrucciones[i].estado = INVALIDO;
    }
    //init del buffer
    buffer_vic.inicializar( &barrera );
    for(i = 0; i < 8; ++i)
    {
        buffer_vic.buffer[i].palabra[0] = 0;
        buffer_vic.buffer[i].palabra[1] = 0;
        buffer_vic.buffer[i].bloque = -1;
        buffer_vic.buffer[i].estado = LIBRE; 
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
        std::cout << actual.PC << " Pc"<<std::endl;
        cargar( actual.PC, actual.IR, 'I' );
        puts("IR:");
        for( int i = 0; i < 4; ++i )
            std::cout << actual.IR[i] << " ";
        std::cout << std::endl;
        // se apunta a la siguiente direccion
        actual.PC += 4;
        // se ejecuta la instrucción
        asociar( actual.IR[0], actual.IR[1], actual.IR[2], actual.IR[3] );
        // se guardan los cambios realizados al hilillo
        vector_hilos.hilos[vector_hilos.puntero_actual] = actual;
        // se aumenta contador de instrucciones ejecutadas por este hilillo
        inst_ejecutadas++;
        pthread_barrier_wait(&barrera);
        // aca tendria que haber sincronizacion con hilo controlador (semaforo)
        // para seguir con la siguiente inst. o siguiente hilo
        sem_wait( &senal_ejecutar_a_controlador );
    }
    puts("LLEGUE :D");
    int i, j;
    for(i = 0; i < 96; ++i) 
        std::cout << memoria.datos[i] << " "; //Init de memoria de datos
    std::cout << std::endl;
    for(i = 0; i < 640; ++i) 
        std::cout << memoria.instrucciones[i] << " ";
    std::cout << std::endl;
}

void Controlador::cargar( int direccion, int * palabra_retorno, char memoria )
{
    // se obtienen bloque y palabra a los que pertenece la dir. de memoria
    int num_bloque = floor(direccion/8);
    int num_palabra = floor(direccion/4);
    std::cout << num_bloque << " " << num_palabra << std::endl;
    if( memoria == 'I' ) // cache de instrucciones
    {
        puts("cargar");
        // mapeo directo
        if( cache.instrucciones[num_bloque%8].bloque != num_bloque ) // fallo de lectura
        {
            // se busca en la memoria principal
            BloqueInstruc bloque_instr;
            cargar_de_mem_principal( num_bloque, bloque_instr.palabra );
            bloque_instr.bloque = num_bloque;
            bloque_instr.estado = COMPARTIDO; // estado compartido
            // se copia en cache de instrucciones
            copiar_a_cache( &bloque_instr, 24 ); // aqui se duran los 24 ciclos
        }
        // acierto de lectura o ya se subio el bloque
        // si el numero de palabra es par entonces corresponde a
        // la primer palabra de su bloque, si no, es la segunda.
        int palabra_pos = (num_palabra % 2) ? 4:0;
        // se retorna la palabra
        for( int i = 0; i < 4; ++i )
            palabra_retorno[i] = cache.instrucciones[num_bloque%8].palabra[palabra_pos+i];
    }
    else // cache de datos
    {
        int bloque_cache = buscar_en_cache_datos( num_bloque );
        if( bloque_cache == -1 ) // fallo de lectura de cache de datos
        {
            // buscar en el buffer victima
            // aqui se duraria la espera por si el bloque esta siendo copiado a memoria
            int bloque_buffer = buffer_vic.buscar( num_bloque );
            if( bloque_buffer != -1 )
            {
                // se realiza la copia
                // 4 ciclos de copiar de buffer a cache (OJO con los estados de los bloques)
                buffer_vic.buffer[bloque_buffer].estado = SUBIENDO;
                bloque_cache = copiar_a_cache( &buffer_vic.buffer[bloque_buffer], 4 ); // ? aqui no hay problemas
            }
            else // el bloque no estaba en el buffer
            {
                BloqueDatos bloque_datos;
                cargar_de_mem_principal( num_bloque, bloque_datos.palabra );
                bloque_datos.bloque = num_bloque;
                bloque_datos.estado = COMPARTIDO; // estado compartido
                bloque_cache = copiar_a_cache( &bloque_datos, 24 ); // aqui se durarian los 24 ciclos
            }
        }
        // acierto de lectura o ya se subio el bloque a cache
        // si el numero de palabra es par entonces corresponde a
        // la primer palabra de su bloque, si no, es la segunda
        int palabra_pos = (num_palabra % 2) ? 1:0;
        // se retorna la palabra
        *palabra_retorno = cache.datos[bloque_cache].palabra[palabra_pos];
    }
}

void Controlador::cargar_de_mem_principal( int num_bloque, int * bloque_retorno )
{
    if( num_bloque >= 48 ) // mem. de intrucciones
    {
        int bloque_instr = num_bloque - 48;
        for( int i = 0; i < 8; ++i )
            bloque_retorno[i] = memoria.instrucciones[(bloque_instr*8)+i];
    }
    else // mem. de datos
    {
        bloque_retorno[0] = memoria.datos[(num_bloque*2)];
        bloque_retorno[1] = memoria.datos[(num_bloque*2)+1];
    }
}

int Controlador::buscar_en_cache_datos( int num_bloque )
{
    // asociativa por conjuntos de 2 vias
    // num_bloque % cantidad_de_conjuntos
    int conjunto = num_bloque % 2;
    if( cache.datos[conjunto*2].bloque == num_bloque )
    {
        return conjunto*2;
    }
    else if( cache.datos[(conjunto*2)+1].bloque == num_bloque )
    {
        return (conjunto*2)+1;
    }
    return -1;
}

int Controlador::copiar_a_cache( Bloque * bloque, int retraso ) // devuelve en bloque en cache donde hizo la copia
{
    int bloque_cache;
    int counter = 0;
    int num_bloque = bloque->bloque;
    int conjunto = num_bloque % 2;
    int direc_reemplazo;
    int direc_reemplazo_buff;
    bool insertado = false;
    BloqueDatos * bloq_datos = dynamic_cast< BloqueDatos * >( bloque );
    // asociativa o LRU?
    // 4 ciclos de copiar de buffer a cache (OJO con los estados de los bloques)
    if( bloq_datos != NULL ) // se copia desde buffer victima
    {
        // seria LRU porque con ultimo_uso = -1 ya se sabe si el bloque esta en invalido
        // lru()
        direc_reemplazo = menos_recien_usado(); 

        while(counter < retraso)
        {
            pthread_barrier_wait(&barrera);
            ++counter;
        }
        
        if(cache.datos[direc_reemplazo].estado == INVALIDO || cache.datos[direc_reemplazo].estado == COMPARTIDO)
        {
            cache.datos[direc_reemplazo] = *bloq_datos; //se puede hacer esto?
                                                    //La otra seria hacer dynamic cast y igualar cada una de la palabras                                        
        }
        else // estado del bloque en cache MODIFICADO
        {
            direc_reemplazo_buff = buffer_vic.buscar(cache.datos[direc_reemplazo].bloque);
            if(direc_reemplazo_buff == -1)
            {
                while(!insertado)
                {
                    if(!buffer_vic.llena())
                    {
                        insertado = true;
                        if(buffer_vic.vacia()) //si esta vacio le tengo que avisar al hilo del buffer que ahora hay algo
                        {
                            sem_post( &senal_hilo_a_buffer );
                        }
                        counter = 0;
                        while(counter < 4)
                            pthread_barrier_wait(&barrera);
                        buffer_vic.insertar(cache.datos[direc_reemplazo]);
                        cache.datos[direc_reemplazo] = *bloq_datos; //modif de la cache
                    }
                    else
                    {
                        pthread_barrier_wait(&barrera); //para sincronizacion tiene que esperar hasta que el buffer deje de estar lleno
                    }
                }
            }
            else
            {
                while(counter < 4)
                    pthread_barrier_wait(&barrera);
                buffer_vic.buffer[direc_reemplazo_buff] = cache.datos[direc_reemplazo]; //MERGING
                cache.datos[direc_reemplazo] = *bloq_datos; //modif de la cache
            }
        }
        //Fijarme en los estados de la cache para ver si tengo que hacer merging o solo meter al buffer
        // :-)
    }
    else// se copia a cache de instrucciones
    {
        BloqueInstruc * bloq_instr = dynamic_cast< BloqueInstruc * >( bloque );
            // falta el retraso !!!!!
        while(counter < retraso)
        {
            pthread_barrier_wait(&barrera);
            ++counter;
        }
            // reemplazo de mapeo directo
        cache.instrucciones[num_bloque%8] = *bloq_instr;
    }
    if(retraso == 4)
    //Si ocurrio un retraso de 4 es que se estaba copiando de buffer, por lo que se coloco estado de ESCRIBIENDO en el buffer
    //de esta manera devuelvo el bloque del buffer a un estado valido para que cuando siga en la cola se copie a memoria
        bloq_datos->estado = VALIDO; 
    return direc_reemplazo;
}

void Controlador::escribir( int direccion, int palabra )
{
    puts("escribo");
    // se obtienen bloque y palabra a los que pertenece la dir. de memoria
    int num_bloque = floor(direccion/8);
    int num_palabra = floor(direccion/4);
    int bloque_cache = buscar_en_cache_datos( num_bloque );
    if( bloque_cache == -1 ) // fallo de escritura en cache
    {
        puts("fallo de cache");
        // buscar en el buffer victima
        // aqui se duraria la espera por si el bloque esta siendo copiado a memoria
        int bloque_buffer = buffer_vic.buscar( num_bloque );
        if( bloque_buffer != -1 )
        {
            puts("se sube de buffer");
            // se realiza la copia
            // 4 ciclos de copiar de buffer a cache (OJO con los estados de los bloques)
            bloque_cache = copiar_a_cache( &buffer_vic.buffer[bloque_buffer], 4 ); // ? aqui no hay problemas
        }
        else // el bloque no estaba en el buffer
        {
            puts("se sube de memoria prin.");
            BloqueDatos bloque_datos;
            cargar_de_mem_principal( num_bloque, bloque_datos.palabra );
            bloque_datos.bloque = num_bloque;
            bloque_datos.estado = COMPARTIDO; // estado compartido
            bloque_cache = copiar_a_cache( &bloque_datos, 24 ); // aqui se durarian los 24 ciclos
        }
    }
    // hit de escritura o ya se subio el bloque a cache
    // si el numero de palabra es par entonces corresponde a
    // la primer palabra de su bloque, si no, es la segunda
    int palabra_pos = (num_palabra % 2) ? 1:0;
    cache.datos[bloque_cache].palabra[palabra_pos] = palabra;
}

void Controlador::controlador()
{
    int inst_ejecutadas_ant = 0;
    bool cambio_de_contexto = false;
    while(true){
        aumentar_reloj();
        if(inst_ejecutadas == quantum || fin_de_hilillo )
        {
            puts("hola cambio");
            if(fin_de_hilillo){
                vector_hilos.hilos.erase(vector_hilos.hilos.begin()+(vector_hilos.puntero_actual-1));
                fin_de_hilillo = false;
            }
            cambio_contexto();
            cambio_de_contexto = true;
            inst_ejecutadas_ant = 0;
        }
        if( inst_ejecutadas_ant < inst_ejecutadas || cambio_de_contexto )
        {
            sem_post( &senal_ejecutar_a_controlador );
            inst_ejecutadas_ant++;
            cambio_de_contexto = false;
        }
    }
}

void Controlador::init_hilos()
{
    pthread_barrier_init(&barrera, NULL, 3);
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

void *Controlador::hilo_buffer( Controlador * ptr )
{
    ptr->buffer_victima();
    return 0;
}

void *Controlador::hilo_hilillo( Controlador * ptr )
{
    ptr->ejecutar_hilillo();
    return 0;
}

void *Controlador::hilo_controlador( Controlador * ptr )
{
    ptr->controlador();
    return 0;
}

int Controlador::menos_recien_usado() 
{
    int menor = INT_MAX; //numero alto como inicializacion
    int direccion; 
    for(int i = 0; i < 4; ++i)
    {
        if(cache.datos[i].ultimo_uso < menor)
        {
            menor = cache.datos[i].ultimo_uso;
            direccion = i;
        }
    }
    return direccion;
}