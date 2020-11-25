#include "procesador.h"
Controlador::Controlador()
{
    init_estructuras();
    cargar_hilos();
    sem_init( &senal_hilo_a_buffer, 0, 0 );
    sem_init( &senal_ejecutar_a_controlador, 0, 0 );
    reloj = 0;
    inst_ejecutadas = 0;
    se_ejecuto_ins = false;
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
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2] + n; // x1 <- x2 + n
}

void Controlador::sub(int x1, int x2, int x3)
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2] 
        - vector_hilos.hilos[actual].registros[x3]; // x1 <- x2 - x3
}

void Controlador::mul( int x1, int x2, int x3 )
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2]
            * vector_hilos.hilos[actual].registros[x3]; // x1 <- x2 * x3
}

void Controlador::div(int x1, int x2, int x3)
{
    int actual = vector_hilos.puntero_actual;
    vector_hilos.hilos[actual].registros[x1] = vector_hilos.hilos[actual].registros[x2] 
        / vector_hilos.hilos[actual].registros[x3]; // x1 <- x2 / x3
}

void Controlador::lw(int x1, int x2, int n)
{
    int actual = vector_hilos.puntero_actual;
    int direccion = vector_hilos.hilos[actual].registros[x2] + n;
    int palabra;
    cargar(direccion, &palabra);
    vector_hilos.hilos[actual].registros[x1] = palabra; // x1 <- M[x2 + n]
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
    // se activa bandera avisando que se terminó el hilillo
    fin_de_hilillo = true;
}

/*  EFECTO: Espera a los hilos y cuando los otros 2 
            lleguen a la barrera tambien se aumenta el reloj.*/
void Controlador::aumentar_reloj() 
{
    pthread_barrier_wait(&barrera);
    ++reloj;
    vector_hilos.hilos[vector_hilos.puntero_actual].tiempo_en_ejecucion++; 
}

/*  EFECTO: asocia lo leido en el IR con la instruccion que deberia.
    RECIBE: (codigo) Codifo de operacion del IR.
            (x) Primer parametro del IR.
            (y) Segundo parametro del IR.
            (z) Tercer parametro del IR.*/
void Controlador::asociar(int codigo, int x, int y, int z) 
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

/*  EFECTO: Metodo que estara ejecutando el metodo 
            del buffer victima. */
void Controlador::buffer_victima()
{
    buffer_termino = false;
    while( vector_hilos.hilos.size() != 0 ) //Si no hay hilillos se termino el programa
    {
        if( !sem_trywait( &senal_hilo_a_buffer ) ) //hasta que le pongan algo en el buffer comienza a trabajar "dormido"
        {
            while(!buffer_vic.vacia()) //siga mientras el buffer no este vacio
            {
                buffer_a_mem();
            }
        }
        else
            pthread_barrier_wait( &barrera ); //igualmente a pesar de estar dormido pasa por la barrera por sincronizacion
    }
    buffer_termino = true;
    if(!hillilo_termino)
        pthread_barrier_wait( &barrera );
    if(!controlador_termino)
        pthread_barrier_wait(&barrera); 
}


/*  EFECTO: Metodo que realiza la escritura a mem principal 
            de parte del buffer. */
void Controlador::buffer_a_mem()
{
    //saca el boque del inicio y toma el candado
    BloqueDatos victima = buffer_vic.sacar();
    int dir_en_buffer = buffer_vic.inicio - 1; //al sacarlo se le suma 1 al inicio entonces aqui se le resta para saber de verdad caul se saco
    int direccion; //direccion de memoria que va a ser reemplazado
    int retrasos = 0;
    while(retrasos < 24) //los 24 retrasos de reloj para escribir en mem principal
    {
        pthread_barrier_wait(&barrera);
        retrasos++;
    }
    direccion = victima.bloque * 2;
    //escritura en la memoria 
    memoria.datos[direccion] = victima.palabra[0];
    memoria.datos[direccion + 1] = victima.palabra[1];

    if( dir_en_buffer == -1 )//por propiedades de vector circular es necesario preguntar esta condicion
        dir_en_buffer = 7;
    //luego de escribirlo en memoria regresa al estado en que se inicializo y se libera el candado
    buffer_vic.buffer[dir_en_buffer].estado = LIBRE;
    buffer_vic.buffer[dir_en_buffer].bloque = -1;
    pthread_mutex_unlock( &buffer_vic.candado[dir_en_buffer] );
}
    
