simulador: main.cpp procesador.cpp 
	g++ main.cpp procesador.cpp -pthread -o procesador

clean:
	rm -f *.out
