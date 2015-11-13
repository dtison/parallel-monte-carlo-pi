
/*
 *  Experimental Parallel Monte Carlo Pi Estomator
 *
 */


#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <trng/yarn2.hpp>
#include <trng/mt19937.hpp>
#include <trng/mt19937_64.hpp>
#include <trng/mrg2.hpp>

#include <trng/lcg64.hpp>

#include <trng/mt19937_64.hpp>

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

  /*  //  Mark begin tome
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

*/
    double start_time = omp_get_wtime();

    const uint64_t samples = iterations;

    // number of points in circle
    uint64_t points_inside = 0;
 //   uint64_t in2 = 0;

    //  Launches team of threads
#pragma omp parallel reduction(+:points_inside)
#pragma acc kernels
    {
        trng::yarn2 r;
      //  trng::lcg64 r;
      //  trng::mrg2 r;
        num_threads = omp_get_num_threads();
        int rank = omp_get_thread_num();

        // random number distribution
#pragma acc routine(trng::utility::u01xx_traits<double, (unsigned long)1, trng::yarn2>::addin) seq
//#pragma acc routine(trng::utility::u01xx_traits::addin) seq

        trng::uniform01_dist<> u;
        //  Synchronize thread phase int4erval for RNG

      //  r.jump (2 * (rank * samples / size));
        uint64_t old_jump =  (rank * samples / num_threads);


        uint64_t jump = static_cast<uint64_t>(
            static_cast<double>(iterations) / static_cast<double>(num_threads) * static_cast<double>(rank)
        );
        r.jump(jump);
        cout << "Jump for thread " << rank << " is " << jump << " old " << old_jump << endl;

#pragma omp for
        for (uint64_t i = 0; i < iterations; i++) {
            double x = u(r), y = u(r);
            if (x * x + y * y <= 1.0) {
                if ((i % progress) == 0) {
                    cout << "." << flush;
                }
                ++points_inside;
            }
        }
    }


 /*   std::chrono::steady_clock::time_point end= std::chrono::steady_clock::now();
    std::cout << "Time was " << std::chrono::duration_cast<chrono::microseconds>(end - begin).count() / 1000000.f << " seconds " << std::endl;
*/
    double run_time = omp_get_wtime() - start_time;


    // print result
    //std::cout << "pi = " << 4.0*in / samples << endl <<  "Threads " << num_threads << std::endl;
    std::cout << "pi2 = " << 4.0 * points_inside / samples << endl <<  "Threads " << num_threads <<
            " in " <<  setprecision (5)  << run_time  << " seconds " << std::endl;

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
//#pragma acc routine seq

