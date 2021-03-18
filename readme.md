# Anders Nielsen - CPE690
## Project 2
***
### Problem 1
- clocktest -> prints time between calls to clock_gettime()
- syscall   -> prints average time for a gettid call (n=1000)
- pipeSW    -> Uses fork() and pipe() to try to find context switching time. Inaccurate.
- futexSW   -> Uses shared memory to switch between threads. Remove SINGLE_CORE definition at the top to run on all processors. Heavily inspired by [Tsuna](https://blog.tsunanet.net/2010/11/how-long-does-it-take-to-make-context.html).

### Problem 2
- tlb       -> iterates over an array to find the average access time and TLB size. arguments are tlb [numpages][numtrials]
- tlb.py    -> Runs tlb with num trials ranging from 2 to 2048 in powers of two
- data.txt  -> Holds the data from tlb.py. Data is arranged by columns as time in ns for 2^col_num
- graph.png -> graph of averaged data in data.txt

### Problem 3
- visitor    -> requires 2 inputs: visitor [input_file][time_sec]
- input_file -> Direction each visitor approaches from in the format "L,R,R...L" 