/*  EFECTO: Cambia de hilillo de ejecucion y junto a el las estructuras 
            de estos hilos que se utiliza.*/
void Controlador::cambio_contexto()
{
    vector_hilos.hilos[vector_hilos.puntero_actual].RL = -1; //asignacion del RL al hacer cambio de contexto
    vector_hilos.puntero_actual = (vector_hilos.puntero_actual+1) % vector_hilos.hilos.size(); //cual sera el siguiente hilo utilizando propiedades de vector circular
    inst_ejecutadas = 0; //reinicia las instrucciones ejecutadas porque esta variable es la que se compara con el quantum
}

void Controlador::cargar_hilos()
{
    //pedir el quantum
    std::string input = ""; //variable que va a guardar el input del usuario
    int quantum = 0;
    std::cout << "De cuantos ciclos de reloj va a ser el quantum? Digite un número mayor o igual a 10." << std::endl;
    while(true)
    {
        getline(std::cin,input);
        std::stringstream stream(input);
        bool es_numero = true;//boolean que se vuelve falso si el input no es un número
        for(int i = 0; i < input.length(); i++)//se revisan los caracteres del numero para ver si corresponden a digitos, sino se considera un input inválido
        { 
            if(input.at(i) < 48 || input.at(i) > 57)
            {
                es_numero = false;
                break;
            }    
        }
        if(es_numero && (stream >> quantum) && (quantum >= 10)) //si el input es un numero valido
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
        bool es_numero = true;//boolean que se vuelve falso si el input no es un número
        for(int i = 0; i < input.length(); i++){ //se revisan los caracteres del numero para ver si corresponden a digitos, sino se considera un input inválido
            if(input.at(i) < 48 || input.at(i) > 57)
            {
                es_numero = false;
                break;
            }    
        }
        if(es_numero && (stream >> num_hilillos)) //si el input es un numero valido
            break;
        else
            std::cout << "Número inválido, por favor trate de nuevo." <<std::endl;
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
        vector_hilos.hilos[i].reloj_fin = 0;
        vector_hilos.hilos[i].reloj_inicio = 0;
        vector_hilos.hilos[i].RL = -1;
        vector_hilos.hilos[i].tiempo_en_ejecucion = 0;
        vector_hilos.hilos[i].id = i;
        for( int j = 0; j < 32; ++j )
            vector_hilos.hilos[i].registros[j] = 0;
    }
    vector_hilos.puntero_actual = 0;
}


/*  EFECTO: Inicializacion de las caches, memorias y buffer */
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
    //Init de cache de instrucciones con 0
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
    hillilo_termino = false;
    // mientras hayan hilos por ejecutar
    while( vector_hilos.hilos.size() != 0 )
    {
        // se trabaja con el hilo al que apunta vector_hilos.puntero_actual
        // se carga al IR la instrucción que apunta el PC
        cargar( vector_hilos.hilos[vector_hilos.puntero_actual].PC,
                vector_hilos.hilos[vector_hilos.puntero_actual].IR, 'I' );
        // se apunta a la siguiente direccion
        vector_hilos.hilos[vector_hilos.puntero_actual].PC += 4;
        // se manda a ejecutar la instrucción
        asociar( vector_hilos.hilos[vector_hilos.puntero_actual].IR[0],
                vector_hilos.hilos[vector_hilos.puntero_actual].IR[1],
                vector_hilos.hilos[vector_hilos.puntero_actual].IR[2],
                vector_hilos.hilos[vector_hilos.puntero_actual].IR[3] );
        // se aumenta contador de instrucciones ejecutadas por este hilillo
        inst_ejecutadas++;
        // se activa la bandera ("señal") de que se ejecutó una instrucción
        se_ejecuto_ins = true;
        pthread_barrier_wait(&barrera);
        // se espera señal para seguir con la siguiente inst. del mismo hilo u otro
        sem_wait( &senal_ejecutar_a_controlador );
    }
    impresion_final();
    while(!buffer_termino)
        pthread_barrier_wait(&barrera); //para darle tiempo al buffer que termine y lograr su join
    hillilo_termino = true;
    pthread_barrier_wait(&barrera);
    if(!controlador_termino)
        pthread_barrier_wait(&barrera); 
}

