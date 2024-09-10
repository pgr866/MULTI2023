/******************************************************************************
* FILE: omp_bug6.c
* DESCRIPTION:
*   This program compiles and runs fine, but produces the wrong result.
*   Compare to omp_orphan.c.
* AUTHOR: Blaise Barney  6/05
* LAST REVISED: 06/30/05
* Fixed by LGCasado 05/10/23
*   omp parallel needs all variables in the scope of it body.
*   usinf a procedure omp parallel will be in the procedure because all 
*   procedure variables are local to procedire.
******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define VECLEN 100

//No global variables.

//--------------------------------------------------------------
void DotProd (float *a, float *b, float *pMainSum) 
{
 float DotProdSum=0.0;

 #pragma omp parallel default(none) shared(a,b,DotProdSum)
 {
  int tid = omp_get_thread_num();
  #pragma omp for reduction(+:DotProdSum) //DotProdSum here is local to DotProd
   for (int i=0; i < VECLEN; i++){
     DotProdSum = DotProdSum + (a[i]*b[i]);
     //To cjeck it easy. If next works move to previous
     //DotProdSum = DotProdSum + 1.0;
    printf("  tid= %d i=%d\n",tid,i);
   }
 }
 *pMainSum=DotProdSum;
}

//--------------------------------------------------------------
int main (int argc, char *argv[]) 
{
 float sum=0.0;
 float a[VECLEN], b[VECLEN];

 for (int i=0; i < VECLEN; i++)
  a[i] = b[i] = 1.0 * i;

 DotProd(a,b,&sum);

 printf("Sum = %f\n",sum);
 return 0;
}

