#include <stdio.h>
#include <stdlib.h>

#define KNL_TYPE char
#include "knobloc.h"

#define NTST (1000)

KNL_TYPE *arr[NTST];


int main(void)
{
  int i,x,j;
  struct knl_head h;
  char *p;
  int nblock=NTST/2;
  int blocksize=100;
  int nal;
  
  p=calloc(nblock,blocksize);
  knl_init(&h,p,nblock,blocksize);

  srand(12);
  for(i=0;i<NTST;i++)
  {
    arr[i]=KNL_ALLOC(&h);
    if(arr[i]==NULL&&i<nblock) printf("arr[%d] failed but should be successful\n",i);
    else if(arr[i]!=NULL&&i>=nblock) printf("arr[%d] not failed but it should\n",i);
  }
  if(!KNL_ISEMPTY(&h)) printf("***pool should be empty here\n");
  for(i=0;i<NTST;i++)
  {
    if(NULL!=arr[i]) KNL_FREE(&h,arr[i]);
    arr[i]=NULL;
  }  
  if(KNL_ISEMPTY(&h)) printf("***pool should not be empty here\n");
  nal=0;
  for(i=0;i<10000000;i++)
  {
    x=rand()%((NTST/4)*3);
    if(arr[x]==NULL)
    { 
      arr[x]=KNL_ALLOC(&h);
      if(arr[x]!=NULL)
      {
        for(j=0;j<blocksize;j++) arr[x][j]='a'+(x%26);
        nal++; 
      }
    }
    else 
    { 
      int fail=0;
      for(j=0;j<blocksize;j++) if(arr[x][j]!='a'+(x%26)) fail++;
      if(fail!=0) printf("***Memcheck failed for arr[%d]\n",x);
      if(rand()%2==0)
      {
        arr[x]=KNL_FREE(&h,arr[x]); 
        --nal; 
      }
    }
    if(nal>nblock) printf("***More blocks allocated than available!\n");
  }

  free(p);
  
  return(0);
}
