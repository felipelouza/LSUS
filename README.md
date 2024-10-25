# LSUS
LSUS is an array that stores the length of all Shortest Unique Substring that begin in each index i of a string.
## Build requirements

An ANSI C Compiler (e.g. GNU GCC)
## Example

**Compilation:**

```sh
make
```
**Run a test:**

```c
./main-sus dataset/example.txt -A 7 -p
```

**Output:**

```c
./main-sus dataset/example.txt -A 7 -p 
## PREPROCESSING ##
N = 7 bytes
sizeof(int) = 4 bytes
## SACAK ##
## PHI ##
## PLCPSUS ##
LSUS[0]: 	 1	 T[0]:	 G
LSUS[1]: 	 4	 T[1]:	 C
LSUS[2]: 	 3	 T[2]:	 T
LSUS[3]: 	 0	 T[3]:	 C
LSUS[4]: 	 0	 T[4]:	 T
LSUS[5]: 	 0	 T[5]:	 C
LSUS[6]: 	 0	 T[6]:	 0
LSUS[7]: 	 0	 T[7]:	 0
malloc_count ### exiting, total: 23,320, peak: 22,216, current: 1,176
```
