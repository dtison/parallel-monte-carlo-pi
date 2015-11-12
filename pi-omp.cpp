
/*
 *  Experimental Parallel Monte Carlo Pi Estomator
 *
 */


#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <trng/yarn2.hpp>
#include <trng/uniform01_dist.hpp>
#include <inttypes.h>
#include <chrono>


using namespace std;

#define NUM_BITS 4

uint64_t count = 0;						//Count holds all the number of how many good coordinates

int num_threads;

int main(int argc, char* argv[]) {


    //  This value is 2**24 anout 16.7 M
    uint64_t iterations = 0x0000000000FFFFFF;

    uint32_t num_bits = NUM_BITS;

    if (argc > 1) {
        uint32_t value = stoi(argv[1]);
        value = min (40u,  value);
        if (value > num_bits) {
            num_bits = value;
        }
    }
    //
    for (int i = 0; i < num_bits; i++) {
        iterations <<= 1;
        iterations |= 1;
    }

    uint64_t progress = iterations >> 5;

    //  Tune numeric display
    cout.precision(numeric_limits<double>::digits10 + 1);

    cout << "Shifting " << num_bits << " bits - Total iterations will be " << iterations << " Progress modulus " << progress << endl;

    //  Mark begin tome
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();


    const uint64_t samples = iterations;

    // number of points in circle
    uint64_t points_inside = 0;
    uint64_t in2 = 0;

    //  Launches team of threads
#pragma omp parallel reduction(+:points_inside, in2)
    {
        trng::yarn2 r;
        int size = omp_get_num_threads();
        int rank = omp_get_thread_num();
        //  Save number of threads
        if (rank == 0) {
            num_threads = size;
        }
        // random number distribution
        trng::uniform01_dist<> u;
        //  Synchronize thread phase int4erval for RNG
    //    r.jump (4 * (rank * samples / size));
        r.jump (2 * (rank * samples / size));


#pragma omp for
        for (uint64_t i = 0; i < iterations; i++) {
            double x = u(r), y = u(r);
            if (x * x + y * y <= 1.0) {
                if ((i % progress) == 0) {
                    cout << "." << flush;
                }
                ++in2;
            }
        }
/*
        // throw random points into square
        for (uint64_t i = rank*samples/size; i < (rank+1)*samples/size; ++i) {
            double x=u(r), y=u(r);            // choose random x- and y-coordinates
            if (x*x+y*y<=1.0)                 // is point in circle?
                ++in;                           // increase thread-local counter
        }
*/



    }


    std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();

    std::cout << "Time was " << std::chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.f << " seconds " << std::endl;
    //std::cout << "Time difference = " << std::chrono::duration_cast<chrono::nanoseconds> (end - begin).count() <<std::endl;





    // print result
    //std::cout << "pi = " << 4.0*in / samples << endl <<  "Threads " << num_threads << std::endl;
    std::cout << "pi2 = " << 4.0*in2 / samples << endl <<  "Threads " << num_threads << std::endl;

    return EXIT_SUCCESS;











/*	double pi;						//holds approx value of pi
	time_t t;

	bool num_threads_displayed = false;
	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);



	setbuf(stdout, NULL);


	srand48(((unsigned)time(&t)));				//Give rand() a seed value

//#pragma omp for reduction(+:count)
#pragma omp parallel for reduction(+:count)
	for (uint64_t i = 0; i <= niter; i++) {
		double x = (double)drand48();//RAND_MAX;			//gets a random x coordinate
		double y = (double)drand48();//RAND_MAX;			//gets a random y coordinate
		double z = ((x * x) + (y * y));
		if (z <= 1.0) {
			count++;
			if ((i % progress) == 0) {


			//	#pragma omp single
				if (! num_threads_displayed){
					printf("Num threads: %d\n", omp_get_num_threads());
					num_threads_displayed = true;
				}

				printf (".");
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &end);

	uint64_t timeElapsed = timespecDiff(&end, &start);

	pi = ((double)count/(double)niter)*4.0;				//p = 4(m/n)
	printf("\nPi: %f  %f\n", pi, (float) timeElapsed / 1000000);
	//printf("Elapsed %" PRId64 "\n", timeElapsed);

 */

}