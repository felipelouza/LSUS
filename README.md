# LSUS

This tool computes the LSUS array A[1,n], which stores the length of all Shortest Unique Substring that begin in each index i of a string S[1,n].

## Installation

**Build requirements**

An ANSI C Compiler (e.g. GNU GCC) and make.


**Download**

```sh
git clone https://github.com/felipelouza/LSUS.git
cd LSUS
make
```

For inputs larger than **4GB**, **_lsus-64_** must be used.

**Available options:**

```sh
-A a	preferred algorithm to use (default 3)
-k K	use the first K strings of the INPUT
-o	output computed array to disk (INPUT.4.lsus)
-t  print the running time	
-c	check output (for debug)
-p  print the output (for debug)
-h	this help message
```

## Example

Given a string stored in an input file (dataset/example.txt):

```c
more dataset/example.txt
GCTCTC
```

**Run a test:**

```c
./lsus dataset/example.txt -A 3 -p
```

**Output:**

```c
## PREPROCESSING ##
N = 7 bytes
sizeof(int) = 4 bytes
## SACAK ##
## PHI ##
## PLCP_LSUS ##
## TOTAL ##
CLOCK = 0.000011 TIME = 0.000000
LSUS[0]: 1	 T[0]: G
LSUS[1]: 4	 T[1]: C
LSUS[2]: 3	 T[2]: T
LSUS[3]: 0	 T[3]: C
LSUS[4]: 0	 T[4]: T
LSUS[5]: 0	 T[5]: C
LSUS[6]: 0	 T[6]: 0
malloc_count ### exiting, total: 23,320, peak: 22,216, current: 1,144
```
