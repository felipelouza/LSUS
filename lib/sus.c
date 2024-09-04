
#include "sus.h"

#define lcp(i) ((i < n) ? (LCP[i]) : (0))

void print(uint_t *A, int_t *B, unsigned char *T, int_t n){
  printf("i\tA\tB\tSuffixes\n");
  for (int_t i = 0; i < n; ++i)
  {
    printf("%" PRIdN "\t%" PRIdN "\t%" PRIdN "\t", i, A[i], B[i]);
    for (int_t j = A[i]; j < n; ++j)
    {
      if(T[j]==0) printf("#");
      else if (T[j]==1){
        printf("$");
        break;
      }
      else printf("%c", T[j]-1);
    }
    printf("\n");
  }
}

void print_sus(uint_t *A, int_t *B, unsigned char *T, int_t n){
  printf("i\tA\tB\tSuffixes\n");
  for (int_t i = 0; i <=n; ++i)
  {
    printf("%" PRIdN "\t%" PRIdN "\t%" PRIdN "\t", i, A[i], B[A[i]]);
    for (int_t j = A[i]; j < B[i]; ++j)
    {
      if(T[j]==0) printf("#");
      else if (T[j]==1){
        printf("$");
        break;
      }
      else printf("%c", T[j]-1);
    }
    printf("\n");
  }
}

/****************************************/

bool equal(int_t *v1, int_t *v2, int_t tam){

  for (int_t i = 0; i <tam; i++){
    if (v1[i] != v2[i]){
      printf("SUS and SUST are different in %" PRIdN " SUS: %" PRIdN " \t SUS_T: %" PRIdN "\n", i, v1[i], v2[i]);
      return false;
    }
  }
  return true;
}

void lcp_plcp(int_t *LCP, int_t *PLCP, uint_t *SA, int_t n){
  for(int_t i=0; i<=n; i++) LCP[i]=PLCP[SA[i]];
}

void isa(int_t *ISA, int_t n, uint_t *SA){
  SA[n] = n;
  for (int_t i = 0; i <= n; i++) ISA[SA[i]] = i;
}

/****************************************/

void phi(int_t *PHI, int_t n, int_t *ISA, uint_t *SA){
  isa(ISA, n, SA);
  for (int_t i = 0; i <= n; i++)
  {
    if (ISA[i] != 0)
      PHI[i] = SA[ISA[i] - 1];
    else
      PHI[i] = n;
  }
}

void buildPHI(int_t *PHI, int_t n, uint_t *SA)
{
  PHI[SA[0]] = n;
  SA[n] = n;
  int_t i;
  for(i = 1; i<=n; i++)
    PHI[SA[i]] = SA[i-1];

}

/****************************************/

void buildPLCP(int_t *PLCP, int_t *PHI, unsigned char *T, int_t n){ //9n bytes
  int_t l = 0, k = 0;
  for (int_t i = 0; i <= n; i++){
    k = PHI[i];
    if (k != n){
      while (T[k + l] == T[i + l] && T[k+l]!=1){
        l++;
      }
      PLCP[i] = l;
      l = max((l - 1), 0);
    }
    else
      PLCP[i] = 0;
  }
  PLCP[n] = 0;
}

/****************************************/

void LSUS_1(int_t sa_last, int_t n, int_t *PLCP, int_t *PHI, unsigned char *T){

  int_t k, cur, aux, i;
  PHI[n]=sa_last; 
  k=PHI[0];// sufixo que antecede o sufixo i 
  aux=PLCP[k];

  for(int_t cont=0; cont<=n; cont++){
    i=PHI[k];
    cur =  max(PLCP[i], aux); // tamanho da subcadeia 
    aux=PLCP[i];
    if (n - i >= cur && T[i+cur] != 1 && T[i+cur]!=0)
      PLCP[i] = cur+1;
    else PLCP[i]=0;
    k=i; 
  }

}
/****************************************/

void LSUS_2(int_t sa_last, int_t n, int_t *PLCP, int_t *PHI, unsigned char *T){
  int_t cur, i, k, aux, cont=0;  
  PHI[n]=sa_last; 
  k=PHI[0];// sufixo que antecede o sufixo j
  aux=0;
  i=0;
  while(cont<=n){
    aux=PHI[k]; //quem está sendo atualizado e o próximo a ser inicializado?
    PHI[k]= PLCP[i]; //na primeira iteração isso é o plcp de 0
    i=k;
    k=aux;
    //printf("%d\t", aux);
    cont++; 
  }
  for (i = 0; i <= n; i++){
    cur = max(PLCP[i], PHI[i]);
    if (n - i >= cur && T[i+cur]!= 1 && T[i+cur]!=0)
      PHI[i] = cur+1;
    else
      PHI[i] = 0;
  }
}

/****************************************/

void LSUS_T(unsigned char *T, int_t *SUS, int_t n, int_t *LCP, uint_t *SA){

  //SUS[SA[0]]=0; // O sufixo em SA[0] não é atualizado no loop, pq?
  for (int_t i = 0; i < n; i++){
    int_t cur = max(lcp(i), lcp(i + 1))+1;

    if (T[SA[i]+cur-1] != 1 && T[SA[i]+cur-1]!=0 && cur+SA[i]<=n-1) //separator == 1
      SUS[SA[i]] = cur;
    else 
      SUS[SA[i]]=0;
  }
}

/****************************************/
//A == LSUS
void SUS_COVER(int_t *A, int_t *B, int_t n){

  int_t *LSUS=A;
  for(int_t i=0; i<n; i++){
    //printf("%d ", LSUS[i]);
    int min = I_MAX;
    int l=0,r=0;
    for(int_t j=i; j>=0; j--){
      if(j+LSUS[j]>i && LSUS[j]<min){
        min = LSUS[j];
        l = j;
        r = j+LSUS[j];
      }
    }
    A[i]=l;
    B[i]=r;
  }

}