/*  EFECTO: impresion al final del programa de la cache, memoria y registros*/
void Controlador::impresion_final()
{
    int i, j;
    std::cout << "IMPRESIONES FINALES:" << std::endl;
    std::cout << "Memoria compartida de datos:" << std::endl;
    for(i = 0; i < 96; ++i)
        std::cout << "[" << i*4 << "]" << ": " << memoria.datos[i] << ", ";
    std::cout << std::endl << std::endl;

    std::cout << "Cache de datos:"<<std::endl;
    for( i = 0; i < 4; i++ )
    {
        std::cout << "Bloque: " << i << std::endl;
        std::cout << cache.datos[i].palabra[0] << " ";
        std::cout << cache.datos[i].palabra[1] << std::endl;
        std::cout << "Estado: ";
        switch( cache.datos[i].estado )
        {
            case INVALIDO:
                std::cout << "INVALIDO" << std::endl;
                break;
            case COMPARTIDO:
                std::cout << "COMPARTIDO" << std::endl;
                break;
            case MODIFICADO:
                std::cout << "MODIFICADO" << std::endl;
                break;
        }
    }
    std::cout << std::endl << std::endl;
    
    std::cout << "Buffer victima: " << std::endl;
    for( int i = 0; i < 8; ++i )
    {
        std::cout << "Bloque: " << i << std::endl;
        std::cout << buffer_vic.buffer[i].palabra[0]<< " ";
        std::cout << buffer_vic.buffer[i].palabra[1] <<std::endl;
        std::cout << "Estado: ";
        switch( buffer_vic.buffer[i].estado )
        {
            case VALIDO:
                std::cout << "VALIDO" << std::endl;
                break;
            case LIBRE:
                std::cout << "LIBRE" << std::endl;
                break;
            case SUBIENDO:
                std::cout << "SUBIENDO" << std::endl;
                break;
            case MERGING:
                std::cout << "MERGING" << std::endl;
                break;
            case ESCRIBIENDO:
                std::cout << "ESCRIBIENDO" << std::endl;
        }
    }
    std::cout << std::endl;

    std::cout << "Hilillos: " << std::endl;
    for( int i = 0; i < historial_hilos.size(); ++i )
    {
        std::cout << "Hilillo " << historial_hilos[i].id << std::endl;
        std::cout << "Registros: " << std::endl;
        for( int j = 0; j < 32; ++j )
            std::cout << "x" << j << ": " << historial_hilos[i].registros[j] << std::endl;
        std::cout << "Ciclos tardados en ejecucion: " << historial_hilos[i].tiempo_en_ejecucion << std::endl;
        std::cout << "Reloj cuando comenzo ejecucion: " << historial_hilos[i].reloj_inicio << std::endl;
        std::cout << "Reloj cuando termino ejecucion: " << historial_hilos[i].reloj_fin << std::endl << std::endl;
    }
}

/*  EFECTO: se carga desde memoria la palabra ubicada en la dirección
            que se indica, en este método se busca la palabra en caché
            y si es el caso se busca en el buffer víctima.
    RECIBE: (direccion) direccion de memoria.
            (palabra_retorno) puntero donde se retorna la palabra.
            (memoria) parámetro opcional para indicar si la palabra
            requerida es de instrucciones o de datos.*/
