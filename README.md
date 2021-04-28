# SUS
SUS is vector with the length of all shortest unique substring that begin in i. In this algorithm there are, beyond the traditional way, other ways of building it. 
## Build requirements

An ANSI C Compiler (e.g. GNU GCC)
## Example

**Compilation:**

```sh
make
```
**Run a test:**

```c
./main teste1.txt 0
```

**Output:**

```c
Text = GCTCTCAAAATAAAGTACGGA$

Print SUS?
 1-Yes 2- No: 1
 i	SA	SUS	suffixes
0	21	0	$
1	20	0	A$
2	6	4	AAAATAAAGTACGGA$
3	11	4	AAAGTACGGA$
4	7	4	AAATAAAGTACGGA$
5	12	3	AAGTACGGA$
6	8	3	AATAAAGTACGGA$
7	16	2	ACGGA$
8	13	2	AGTACGGA$
9	9	2	ATAAAGTACGGA$
10	5	2	CAAAATAAAGTACGGA$
11	17	2	CGGA$
12	3	4	CTCAAAATAAAGTACGGA$
13	1	4	CTCTCAAAATAAAGTACGGA$
14	19	2	GA$
15	0	2	GCTCTCAAAATAAAGTACGGA$
16	18	2	GGA$
17	14	2	GTACGGA$
18	10	3	TAAAGTACGGA$
19	15	3	TACGGA$
20	4	3	TCAAAATAAAGTACGGA$
21	2	3	TCTCAAAATAAAGTACGGA$


```
