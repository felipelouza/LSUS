#ifndef SUS_K_H
#define SUS_K_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

#define lcp(i) ((i < n) ? (LCP[i]) : (0))



void IKXSUS(unsigned char *T, int_t *SUS, int_t n, int_t *LCP, uint_t *SA);
void HTXSUS(unsigned char *T, int_t *A, int_t *B, int_t n);

void LSUS13_1(unsigned char *T, int_t *PLCP, int_t *PHI, int_t *SUS, int_t n);
void LSUS13_2(unsigned char *T, int_t *PLCP, int_t *PHI, int_t *SUS, int_t n);
void LSUS9_1(unsigned char *T, int_t *PLCP, int_t *PHI, int_t sa_last, int_t n);
void LSUS9_2(unsigned char *T, int_t *PLCP, int_t *PHI, int_t sa_last, int_t n);

bool equal(int_t *v1, int_t *v2, int_t tam);
void buildPLCP(int_t *PLCP, int_t *PHI, unsigned char *T, int_t n); //9n bytes
void isa(int_t *ISA, int_t n, uint_t *SA);
void phi (int_t *PHI, int_t n, int_t *ISA, uint_t *SA);
void buildPHI(int_t *PHI, int_t n, uint_t *SA);
void print(uint_t *SA, int_t *SUS, unsigned char *T, int_t n);
void lcp_plcp(int_t *LCP, int_t *PLCP, uint_t *SA, int_t n);

#endif


