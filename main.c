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

//#include "external/gsacak.h"
#include "external/sacak-lcp.h"
#include "lib/sus.h"
#include "lib/file.h"
#include "lib/utils.h"
#include "external/malloc_count/malloc_count.h"  

#define lcp(i) ((i < n) ? (LCP[i]) : (0))


/*************************************/

int main(int argc, char *argv[]){

  time_t t_start=0, t_total=0;
  clock_t c_start=0, c_total=0;

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
  //printf("k = %" PRIdN "\n", k);
  printf("N = %zu bytes\n", n);
  printf("sizeof(int) = %zu bytes\n", sizeof(int_t)); 

  //free memory
  if (*R!=NULL){
    for(int_t i=0; i<k; i++){
      free(R[i]);
    }
    free(R);
  }

  if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

  if(time) time_start(&t_total, &c_total);
  uint_t *SA = NULL;
  int_t *LCP = NULL;
  //int *ISA = NULL;
  int_t *PHI = NULL;
  int_t *PLCP = NULL;
  int_t *LSUS = NULL;

  SA = (uint_t *)malloc((n + 1) * sizeof(uint_t));

  //IKXSUS (TCS 2015)
  //T + SA + LCP = 9n bytes
  if(alg == 1){
    LCP = (int_t *)malloc((n + 1) * sizeof(int_t));
    if(time) time_start(&t_start, &c_start);
    printf("## SACAK_lcp ##\n");
    sacak_lcp((unsigned char *)T, (uint_t *)SA, (int_t *)LCP, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));
  }
  //HTXSUS (TCS 2017)
  //T + SA = 5n bytes
  if(alg==2){
    if(time) time_start(&t_start, &c_start);
    printf("## SACAK ##\n");
    sacak((unsigned char *)T, (uint_t *)SA, n);
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 
  }

  //SUS_1 e SUS_2 
  //T + SA (PLCP) + PHI = 9n bytes
  int_t sa_last=0;
  if(alg == 3 || alg == 4 || alg == 5 || alg == 6){

    if(time) time_start(&t_start, &c_start);
    printf("## SACAK ##\n");
    //gsacak((unsigned char *)T, (uint_t *)SA, NULL, NULL, n);
    sacak((unsigned char *)T, (uint_t *)SA, n);
    //if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));

    sa_last = SA[n-1];
    PHI = (int_t *)malloc((n + 1) * sizeof(int_t));

    //if(time) time_start(&t_start, &c_start);
    printf("## PHI ##\n");
    buildPHI(PHI, n, SA);//8n bytes 
    //if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 

    PLCP = (int_t*) SA;

    //if(time) time_start(&t_start, &c_start);
    printf("## PLCP ##\n");
    buildPLCP(PLCP, PHI, T, n);//9n
    if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start)); 
  }

  //================================
  //ALGORITHMS
  //================================

  if(time) time_start(&t_start, &c_start);
  switch (alg){
    case 1: printf("## IKXSUS ##\n");
            //4n bytes 
            LSUS = (int_t *)malloc((n+1) * sizeof(int_t));
            IKXSUS(T, LSUS, n, LCP, SA); //13n bytes
            break;
    case 2: printf("## HTXSUS ##\n");
            LSUS = (int_t *)malloc((n+1) * sizeof(int_t));
            int_t *A = (int_t*) SA;
            int_t *B = LSUS;
            HTXSUS(T, A, B, n); //9n bytes
            break;
    case 3: printf("## LSUS_13n_v1 ##\n");
            LSUS = (int_t *)malloc((n+1) * sizeof(int_t));
            LSUS13_1(T, PLCP, PHI, LSUS, n); //13n bytes
            break; 
    case 4: printf("## LSUS_13n_v2 ##\n");
            LSUS = (int_t *)malloc((n+1) * sizeof(int_t));
            LSUS13_2(T, PLCP, PHI, LSUS, n); //13n bytes
            break; 
    case 5: printf("## LSUS_9n_v1 ##\n");
            LSUS = PLCP;
            LSUS9_1(T, LSUS, PHI, sa_last, n); //13n bytes
            break;
    case 6: printf("## LSUS_9n_v2 ##\n");
            LSUS=PHI;
            LSUS9_2(T, PLCP, LSUS, sa_last, n); //13n bytes
            break;
    default:
            break;
  }
  if(time) fprintf(stderr,"%.6lf\n", time_stop(t_start, c_start));



  if(time) printf("## TOTAL ##\n");
  if(time) fprintf(stderr,"%.6lf\n", time_stop(t_total, c_total));


  if(pri ==1){
    for(int_t i=0; i<=n; i++){
      if(T[i]!=1 && T[i]!=0){
        printf("LSUS[%" PRIdN "]: \t %" PRIdN "\t T[%" PRIdN "]:\t %c\n", i, LSUS[i]-1, i, T[i]-1);
      }
      else if(T[i]==1)
        printf("LSUS[%"PRIdN"]: \t %"PRIdN"\t T[%"PRIdN"]:\t %d\n", i, LSUS[i]-1, i, 1);
      else if(T[i]==0)
        printf("LSUS[%"PRIdN"]: \t %"PRIdN"\t T[%"PRIdN"]:\t %d\n", i, LSUS[i]-1, i, 0);
    }
  }
  //VALIDATION
  if (comp == 1){

    int_t* LCP2 = (int_t *)malloc((n + 1) * sizeof(int_t));
    uint_t* SA2 = (uint_t *)malloc((n + 1) * sizeof(uint_t));
    sacak_lcp((unsigned char *)T, (uint_t *)SA2, (int_t *)LCP2, n);

    int_t *SUS2 = (int_t *)malloc((n+1) * sizeof(int_t));
    IKXSUS(T, SUS2, n, LCP2, SA2);
    if (!equal(LSUS, SUS2, n))
      printf("ERROR!\n");  
    else
      printf("OK!\n");  

    free(SUS2); free(LCP2); free(SA2);
  }

  if( alg == 1 || alg == 2 || alg==3 || alg == 4){
    free(LSUS);
  }

  free(T);
  free(SA);
  if(alg==5 || alg==6)
    free(PHI);

return 0;
}

