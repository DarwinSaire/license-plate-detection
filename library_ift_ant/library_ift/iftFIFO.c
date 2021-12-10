#include "iftFIFO.h"

iftFIFO *iftCreateFIFO(int n)
{
  iftFIFO *F=(iftFIFO *)calloc(1,sizeof(iftFIFO));

  F->FIFO  = iftAllocIntArray(n);
  F->color = iftAllocCharArray(n);
  F->n     = n;
  F->first=F->last=0;

  return(F);
}

void     iftDestroyFIFO(iftFIFO **F)
{
  iftFIFO *aux=*F;

  if (aux != NULL) {
    free(aux->FIFO);
    free(aux->color);
    free(aux);
    *F = NULL;
  }
}

char iftInsertFIFO(iftFIFO *F, int node)
{  
  if (iftFullFIFO(F)){
    iftWarning("FIFO is full","iftInsertFIFO");
    return 0;
  }
  F->color[node]=GRAY;
  F->FIFO[F->last]=node;  F->last++;

  return 1;
}

int      iftRemoveFIFO(iftFIFO *F)
{
  int node=NIL;

  if (!iftEmptyFIFO(F)){
    node = F->FIFO[F->first];  F->first++;
    F->color[node]=BLACK;
  }else{
    iftWarning("FIFO is empty","iftRemoveFIFO");
  }

 return node;
}

char     iftEmptyFIFO(iftFIFO *F)
{
  if (F->first == F->last) {
    iftResetFIFO(F);
    return(1);
  }else
    return(0);
}

char     iftFullFIFO(iftFIFO *F)
{
  if (F->last==F->n) {
    return(1);
  }else
    return(0);
}

void     iftResetFIFO(iftFIFO *F)
{
  memset(F->color,WHITE,F->n);
  F->first=F->last=0;
}
