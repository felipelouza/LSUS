#ifndef SUS_K_H
#define SUS_K_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"

#define lcp(i) ((i < n) ? (LCP[i]) : (0))



void IKXLSUS(unsigned char *T, uint_t *LSUS, uint_t *SA, uint_t *LCP, uint_t n);
void HTXLSUS(unsigned char *T, uint_t *A, uint_t *B, uint_t n);
void PLCPLSUS(unsigned char *T, uint_t *PLCP, uint_t *PHI, uint_t *LSUS, uint_t n);

bool equal(uint_t *v1, uint_t *v2, uint_t n);
void buildPLCP(uint_t *PLCP, uint_t *PHI, unsigned char *T, uint_t n); //9n bytes
void isa(int_t *ISA, int_t n, uint_t *SA);
void phi (int_t *PHI, int_t n, int_t *ISA, uint_t *SA);
void buildPHI(uint_t *PHI, uint_t n, uint_t *SA);
void print(uint_t *SA, uint_t *LSUS, unsigned char *T, uint_t n);
void lcp_plcp(uint_t *LCP, uint_t *PLCP, uint_t *SA, uint_t n);

#endif


