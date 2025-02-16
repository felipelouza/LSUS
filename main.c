/*
 * sus
 *
 * Authors: Larissa M. Aguiar and Felipe A. Louza
 * contact: louza@ufu.br
 * 05/06/2024
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "external/sacak-lcp.h"
#include "lib/lsus.h"
#include "lib/file.h"
#include "lib/utils.h"
#include "external/malloc_count/malloc_count.h"  

#define lcp(i) ((i < n) ? (LCP[i]) : (0))

#define WORD (size_t)(pow(256,sizeof(int_t)))

/*************************************/

int main(int argc, char *argv[]){

  time_t t_start=0, t_total=0;
  clock_t c_start=0, c_total=0;

  extern char *optarg;
  extern int optind, opterr, optopt;
  char *c_file=NULL;
  int c, alg = 3, check = 0, p = 0, time=0;
  while ((c = getopt(argc, argv, "A:kpct")) != -1){
    switch (c){
      case 'A':
        alg=(int)atoi(optarg);
        break;
      case 'p':
        p = 1;
        break;
      case 'c':
        check = 1;
        break;
      case 't':
        time=1;
        break;
      default:
        break;
    }
  }
  free(optarg);

  if(optind+1==argc) {
    c_file=argv[optind++];
  }

  /**/

  if(time) time_start(&t_start, &c_start);
  printf("## PREPROCESSING ##\n");
  int_t k=0;
  size_t n=0;
  unsigned char **R = (unsigned char**) file_load_multiple(c_file, &k, &n);

  if(n>pow(2,32) && (sizeof(int_t)==sizeof(int32_t))){
    fprintf(stderr, "####\n");
    fprintf(stderr, "ERROR: INPUT LARGER THAN %.1lf GB (%.1lf GB)\n", WORD/pow(2,30), n/pow(2,30));
    fprintf(stderr, "PLEASE USE %s-64\n", argv[0]);
    fprintf(stderr, "####\n");
    for(int_t i=0; i<k; i++) free(R[i]); 
    free(R);
    exit(EXIT_FAILURE);
  }


  //concatenate all string
  unsigned char *T =cat_char(R, k, &n);
  //printf("k = %" PRIdN "\n", k);
  printf("N = %zu bytes\n", n);
  printf("sizeof(int) = %zu bytes\n", sizeof(int_t)); 

  //free memory
  if (*R!=NULL){
    for(int_t i=0; i<k; i++) free(R[i]);
    free(R);
  }

  if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

  time_start(&t_total, &c_total);
  uint_t *SA = NULL;
  uint_t *LCP = NULL;
  uint_t *PHI = NULL;
  uint_t *PLCP = NULL;
  uint_t *LSUS = NULL;

  SA = (uint_t *)malloc((n + 1) * sizeof(uint_t));

  //IKXLSUS (TCS 2015)
  //T + SA + LCP + LSUS = 13n bytes
  if(alg == 1){
    LCP = (uint_t *)malloc((n + 1) * sizeof(uint_t));
    if(time) time_start(&t_start, &c_start);
    printf("## SACAK ##\n");
    sacak((unsigned char *)T, SA, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
    if(time) time_start(&t_start, &c_start);
    PHI = LCP;
    printf("## PHI ##\n");
    buildPHI(PHI, n, SA);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
    if(time) time_start(&t_start, &c_start);
    PLCP = (uint_t *)malloc((n + 1) * sizeof(uint_t));
    printf("## LCP ##\n");
    buildPLCP(PLCP, PHI, T, n);
    lcp_plcp(LCP, PLCP, SA, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
  }
  //HTXLSUS (TCS 2017)
  //T + SA + LSUS = 13n bytes
  if(alg==2){
    if(time) time_start(&t_start, &c_start);
    printf("## SACAK ##\n");
    sacak((unsigned char *)T, SA, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 
  }
  //PLCPLSUS 
  //T + SA (LSUS) + PHI (PLCP) = 9n bytes
  if(alg==3){
    if(time) time_start(&t_start, &c_start);
    printf("## SACAK ##\n");
    sacak((unsigned char *)T, SA, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
    if(time) time_start(&t_start, &c_start);
    PHI = (uint_t *)malloc((n + 1) * sizeof(uint_t));
    printf("## PHI ##\n");
    buildPHI(PHI, n, SA);//8n bytes 
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 
  }

  /**/

  if(time) time_start(&t_start, &c_start);
  switch (alg){
    case 1: printf("## IKX_LSUS ##\n");
            LSUS = PLCP;
            IKXLSUS(T, LSUS, SA, LCP, n); //13n bytes
            break;
    case 2: printf("## HTX_LSUS ##\n");
            LSUS = (uint_t *)malloc((n+1) * sizeof(uint_t));
            uint_t *A = SA;
            uint_t *B = LSUS;
            HTXLSUS(T, A, B, n); //9n bytes
            break;
    case 3: printf("## PLCP_LSUS ##\n");
            PLCP = PHI;
            LSUS = SA;
            PLCPLSUS(T, PLCP, PHI, LSUS, n); //9n bytes
            break;
    default:
            break;
  }
  if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

  printf("## TOTAL ##\n");
  fprintf(stderr,"%.6lf\n", time_stop(t_total, c_total));


  if(p ==1){
    for(int_t i=0; i<n; i++)
      if(T[i]!=1 && T[i]!=0)
        printf("LSUS[%" PRIdN "]: %" PRIdN "\t T[%" PRIdN "]: %c\n", i, LSUS[i], i, T[i]-1);
      else 
        printf("LSUS[%"PRIdN"]: %"PRIdN"\t T[%"PRIdN"]: %d\n", i, LSUS[i], i, T[i]);
  }

  //VALIDATION
  if (check == 1){
    uint_t* A = (uint_t *)malloc((n + 1) * sizeof(uint_t));
    int_t* B = (int_t *)malloc((n + 1) * sizeof(int_t));
    sacak_lcp((unsigned char *)T, A, B, n);

    uint_t *C = (uint_t *)malloc((n+1) * sizeof(uint_t));
    IKXLSUS(T, C, A, (uint_t*)B, n);
    if (!equal(LSUS, C, n)) printf("ERROR!\n");  
    else printf("OK!\n");  

    free(A); free(B); free(C);
  }

  if(alg == 1 || alg == 2) free(LSUS);
  if(alg == 1) free(LCP);
  if(alg == 3) free(PLCP);

  free(T);
  free(SA);

return 0;
}