void Controlador::cargar( int direccion, int * palabra_retorno, char memoria )
{
    // se obtienen bloque y palabra a los que pertenece la dir. de memoria
    int num_bloque = floor(direccion/8);
    int num_palabra = floor(direccion/4);
    if( memoria == 'I' ) // se carga desde cache de instrucciones
    {
        // se busca en cache de instrucciones
        if( cache.instrucciones[num_bloque%8].bloque != num_bloque ) // mapeo directo
        {
            // se dio fallo de caché
            // se procede a cargar de memoria principal
            BloqueInstruc bloque_instr;
            cargar_de_mem_principal( num_bloque, bloque_instr.palabra );
            bloque_instr.bloque = num_bloque;
            bloque_instr.estado = COMPARTIDO; // estado compartido
            // se copia en cache de instrucciones
            copiar_a_cache( &bloque_instr, 24 ); // aqui se duran los 24 ciclos de retraso
        }
        // acierto de lectura o ya se subio el bloque
        // si el numero de palabra es par entonces corresponde a
        // la primer palabra de su bloque, si no, es la segunda.
        int palabra_pos = (num_palabra % 2) ? 4:0;
        // se retorna la palabra
        for( int i = 0; i < 4; ++i )
            palabra_retorno[i] = cache.instrucciones[num_bloque%8].palabra[palabra_pos+i];
    }
    else // se carga desde cache de datos
    {
        int bloque_cache = buscar_en_cache_datos( num_bloque );
        if( bloque_cache == -1 ) // fallo de lectura de cache de datos
        {
            // se busca en el buffer victima
            // aqui se duraria la espera por si el bloque esta siendo copiado a memoria
            // y se obtiene el acceso exclusivo.
            int bloque_buffer = buffer_vic.buscar( num_bloque );
            if( bloque_buffer != -1 )
            {
                // se realiza la copia desde buffer víctima donde se dan los 4 ciclos de retraso
                // libremente se puede colocar el estado subiendo porque se tiene el candado
                buffer_vic.buffer[bloque_buffer].estado = SUBIENDO;
                bloque_cache = copiar_a_cache( &buffer_vic.buffer[bloque_buffer], 4, bloque_buffer );
            }
            else // el bloque no estaba en el buffer
            {
                // se carga el bloque desde memoria principal
                BloqueDatos bloque_datos;
                cargar_de_mem_principal( num_bloque, bloque_datos.palabra );
                bloque_datos.bloque = num_bloque;
                bloque_datos.estado = COMPARTIDO;
                // se realiza la copia
                bloque_cache = copiar_a_cache( &bloque_datos, 24 ); // aqui se durarian los 24 ciclos
            }
        }
        // acierto de lectura o ya se subio el bloque a cache
        // si el numero de palabra es par entonces corresponde a
        // la primer palabra de su bloque, si no, es la segunda
        int palabra_pos = (num_palabra % 2) ? 1:0;
        // se retorna la palabra
        *palabra_retorno = cache.datos[bloque_cache].palabra[palabra_pos];
        // cuando se carga tambien se esta haciendo uso del bloque
        cache.datos[bloque_cache].ultimo_uso = reloj;
    }
}

/*  EFECTO: retorna las palabras de un bloque específico.
    RECIBE: (int num_bloque) el número de bloque.
            (int * bloque_retorno) puntero a entero donde se retorna
            el bloque solicitado. */
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

/*  RECIBE: (num_bloque) número de bloque que se busca.
    RETORNA: número de bloque dentro de la caché donde se
             encuentra el bloque que se busca.*/
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

/*  EFECTO: Metodo que carga de buffer o de memoria prin y bien 
            tambien si el que se reemplaza estaba modif, se escribe en el buffer
    RECIBE: (bloque) Bloque que se va a escribir en la cache
            (retraso) cantidad de ciclos de retraso .
            (num_bloque_en_buffer) numero de bloque en el buffer donde se esta cargando el bloque*/
int Controlador::copiar_a_cache( Bloque * bloque, int retraso, int num_bloque_en_buffer ) // devuelve en bloque en cache donde hizo la copia
{
    int bloque_cache;
    int counter = 0;
    int num_bloque = bloque->bloque;
    int conjunto = num_bloque % 2;
    int direc_reemplazo;
    BloqueDatos * bloq_datos = dynamic_cast< BloqueDatos * >( bloque );
    if( bloq_datos != NULL ) //carga de datos ya sea desde el buffer o bien desde memoria principal
    {  
        direc_reemplazo = menos_recien_usado( conjunto ); 
        while(counter < retraso) //Aqui se espera ya sea 4 ciclos o 24 ciclos de reloj
        {
            pthread_barrier_wait(&barrera);
            ++counter;
        }
        if(cache.datos[direc_reemplazo].estado == INVALIDO || cache.datos[direc_reemplazo].estado == COMPARTIDO)
        {
            cache.datos[direc_reemplazo] = *bloq_datos; //se le puede caer encima libremente                   
        }
        else // estado del bloque en cache MODIFICADO
        {
            reemplazo_bloq_modif(bloq_datos,direc_reemplazo);
        }
    }
    else// se copia a cache de instrucciones
    {
        BloqueInstruc * bloq_instr = dynamic_cast< BloqueInstruc * >( bloque );
        while(counter < retraso) //retraso que entra por parametro, en este caso es de 24
        {
            pthread_barrier_wait(&barrera);
            ++counter;
        }
        // reemplazo de mapeo directo
        cache.instrucciones[num_bloque%8] = *bloq_instr;
        direc_reemplazo = num_bloque%8;
    }
    if(retraso == 4)
    {
        //Si el retraso fue de 4, en este caso se sabe que trabajo con el buffer entonces se vuelve el bloque valido y se libera candado de bloque
        bloq_datos->estado = VALIDO;
        pthread_mutex_unlock( &buffer_vic.candado[num_bloque_en_buffer] );
    }
    return direc_reemplazo;
}

