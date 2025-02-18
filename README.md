# LSUS

This repository contains a set of algorithms to compute the LSUS array, which stores the length of all Shortest Unique Substring that begin in each index i of a string S[1,n].

## Installation

**Download**

```sh
git clone https://github.com/felipelouza/LSUS.git
cd LSUS
make
```

**Build requirements**

An ANSI C Compiler (e.g. GNU GCC) and make.

## Execution

```sh
./lsus INPUT [options]
```
where INPUT is a single file. 

### Available options:

```sh
-A a	preferred algorithm to use (default 3)
-k K	use the first K strings of the INPUT
-o    output computed array to disk (INPUT.4.lsus)
-t    print the running time	
-c    check output (for debug)
-p    print the output (for debug)
-h    this help message
```
### Note

For inputs larger than **4GB**, **_lsus-64_** must be used.
```sh
./lsus-64 INPUT [options]
```

## Running example

Given a string stored in an input _txt_ file (dataset/example.txt):

```c
more dataset/example.txt
GCTCTC
```

### Run a test:

```c
./lsus dataset/example.txt -A 3 -p
```

**Output:**

```c
## PREPROCESSING ##
N = 7 bytes
sizeof(int) = 4 bytes
MEM = 0.00 GB
## SACAK ##
## PHI ##
## PLCP_LSUS ##
## TOTAL ##
CLOCK = 0.000057 TIME = 0.000000
0.000057
LSUS[0]: 1	 T[0]: G
LSUS[1]: 4	 T[1]: C
LSUS[2]: 3	 T[2]: T
LSUS[3]: 0	 T[3]: C
LSUS[4]: 0	 T[4]: T
LSUS[5]: 0	 T[5]: C
LSUS[6]: 0	 T[6]: 0
malloc_count ### exiting, total: 23,325, peak: 22,216, current: 1,024
```
