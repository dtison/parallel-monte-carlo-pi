omp:  pi-omp

pi-omp: pi-omp.o
	g++ -o pi-omp pi-omp.o -l:libtrng4.a -lgomp

pi-omp.o: pi-omp.cpp
	g++ -std=c++11 -g -O2 -fopenmp -c pi-omp.cpp



mic:  pi-mic

pi-mic: pi-mic.o
	icc -mmic -fopenmp -o pi-mic pi-mic.o  ../trng-4.18/build/mic/src/.libs/libtrng4.a  

pi-mic.o: pi-omp.cpp
	icc -std=c++11 -mmic -fopenmp -I ../trng-4.18/build/mic -o pi-mic.o -c pi-omp.cpp






clean:
	rm *.o pi-omp pi-mic
