#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#define MAX 1000
int arr[MAX];
int n,i,thc;
long long gs=0;
pthread_mutex_t m;
void *partial_sum(void*rank){
   long r=(long)rank;
   int ln=n/thc;
   int f=r*ln;
   int l;
   if(r==thc - 1){
      l=n-1;
   }
   else{
      l=f+ln-1;
   }
   long long ls=0;
   printf("\nThread:%ld\nFirstindex:%d\nLastindex:%d\n",r,f,l);
   for(i=f;i<=l;i++){
      ls+=arr[i];
   }
   pthread_mutex_lock(&m);
   gs+=ls;
   pthread_mutex_unlock(&m);
   pthread_exit(NULL);
}
int main(){
   printf("--------SUM OF ARRAY OF N ELEMENTS USING PTHREADS--------\n");
   long t;
   pthread_t * thh;
   printf("Enter no.of elements:\n");
   scanf("%d",&n);
   printf("Enter the elements:\n");
   for(i=0;i<n;i++){
      scanf("%d",&arr[i]);
   }
   printf("Enter no.of threads:\n");
   scanf("%d",&thc);
   thh=malloc(thc*sizeof(pthread_t));
   pthread_mutex_init(&m,NULL);
   for(t=0;t<thc;t++){
      pthread_create(&thh[t],NULL,partial_sum,(void*)t);
   }
   for(t=0;t<thc;t++){
      pthread_join(thh[t],NULL);
   }
   pthread_mutex_destroy(&m);
   printf("Total Sum=%ld",gs);
   free(thh);
   return 0;
}
