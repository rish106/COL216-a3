# MIPS Processor Cache Simulator
Simulation of the cache structure of MIPS Processor (L1 and L2 Caches) in C++. We get to know the number of read and write misses occuring at each of these caches,
the miss rate at each of these caches and the number of writebacks taking place at each cache when the processor executes a program (interpreted as "trace" for this, since the instruction type(read/write) and the memory address are the only things that matter for caching)
with the help of this simulation. Completed as a part of the course COL216 (Computer Architecture) at IIT Delhi.

## Trace files
Each line of the trace files is an instruction containing the instruction type(read/write) and the memory address.

Each instruction has 1 character `r` or `w` followed by a separator(tab(s) or space(s)), then the memory address in hexadecimal (without leading `0x0`) at which the instruction is reading from or writing to.

Example trace files are provided in `memory_trace_files` folder.

## Run the simulation
1. Compile the program using
```
make compile
```

2. Run the executable with the following parameters
```
./cache_simulate <block_size> <L1_cache_size> <L1_associativity> <L2_cache_size> <L2_associativity> <trace_file>
```

- `block_size` denotes the size of a block in L1 and L2 Caches

- `L1_cache_size` denotes the total size of the L1 cache

- `L1_associativity` denotes the number of ways in the L1 cache

- `L2_cache_size` denotes the total size of the L2 cache

- `L2_associativity` denotes the number of ways in the L2 cache

- `trace_file` denotes the path(including name) of the trace file on which the program needs to be executed

All the above parameters need to be a power of 2.

**Example**: `./cache_simulate 64 1024 4 65536 8 memory_trace_files/trace1.txt`
