omp:  pi-omp

pi-omp: pi-omp.o
	g++ -o pi-omp pi-omp.o -l:libtrng4.a -lgomp

pi-omp.o: pi-omp.cpp
	g++ -std=c++11 -g -O2 -fopenmp -c pi-omp.cpp
clean:
	rm *.o pi-omp
