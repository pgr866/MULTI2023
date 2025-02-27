## Example of lrand48\_r() usage

 - - -
### Instructions for SumVectorInt-OMP_R:
 
1. **Sequential version**:
    - Sequential version will use srand48() and lrand48().
    - Compile without -fopenmp in the makefile: `$ make  AddVectorInt-OMP_R`
    - The vector is initialized using lrand48()
    - View the program's wall clock time with `$ time AddVectorInt-OMP_R -r 500000000` 
```console
[leo@miro OMP-Random_r]$ time AddVectorInt-OMP_R -r 500000000
SumVectorInt sequential: rows=500000000 
T. Init Vector = 4.04847 sec.
T. Add Vector  = 1.11019 sec.
SumVectorInt   = 24749613540.

real	0m5,226s
user	0m4,759s
sys	0m0,458s
```

2. **OMP version with lrand48()**
    - Use `#define OMP_R 0` in the program and compile the OMP version: with -fopenmp.
    - Main uses srand48() and threads make use of lrand48().
    - Since lrand48() generates a pseudo-random sequence, the next number depends on the previous one and the first of the seed in srand48(time(NULL)).
    - Therefore, some threads must wait for other to finish lrand48().
    - Set the number of threads. For example to 4: `$ export OMP_NUM_THREADS=4`
    - Measure time with the same vector size.
    - It should take longer than sequential since there is contention in the threads to get the next speudo-random number.
    - The addition is different because the vector is randomly generated with different seed.
```console
[leo@miro OMP-Random_r]$ time AddVectorInt-OMP_R -r 500000000
SumVectorInt OMP with 4 threads, Rows=500000000, using lrand48() 
T. Init Vector = 9.69849 sec.
T. Add Vector  = 0.23505 sec.
SumVectorInt   = 2249899181.

real	0m10,004s
user	0m37,542s
sys	0m0,609s
```

3. **OMP version with lrand48\_r()**
    - Use `#define OMP_R 1` and compile for OMP.
    - Now each thread in the InitVectorInt routine uses its own pseudorandom sequence with ldrand48\_r() and initializes it with srand48\_r(). So the threads use a local and private drand48_data structure and do not interfere with each other when the seed is established and when they generate their pseudo-random sequence.
    - Measure time of the parallel algorithm with the same vector size.
    - A reasonable speed-up must be obtained.
```console
[leo@miro OMP-Random_r]$ time AddVectorInt-OMP_R -r 500000000
SumVectorInt OMP with 4 threads, Rows=500000000, using lrand48_r() 
T. Init Vector = 0.894483 sec.
T. Add Vector  = 0.235431 sec.
SumVectorInt   = 24750437696.

real	0m1,203s
user	0m4,088s
sys	0m0,486s
```
 - - -
### Instructions for SumVectorInt-OMP_R_Vector:

  * Here srand48\_r() and lrand48\_r() are always used in OMP.
  * Main gets memory for a vector of NThreads drand48\_data, one for each thread.

4. **OMP version using calloc for pRandData vector**
 * Let's see what happens if the main:
     - As in this example, there exists cases where the main gets memory for a vector where each thread writes in the position indicated by its thread's number. In this case the vector is pRandData and each element of the vector is a drand48\_data. This can cause **false sharing**.
     - Additional thing is the **first touch**. Previous vector is stored in RAM using calloc and therefore in continuous memory. Data goes to the L1 of the thread first touches it. The problem is that not only the data for this thread is in the cache line but also the data of another thread running in another core.
     - Set FSHARING to 1 (having false sharing) in AddVectorInt-OMP_R_Vector.c
     - Run the program in parallel for the same number of threads and vector size.
     - The time is similar/larger to/than the sequential. 
```console
[leo@miro OMP-Random_r]$ time AddVectorInt-OMP_R_Vector -r 500000000
SumVectorInt_OMP_R_Vector with 4 threads, Rows=500000000.
Using calloc for pRandData.
T. Init Vector    = 5.19662 sec.
T. Add Vector     = 0.234318 sec.
SumVectorInt      = 2250096058.

real	0m5,501s
user	0m17,209s
sys	0m0,516s
```
 * Do a performance evaluation:
```console
perf stat -r 3 -ddd  AddVectorInt-OMP_R_Vector -r 500000000
...
  page-faults:u = 488.369
  L1-dcache-load-misses:u = 405.519.617
  LLC-load-misses:u =1.066.880
...
```

5. **OMP version using aligned_alloc for drand48\_data**
 * This version does the same as previous one but:
    - Gets the cache line size with `sysconf(_SC_LEVEL1_DCACHE_LINESIZE);`
    - Gets a vector ppRandData of NThreads pointers to drand48\_data.
    - Each ppRandData[i] is allocated in memory but aligned to cache line size using `aligned_alloc()`.
    - Set FSHARING to 0 (no false sharing) in AddVectorInt-OMP_R_Vector.c
    - Run the program in parallel for the same number of threads and vector size.
    - The time is similar to version in point 3, where drand48_data is local to each thread. 
```console
[leo@miro OMP-Random_r]$ time AddVectorInt-OMP_R_Vector -r 500000000
SumVectorInt-OMP_R_Vector with 4 threads, Rows=500000000.
Using aligned_alloc for *ppRandata[i].
Cache line size   = 64.
drand48_data size = 24 
T. Init Vector    = 1.04029 sec.
T. Add Vector     = 0.234652 sec.
SumVectorInt      = 2250013696.

real	0m1,345s
user	0m4,641s
sys	0m0,487s
```
 * Cache line is 64b and drand48\_data structure uses 24b. So, in a cache line used in previous point 4 there are at least  two drand48\_data. Here, one drand48\_data is alone in each cache line thanks to `aligned_alloc()`. 
 * Do a performance evaluation:
```console
perf stat -r 3 -ddd  AddVectorInt-OMP_R_Vector -r 500000000
...
  page-faults:u = 488.370
  L1-dcache-load-misses:u = 31.863.431
  LLC-load-misses:u = 1.102.767
...
```
* The L1-dcache-load-misses is smaller than in point 4.


