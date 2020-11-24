contador: main.cpp procesador.cpp 
	g++ main.cpp procesador.cpp -pthread -o procesador.out

clean:
	rm -f *.out