/*  EFECTO: Se encargo del reemplazo cuando el bloque de cache estaba en estado modificado
    RECIBE: (bloq_datos) Bloque que se va a escribir en la cache
            (direc_reemplazo) direccion de reemplazo en la cache.*/
void Controlador::reemplazo_bloq_modif(BloqueDatos * bloq_datos,int direc_reemplazo)
{
    int direc_reemplazo_buff;
    bool insertado = false;
    bool estaba_vacia = false;
    int counter;
    //encuentra bloque a utilizar y coloca candado en el
    direc_reemplazo_buff = buffer_vic.buscar(cache.datos[direc_reemplazo].bloque);
    if(direc_reemplazo_buff == -1) // -1 indica que el buscar no encontro el bloque
    {
        while(!insertado)
        {
            if(!buffer_vic.llena())
            {
                insertado = true;
                if(buffer_vic.vacia())  
                 //por cuestiones de sincronizacion se ocupa verificar si esta vacia antes de insertar al buffer
                //con el fin de despertar el hilo del buffer que estaria dormido
                    estaba_vacia = true;
                counter = 0;
                while(counter < 4) //retraso de 4 ciclos para insertar en el buffer
                {
                    pthread_barrier_wait(&barrera);
                    counter++;
                }
                buffer_vic.insertar(cache.datos[direc_reemplazo]); //el bloque que va a ser reemplazado se guarda en el buffer
                if( estaba_vacia )
                {
                    sem_post( &senal_hilo_a_buffer ); //despierta el hilo del buffer
                    estaba_vacia = false;
                }
                cache.datos[direc_reemplazo] = *bloq_datos; //se modifica la cache
            }
            else //el buffer esta lleno, asi que espero a que tenga un espacio
            {
                pthread_barrier_wait(&barrera); 
            }
        }
    }
    else
    {
        counter = 0;
        //libremente se puede colocar el estado merging porque se tiene el candado de este bloque del buscar
        buffer_vic.buffer[direc_reemplazo_buff].estado = MERGING; 
        while(counter < 4) //retraso por hacer merging
        {
            pthread_barrier_wait(&barrera);
            counter++;
        }
        buffer_vic.buffer[direc_reemplazo_buff] = cache.datos[direc_reemplazo]; //se realiza el merging
        buffer_vic.buffer[direc_reemplazo_buff].estado = VALIDO; //el bloque vuelve a estar valido
        pthread_mutex_unlock( &buffer_vic.candado[direc_reemplazo_buff] ); //se libera el candado del bloque
        cache.datos[direc_reemplazo] = *bloq_datos; //se modifica la cache
    }
}

/*  EFECTO: se escribe a memoria la palabra que se pasa por parámetro,
            en este método se contemplan fallos de caché y lo que
            implican.
    RECIBE: (direccion) direccion de memoria donde se quiere escribir.
            (palabra) palabra que se escribe.*/
