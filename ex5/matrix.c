#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define MAX 10
int A[MAX][MAX],B[MAX][MAX];
int sum[MAX][MAX],res[MAX][MAX];
int r1,c1,r2,c2;
void *add(void* arg){
   int i,j;
   for(i=0;i<r1;i++){
      for(j=0;j<c1;j++){
	 sum[i][j]=A[i][j]+B[i][j];
      }
   }
   printf("**Addition completed by thread**\n");
   pthread_exit(NULL);
}
void *mul(void* arg){
   int i,j,k;
   for(i=0;i<r1;i++){
      for(j=0;j<c2;j++){
	 res[i][j]=0;
	 for(k=0;k<c1;k++){
	    res[i][j]+=A[i][k]*B[k][j];
	 }
      }
   }
   printf("**Multiplication completed by thread**\n");
   pthread_exit(NULL);
}
int main(){
   printf("-------MATRIX ADDITION AND MULTIPLICATION USING PTHREADS-------\n");
   pthread_t t1,t2;
   int i,j;
   printf("Enter rows and columns of matrix A:\n");
   scanf("%d %d",&r1,&c1);
   printf("Enter rows and columns of matrix B:\n");
   scanf("%d %d",&r2,&c2);
   if(c1!=r2){
      printf("\nMatrix multiplication not possible");
      return 0;
   }
   printf("Enter element of matrix A:\n");
   for(i=0;i<r1;i++){
      for(j=0;j<c1;j++){
	 scanf("%d",&A[i][j]);
      }
   }
   printf("Enter element of matrix B:\n");
   for(i=0;i<r2;i++){
      for(j=0;j<c2;j++){
	 scanf("%d",&B[i][j]);
      }
   }
   pthread_create(&t1,NULL,add,NULL);
   pthread_create(&t2,NULL,mul,NULL);
   pthread_join(t1,NULL);
   pthread_join(t2,NULL);
   printf("Matrix Addition Result:\n");
   for(i=0;i<r1;i++){
      for(j=0;j<c1;j++){
	 printf("%d ",sum[i][j]);
      }
      printf("\n");
   }
   printf("Matrix Multiplication Result:\n");
   for(i=0;i<r1;i++){
      for(j=0;j<c2;j++){
	 printf("%d ",res[i][j]);
      }
      printf("\n");
   }
   return 0;
}
