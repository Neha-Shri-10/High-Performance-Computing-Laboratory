#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
int thread_count;
void* hello(void* rank);
int main(int argc,char* argv[]){
   printf("--------PRINT HELLO WORLD USING PTHREADS---------\n");
   long t;
   pthread_t* thread_handles;
   thread_count=strtol(argv[1],NULL,10);
   thread_handles=malloc(thread_count*sizeof(pthread_t));
	 for(t=0; t<thread_count; t++){
	 pthread_create(&thread_handles[t],NULL,hello,(void*)t);
	 }
   printf("Hello from main thread\n");
   for(t=0;t<thread_count;t++){
      pthread_join(thread_handles[t],NULL);
   }
   free(thread_handles);
   return 0;
   }
void* hello(void* rank){
   long my_rank=(long)rank;
   printf("Hello from thread %ld of %d\n",my_rank,thread_count);
   return NULL;
}