void Controlador::escribir( int direccion, int palabra )
{
    // se obtienen bloque y palabra a los que pertenece la dir. de memoria
    int num_bloque = floor(direccion/8);
    int num_palabra = floor(direccion/4);
    int bloque_cache = buscar_en_cache_datos( num_bloque );
    if( bloque_cache == -1 ) // fallo de escritura en cache
    {
        // buscar en el buffer victima
        // se busca en el buffer victima
        // aqui se duraria la espera por si el bloque esta siendo copiado a memoria
        int bloque_buffer = buffer_vic.buscar( num_bloque );
        if( bloque_buffer != -1 )
        {
            // se copia desde buffer víctima
            // libremente se puede colocar el estado subiendo porque se tiene el candado
            buffer_vic.buffer[bloque_buffer].estado = SUBIENDO;
            bloque_cache = copiar_a_cache( &buffer_vic.buffer[bloque_buffer], 4, bloque_buffer );
        }
        else // el bloque no estaba en el buffer
        {
            // se carga el bloque desde memoria principal
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
    cache.datos[bloque_cache].estado = MODIFICADO;
    cache.datos[bloque_cache].ultimo_uso = reloj;
}

void Controlador::controlador()
{
    bool cambio_de_contexto = false; //booleano que avisa si se hizo un cambio de contexto
    fin_de_hilillo = false; //booleano que avisa si un hilillo ejecutó la instrucción FIN
    controlador_termino = false;
    while(vector_hilos.hilos.size() != 0){
        aumentar_reloj(); // en este metodo hay una barrera para aumentar el reloj cuando se ejecuta una instrucción
        if(inst_ejecutadas == quantum  || fin_de_hilillo ) //si el hilillo actual ha ejecutado la cantidad de instrucciones correspondientes al quantum o ejecutó la instrucción FIN
        {
            if(fin_de_hilillo){ //si ejecutó la instrucción FIN
                vector_hilos.hilos[vector_hilos.puntero_actual].reloj_fin = reloj; // se guarda el ciclo de reloj en el que terminó el hilillo
                historial_hilos.push_back( vector_hilos.hilos[vector_hilos.puntero_actual] ); // se guarda el hilillo antes de sacarlo de cola de ejecucion
                vector_hilos.hilos.erase(vector_hilos.hilos.begin()+vector_hilos.puntero_actual); // se saca el hilillo de la cola de hilillos
                fin_de_hilillo = false; //se indica que el booleano ahora es falso para que no saque al siguiente hilo
            }
            if( vector_hilos.hilos.size() != 0 ) //si aún quedan hilillos en la cola
            {
                cambio_contexto(); // se hace un cambio de contexto
                cambio_de_contexto = true; //se pone el booleano que indica el cambio de contexto en true
            }
        }
        if( se_ejecuto_ins || cambio_de_contexto ) //si se ejecutó una instrucción o se realizó un cambio de contexto
        {
            se_ejecuto_ins = false; //vuelve el booleano se_ejecuto_ins a falso para que se active hasta que se ejecute la próxima instrucción
            cambio_de_contexto = false; //vuelve este booleano a falso para que no se de un cambio de contexto cuando no tiene que darse
            sem_post( &senal_ejecutar_a_controlador ); //se manda la señal post al semáforo para que el hilo de ejecución siga con la siguiente instrucción
        }
    }
    while(!buffer_termino) 
        pthread_barrier_wait(&barrera); //para darle tiempo al buffer que termine y lograr su join
    controlador_termino = true;
    pthread_barrier_wait(&barrera);
    if(!hillilo_termino)
        pthread_barrier_wait(&barrera); //para darle tiempo al buffer que termine y lograr su join
}

/*  EFECTO: Busca cual tiene el ultimo uso mas viejo.
    RECIBE: (conjunto) Conjunto en el que se evaluara el ultimo uso
    RETORNA: La posicion donde el algoritmo determina que se 
            hara el reemplazo*/
int Controlador::menos_recien_usado( int conjunto ) 
{
    int menor = INT_MAX; //el entero con signo mas grande que se puede representar 
    int direccion; //direccion que se va a reemplazar y sera retornado
    for(int i = 0; i < 2; ++i)
    {
        if(cache.datos[(conjunto*2)+i].ultimo_uso < menor) 
        {
            //si se encuentra un ultimo uso menor se reemplaza la variable menor por el nuevo
            menor = cache.datos[(conjunto*2)+i].ultimo_uso;
            direccion = (conjunto*2)+i;
        }
    }
    return direccion;
}

/*  EFECTO: manda a correr los 3 hilos necesarios para la simulación.*/
void Controlador::init_hilos()
{
    pthread_barrier_init(&barrera, NULL, 3);
    hilos[0] = std::thread( hilo_controlador, this );
    hilos[1] = std::thread( hilo_hilillo, this );
    hilos[2] = std::thread( hilo_buffer, this );
}

/*  EFECTO: se espera que finalicen los 3 hilos creados.*/
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

