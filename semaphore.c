#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define TOTAL_TASKS 10 

typedef struct {
    int a, b;
    char op;
} Work;

Work work_buffer[BUFFER_SIZE];
int in = 0, out = 0;
int tasks_completed = 0;
int shutdown = 0;

sem_t empty, full, mutex;
FILE *result_file;

void* compute(void* arg) {
    char my_op = *(char*)arg;
    pthread_t tid = pthread_self();

    while (1) {
        // 1. Wait for work OR a shutdown signal
        sem_wait(&full);
        sem_wait(&mutex);

        // Check if it's time to quit
        if (shutdown && tasks_completed >= TOTAL_TASKS) {
            sem_post(&mutex);
            sem_post(&full); // Pass the wake-up to the next thread
            break;
        }

        Work job = work_buffer[out];

        if (job.op == my_op) {
            out = (out + 1) % BUFFER_SIZE;
            tasks_completed++;
            
            double res = 0;
            if (my_op == '+') res = job.a + job.b;
            else if (my_op == '-') res = job.a - job.b;
            else if (my_op == '*') res = job.a * job.b;
            else if (my_op == '/') res = (double)job.a / job.b;

            // SAVE TO FILE
            fprintf(result_file, "[Thread %lu] %d %c %d = %.2f\n", (unsigned long)tid, job.a, my_op, job.b, res);
            fflush(result_file);
            printf("Thread [%lu] processed %c and saved to file.\n", (unsigned long)tid, my_op);

            if (tasks_completed >= TOTAL_TASKS) shutdown = 1;

            sem_post(&mutex);
            sem_post(&empty);
        } else {
            // Not my operator: put it back and let someone else check
            sem_post(&mutex);
            sem_post(&full); 
            usleep(1000); 
            if (shutdown) break;
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    result_file = fopen("result.txt", "w");
    
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_t threads[4];
    char ops[4] = {'+', '-', '*', '/'};

    for (int i = 0; i < 4; i++) 
        pthread_create(&threads[i], NULL, compute, &ops[i]);

    // Producer Loop
    for (int i = 0; i < TOTAL_TASKS; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);

        work_buffer[in] = (Work){rand() % 100, rand() % 99 + 1, ops[rand() % 4]};
        in = (in + 1) % BUFFER_SIZE;

        sem_post(&mutex);
        sem_post(&full);
    }

    // CRITICAL: Wake up threads so they see the shutdown flag
    sem_wait(&mutex);
    shutdown = 1;
    sem_post(&mutex);
    sem_post(&full); 

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL); // Now threads can actually exit
    }

    fclose(result_file);
    printf("Processing complete. Check result.txt for details.\n");
    return 0;
}


