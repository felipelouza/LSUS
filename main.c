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

#include "external/gsacak.h"
#include "lib/sus.h"
#include "lib/file.h"
#include "lib/utils.h"
#include "external/malloc_count/malloc_count.h"  

#define lcp(i) ((i < n) ? (LCP[i]) : (0))


/*************************************/

int main(int argc, char *argv[]){

  time_t t_start=0;
  clock_t c_start=0;

  extern char *optarg;
  extern int optind, opterr, optopt;
  char *c_file=NULL;
  int c, alg = 0, comp = 0, pri = 0, time=0;
  while ((c = getopt(argc, argv, "A:kpct")) != -1){
    switch (c){
      case 'A':
        alg=(int)atoi(optarg);
        break;
      case 'p':
        pri = 1;
        break;
      case 'c':
        comp = 1;
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

  //================================
  //PREPROCESSING FASTA
  //================================

  if(time) time_start(&t_start, &c_start);
  printf("## PREPROCESSING ##\n");
  int_t k=0;
  size_t n=0;
  unsigned char **R = (unsigned char**) file_load_multiple(c_file, &k, &n);

  //concatenate all string
  unsigned char *T =cat_char(R, k, &n);
  printf("k = %" PRIdN "\n", k);
  printf("N = %zu bytes\n", n);
  printf("sizeof(int) = %zu bytes\n", sizeof(int_t)); 

  //free memory
  if (*R!=NULL){
    for(int_t i=0; i<k; i++)
      free(R[i]);
    free(R);
  }

  if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

  uint_t *SA = NULL;
  int_t *LCP = NULL;
  int_t *LCP1 = NULL;
  //int *ISA = NULL;
  int_t *PHI = NULL;
  int_t *PLCP = NULL;
  int_t *LSUS = NULL;
#if DEBUG
  printf("T = %s$\n\n", T);
#endif

  //================================
  //COMPUTING SA
  //================================
  SA = (uint_t *)malloc((n + 1) * sizeof(uint_t));

  //IKXSUS
  //T + SA + LCP = 9n bytes
  if(alg == 0){
    LCP = (int_t *)malloc((n + 1) * sizeof(int_t));
    if(time) time_start(&t_start, &c_start);
    printf("## SACAK_lcp ##\n");
    gsacak((unsigned char *)T, (uint_t *)SA, (int_t *)LCP, NULL, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
  }

  //IKXSUS (version 2)
  //T + SA + PLCP + LCP(PHI) = 13n bytes
  if(alg==1){ //OKAY
    if(time) time_start(&t_start, &c_start);
    printf("## SACAK ##\n");
    gsacak((unsigned char *)T, (uint_t *)SA, NULL, NULL, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
    PHI = (int_t *)malloc((n + 1) * sizeof(int_t));
    if(time) time_start(&t_start, &c_start);
    printf("## PHI ##\n");
    buildPHI(PHI, n, SA);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 
    PLCP= (int_t *)malloc((n + 1) * sizeof(int_t));
    if(time) time_start(&t_start, &c_start);
    printf("## LCP ##\n");
    buildPLCP(PLCP, PHI, T, n); //9n 
    LCP1 = (int_t *) PHI; //reuses LCP 
    lcp_plcp(LCP1, PLCP, SA, n); // 12n bytes
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
    free (PLCP); 
  }

  //SUS_1 e SUS_2 
  //T + SA (PLCP) + PHI = 9n bytes
  int_t sa_last=0;
  if(alg == 2 || alg == 3){

    if(time) time_start(&t_start, &c_start);
    printf("## SACAK ##\n");
    gsacak((unsigned char *)T, (uint_t *)SA, NULL, NULL, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

    sa_last = SA[n-1];
    PHI = (int_t *)malloc((n + 1) * sizeof(int_t));

    if(time) time_start(&t_start, &c_start);
    printf("## PHI ##\n");
    buildPHI(PHI, n, SA);//8n bytes 
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 

    PLCP = (int_t*) SA;

    if(time) time_start(&t_start, &c_start);
    printf("## PLCP ##\n");
    buildPLCP(PLCP, PHI, T, n);//9n
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 
  }

  //================================
  //ALGORITHMS
  //================================

  if(time) time_start(&t_start, &c_start);
  switch (alg){
    case 0: printf("## IKXSUS_1 ##\n");
            //4n bytes 
            LSUS = (int_t *)malloc((n+1) * sizeof(int_t));
            SUS_T(T, LSUS, n, LCP, SA); //13n bytes
            break;
    case 1: printf("## IKXSUS_2 ##\n");
            //4n bytes 
            LSUS = (int_t *)malloc((n+1) * sizeof(int_t));
            SUS_T(T, LSUS, n, LCP1, SA); //13n bytes
            break;
    case 2: printf("## SUS_1 ##\n");
            LSUS = PLCP;
            SUS_1(sa_last, n, LSUS, PHI, T);
            break;
    case 3: printf("## SUS_2 ##\n");
            LSUS=PHI;
            SUS_2(sa_last, n, PLCP, LSUS, T);
            break;
    default:
            break;
  }
  if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

  if(pri ==1){
    for(int_t i=0; i<=n; i++){
      if(T[i]!=1 && T[i]!=0){
        printf("LSUS[%" PRIdN "]: \t %" PRIdN "\t T[%" PRIdN "]:\t %c\n", i, LSUS[i], i, T[i]-1);
      }
      else if(T[i]==1)
        printf("LSUS[%"PRIdN"]: \t %"PRIdN"\t T[%"PRIdN"]:\t %d\n", i, LSUS[i], i, 1);
      else if(T[i]==0)
        printf("LSUS[%"PRIdN"]: \t %"PRIdN"\t T[%"PRIdN"]:\t %d\n", i, LSUS[i], i, 0);
    }
  }
  //VALIDATION
  if (comp == 1){
    int_t* LCP2 = (int_t *)malloc((n + 1) * sizeof(int_t));
    uint_t* SA2 = (uint_t *)malloc((n + 1) * sizeof(uint_t));
    gsacak((unsigned char *)T, (uint_t *)SA2, (int_t *)LCP2, NULL, n);
    int_t *SUS2 = (int_t *)malloc((n+1) * sizeof(int_t));
    SUS_T(T, SUS2, n, LCP2, SA2);
    if (equal(LSUS, SUS2, n))
      printf("LSUS and SUST are equal :)\n");  
    free(SUS2);
    free(LCP2); 
    free(SA2);
  }

  if(alg == 0){
    free(LCP);
  }
  if(alg == 0 || alg == 1|| alg==5 || alg == 6){
    free(LSUS);
  }

  free(T);
  free(SA);
  if(alg==2 || alg==1 || alg==3 || alg==5 || alg==6)
  {
    free(PHI);
  }

}

