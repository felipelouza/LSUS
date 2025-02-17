#include "lsus.h"

#define lcp(i) ((i < n) ? (LCP[i]) : (0))

void print(uint_t *A, uint_t *B, unsigned char *T, uint_t n){
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

bool equal(uint_t *v1, uint_t *v2, uint_t n){

  for (uint_t i = 0; i <n; i++){
    if (v1[i] != v2[i]){
      printf("Values are different in %" PRIdN " LSUS: %" PRIdN " \t LSUS: %" PRIdN "\n", i, v1[i], v2[i]);
      return false;
    }
  }
  return true;
}

void lcp_plcp(uint_t *LCP, uint_t *PLCP, uint_t *SA, uint_t n){
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

void buildPHI(uint_t *PHI, uint_t n, uint_t *SA){
  PHI[SA[0]] = n;
  SA[n] = n;
  for(uint_t i = 1; i<=n; i++)
    PHI[SA[i]] = SA[i-1];
}

/****************************************/

void buildPLCP(uint_t *PLCP, uint_t *PHI, unsigned char *T, uint_t n){ //9n bytes
  uint_t k = 0;
  int_t l = 0;
  for (uint_t i = 0; i <= n; i++){
    k = PHI[i];
    if (k != n){
      while (k+l<n && i+l<n && T[k + l] == T[i + l]){
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

void IKXLSUS(unsigned char *T, uint_t *LSUS, uint_t *SA, uint_t *LCP, uint_t n){

  for (uint_t i = 0; i < n; i++)
    LSUS[i] = 0;

  for (uint_t i = 0; i < n; i++){
    uint_t cur = 1+max(lcp(i), lcp(i + 1));
    if (n-SA[i] > cur)
      LSUS[SA[i]] = cur;
  }
}

/****************************************/


void HTXLSUS(unsigned char *T, uint_t *A, uint_t *B, uint_t n){

  uint_t *SA = A;
  uint_t *LSUS= B;
  uint_t *ISA = B;

  for (uint_t i = 0; i < n; i++)
    ISA[SA[i]] = i;

  //print(SA, ISA, T, n);

  uint_t x=0, y=0;

  for (uint_t i = 0; i < n; i++){
    if(ISA[i]>0){
      uint_t j = SA[ISA[i]-1];
      while(i+x<n && j+x<n && T[i+x] == T[j+x]) x++; // && T[i+x] != 0 && T[i+x] != 1
    }
    else{
      x=0;
    }
    if(ISA[i] < n-1){
      uint_t j = SA[ISA[i]+1];
      while(i+y<n && j+y<n && T[i+y] == T[j+y]) y++;
    }
    else{
      y = 0;
    }
    if(i+max(x,y)+1 < n)
      LSUS[i] = max(x,y)+1;
    else{
      for(uint_t j=i; j<n; j++)
        LSUS[j]=0;
      break;
    }
    if(x>0) x--;
    if(y>0) y--;
  }

  LSUS[n]=0;
}

/****************************************/

void PLCPLSUS(unsigned char *T, uint_t *PLCP, uint_t *PHI, uint_t *LSUS, uint_t n){

  //PLCP points to PHI
  for (uint_t i = 0; i <= n; i++) LSUS[i]=U_MAX; //U_MAX as -1

  //PLCP[n] = 0;
  int_t l = 0;
  uint_t k = 0, cur=0;

  for (uint_t i = 0; i <= n; i++){
    k = PHI[i];
    if (k != n){
      //current LCP
      while (k+l<n && i+l<n && T[k + l] == T[i + l]){
        l++;
      }

      if (LSUS[i]!= U_MAX){
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
