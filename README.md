# parallel-monte-carlo-pi
Experimental Parallel Monte Carlo Pi Estimation for CPU and GPU.

To get better accuracy, Monte Carlo algorithm uses 64 bit integers - so you can have a large number of samples 2**64 - 1

Multicore CPU: Has been compiled and runs with OpenMP on x86_64. I would be very interested if someone could test
ARM  or IBM Power8 CPU's.

GPU Accelerated:  OpenACC / CUDA version in progress.

Thrust Library and TBB versions planned.

 Status on RNG:

 Oiriginally implemented using TRNG (trng4) parallel random number generation.

 After this, I found another RNG called sitmo and the source file for it is included.

 As of now (2015-11-23) the CPU version uses the sitmo parallel RNG.

How it works:

The simulator determins a number of samples - and then runs them in parallel.

The default number of samples is a full 24 bit integer, i.e. 16.7 million samples.  Then this value is shifted left 4, and the
1 is extended on the low order bit, so by default the number of saples is 16 * 16.7M = 268,435,455.

./pi-omp


You can adjust the number of samples used by the Monte Carlo algorithm by passing a number of bits to shift left i.e.:

./pi-omp 6

will run the sumulator with 4,294,967,295 samples.

The max number of bits you can specify is 40 (24 + 40 = 64)

TODO:

Add some Markdown to this.

More testing of TRNG vs. sitmo RNG.

OpenACC support.