#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2
#define READER_ACTIONS 3 // Number of actions each reader performs
#define WRITER_ACTIONS 2 // Number of actions each writer performs

sem_t read_count;   // Semaphore for managing read count
sem_t resource;     // Semaphore for writer access
pthread_mutex_t mutex; // Mutex to protect read_count

int read_count_val = 0; // Number of readers currently reading

void *reader(void *param) {
    int id = *(int *)param;
    int actions = READER_ACTIONS;

    while (actions--) {
        // Wait for readers to start
        sem_wait(&read_count);
        pthread_mutex_lock(&mutex);
        read_count_val++;
        if (read_count_val == 1) {
            // First reader waits for writers to finish
            sem_wait(&resource);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&read_count);

        // Reading section
        printf("Reader %d is reading\n", id);
        sleep(rand() % 3 + 1);  // Simulate reading time

        // End of reading section
        pthread_mutex_lock(&mutex);
        read_count_val--;
        if (read_count_val == 0) {
            // Last reader releases the resource
            sem_post(&resource);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&read_count);

        sleep(rand() % 5 + 1);  // Simulate time between reads
    }

    return NULL;
}

void *writer(void *param) {
    int id = *(int *)param;
    int actions = WRITER_ACTIONS;

    while (actions--) {
        // Waiting to access resource
        sem_wait(&resource);

        // Writing section
        printf("Writer %d is writing\n", id);
        sleep(rand() % 3 + 1);  // Simulate writing time

        // End of writing section
        sem_post(&resource);

        sleep(rand() % 5 + 1);  // Simulate time between writes
    }

    return NULL;
}

int main() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int reader_ids[NUM_READERS], writer_ids[NUM_WRITERS];
    
    // Initialize semaphores and mutex
    sem_init(&read_count, 0, 1);
    sem_init(&resource, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        reader_ids[i] = i + 1;
        if (pthread_create(&readers[i], NULL, reader, &reader_ids[i]) != 0) {
            perror("Failed to create reader thread");
            exit(EXIT_FAILURE);
        }
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writer_ids[i] = i + 1;
        if (pthread_create(&writers[i], NULL, writer, &writer_ids[i]) != 0) {
            perror("Failed to create writer thread");
            exit(EXIT_FAILURE);
        }
    }

    // Join reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    // Join writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    // Destroy semaphores and mutex
    sem_destroy(&read_count);
    sem_destroy(&resource);
    pthread_mutex_destroy(&mutex);

    return 0;
}

