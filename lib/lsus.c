#include "lsus.h"

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

/****************************************/

bool equal(int_t *v1, int_t *v2, int_t tam){

  for (int_t i = 0; i <tam; i++){
    if (v1[i] != v2[i]){
      printf("LSUS and SUST are different in %" PRIdN " LSUS: %" PRIdN " \t LSUS: %" PRIdN "\n", i, v1[i], v2[i]);
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
  for (int_t i = 0; i <= n; i++){
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
      while (T[k + l] == T[i + l]){
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
void LSUS13_1(unsigned char *T, int_t *PLCP, int_t *PHI, int_t *LSUS, int_t n){
  
  for (int_t i = 0; i < n; i++)
    LSUS[i] = 0;

  //int_t pos=n;
  int_t k, cur;
  //until n because SA[n]=n and PHI[n]=SA[n-1] (last suffix in lexorder)
  for (int_t i = 0; i <= n; i++){
    k = PHI[i]; 
    //if(k>pos) continue;
    cur = 1+max(PLCP[i], PLCP[k]); 
    if (n - k > cur)
      LSUS[k] = cur;
    //else
      //pos = min(pos, k); 
  }
}
/****************************************/
void LSUS13_2(unsigned char *T, int_t *PLCP, int_t *PHI, int_t *LSUS, int_t n){

  //n--;
  int_t p, cur, i;
  for (i = 0; i < n; i++){
    p=PHI[i];
    //if (p!=n) LSUS[p] = PLCP[i];
    LSUS[p] = PLCP[i];
  }

  for (i = 0; i <n; i++){
    cur = 1+max(PLCP[i], LSUS[i]);
    if (n - i > cur)
      LSUS[i] = cur;
    else
      LSUS[i] = 0;
  }
}
/****************************************/

void LSUS9_1(unsigned char *T, int_t *PLCP, int_t *PHI, int_t sa_last, int_t n){

  int_t *LSUS = PLCP;
  int_t k, cur, aux, i;
  //PHI[n]=sa_last; 
  k=PHI[0];// sufixo que antecede o sufixo i 
  aux=PLCP[k];

  int_t c=0;
  while(c<=n){
    i=PHI[k];
    cur =  1+max(PLCP[i], aux); 
    aux=PLCP[i];
    if (n - i > cur)
      LSUS[i] = cur;
    else LSUS[i]=0;
    k=i; 
    c++;
  }

}
/****************************************/

void LSUS9_2(unsigned char *T, int_t *PLCP, int_t *PHI, int_t sa_last, int_t n){
  int_t cur, i, k, aux, cont=0;  
  //PHI[n]=sa_last; 
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
    cur = 1+max(PLCP[i], PHI[i]);
    if (n - i > cur)
      PHI[i] = cur;
    else
      PHI[i] = 0;
  }
}

/****************************************/

void IKXSUS(unsigned char *T, int_t *LSUS, int_t n, int_t *LCP, uint_t *SA){

  for (int_t i = 0; i < n; i++)
    LSUS[i] = 0;

  for (int_t i = 0; i < n; i++){
    int_t cur = 1+max(lcp(i), lcp(i + 1));
    if (n-SA[i] > cur)
      LSUS[SA[i]] = cur;
  }
}

/****************************************/


void HTXSUS(unsigned char *T, int_t *A, int_t *B, int_t n){

  int_t *SA = A;
  int_t *LSUS= B;
  int_t *ISA = B;

  for (int_t i = 0; i < n; i++)
    ISA[SA[i]] = i;

  //print(SA, ISA, T, n);

  int_t x=0, y=0;

  for (int_t i = 0; i < n; i++){
    if(ISA[i]>0){
      int_t j = SA[ISA[i]-1];
      while(T[i+x] == T[j+x]) x++; // && T[i+x] != 0 && T[i+x] != 1
    }
    else{
      x=0;
    }
    if(ISA[i] < n-1){
      int_t j = SA[ISA[i]+1];
      while(T[i+y] == T[j+y]) y++;
    }
    else{
      y = 0;
    }
    if(i+max(x,y)+1 < n)
      LSUS[i] = max(x,y)+1;
    else{
      for(int_t j=i; j<n; j++)
        LSUS[j]=0;
      break;
    }
    if(x>0) x--;
    if(y>0) y--;
  }

  LSUS[n]=0;
}

/****************************************/

void PLCPSUS(unsigned char *T, int_t *PLCP, int_t *PHI, int_t *LSUS, int_t n){

  //PLCP points to PHI
  for (int_t i = 0; i <= n; i++) LSUS[i]=-1;

  //PLCP[n] = 0;
  int_t l = 0, k = 0, cur=0;

  for (int_t i = 0; i <= n; i++){
    k = PHI[i];
    if (k != n){
      //current LCP
      while (T[k + l] == T[i + l]){
        l++;
      }

      if (LSUS[i]!= -1){
        cur = 1+max(l, LSUS[i]) ;
        if (n - i > cur) LSUS[i] = cur;
        else LSUS[i]=0;
      }
      else
        LSUS[i] = l;

      //if (PLCP[k]>= 0){
      if (k < i){ //PLCP of S_PHI[i] has been computed?
        cur = 1+max(LSUS[k], l);
        if (n - k  > cur) LSUS[k] = cur;
        else LSUS[k]=0;
      }
      else
        LSUS[k] = l;

      l = max((l - 1), 0);
    }
    else LSUS[i]= 0; //(LSUS[i]==0)?1:0; //max((LSUS[i]),0);
  }

  //LSUS[n-1]=LSUS[n]=0;
}
