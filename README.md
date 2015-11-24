# parallel-monte-carlo-pi
Experimental Parallel Monte Carlo Pi Estimation for

Multicore CPU: Has been compiled and runs with OpenMP on x86_64. I would be very interested if someone could test
ARM  or IBM Power8 CPU's.

GPU Accelerated:  OpenACC / CUDA version in progress.

Thrust Library and TBB versions planned.

 Status on RNG:

 Oiriginally implemented using TRNG (trng4) parallel random number generation.

 After this, I found another RNG called sitmo and the source file for it is included.

 As of now (2015-11-23) the CPU version uses the sitmo parallel RNG.
