#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_WORDS 10         
#define NUM_THREADS 3        
#define DICT_SIZE 10
const char *dictionary[DICT_SIZE] ={"python", "javascript", "golang", "rust", "docker", 
    "kubernetes", "react", "angular", "mongodb", "linux"};
char work_buffer[MAX_WORDS][50];
int work_count = 0;
int input_done = 0; 
char result_buffer[MAX_WORDS][100];
int result_count = 0;
pthread_mutex_t work_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t work_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t result_mutex = PTHREAD_MUTEX_INITIALIZER;
int check_dictionary(const char *word) {
    for (int i = 0; i < DICT_SIZE; i++) {
        if (strcmp(word, dictionary[i]) == 0) {
            return 1; 
        }
    }
    return 0; 
}
void *spell_check_worker(void *arg) {
    long id = (long)arg;    
    while (1) {
        char word[50];
        pthread_mutex_lock(&work_mutex);       
        while (work_count == 0 && !input_done) {
            pthread_cond_wait(&work_cond, &work_mutex);
        }
        if (work_count == 0 && input_done) {
            pthread_mutex_unlock(&work_mutex);
            break; // Exit the thread
        }
        strcpy(word, work_buffer[--work_count]);
        pthread_mutex_unlock(&work_mutex);
        int is_correct = check_dictionary(word);   
        usleep(100000); 
        pthread_mutex_lock(&result_mutex);
        sprintf(result_buffer[result_count], "Thread %ld checked '%s': %s",id, word, is_correct ? "CORRECT" : "INCORRECT");
        result_count++;
        pthread_mutex_unlock(&result_mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    char user_input[50];
    int num_to_check;

    printf("--- SPELLCHECK ---\n");
    printf("Dictionary: python, javascript, golang, rust, docker, kubernetes, react, angular, mongodb, linux\n");

    // Start worker threads
    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, spell_check_worker, (void*)i);
    }

    printf("Enter the number of words to check:");
    scanf("%d", &num_to_check);
    getchar(); // Consume the newline character left by scanf

    for (int i = 0; i < num_to_check; i++) {
        printf("Enter word %d: ", i + 1);
        fgets(user_input, 50, stdin);
        user_input[strcspn(user_input, "\n")] = 0; // Remove trailing newline

        pthread_mutex_lock(&work_mutex);
        
        // Safety check for buffer capacity
        if (work_count < MAX_WORDS) {
            strcpy(work_buffer[work_count], user_input);
            work_count++;
            printf("Master added: %s\n", user_input);
            pthread_cond_signal(&work_cond);
        } else {
            printf("Buffer full! Could not add: %s\n", user_input);
        }
        
        pthread_mutex_unlock(&work_mutex);
    }

    // Signal workers that no more input is coming
    pthread_mutex_lock(&work_mutex);
    input_done = 1;
    pthread_cond_broadcast(&work_cond);
    pthread_mutex_unlock(&work_mutex);

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n--- Final Results ---\n");
    for (int i = 0; i < result_count; i++) {
        printf("%s\n", result_buffer[i]);
    }

    return 0;
}

