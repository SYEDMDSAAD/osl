#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 10

sem_t full, empty;
int buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
int counter;

void *producer(void *p);
void *consumer(void *p);
void insert_item(int item);
int remove_item();
void initialize();
void cleanup();

int main() {
    int n1, n2, i;
    printf("\nEnter number of producers you want to create: ");
    scanf("%d", &n1);
    printf("\nEnter number of consumers you want to create: ");
    scanf("%d", &n2);

    initialize();

    pthread_t producers[n1], consumers[n2];

    // Create producer threads
    for (i = 0; i < n1; i++) {
        if (pthread_create(&producers[i], NULL, producer, NULL) != 0) {
            perror("Failed to create producer thread");
            exit(EXIT_FAILURE);
        }
    }

    // Create consumer threads
    for (i = 0; i < n2; i++) {
        if (pthread_create(&consumers[i], NULL, consumer, NULL) != 0) {
            perror("Failed to create consumer thread");
            exit(EXIT_FAILURE);
        }
    }

    // Join producer threads
    for (i = 0; i < n1; i++) {
        pthread_join(producers[i], NULL);
    }

    // Join consumer threads
    for (i = 0; i < n2; i++) {
        pthread_join(consumers[i], NULL);
    }

    cleanup();

    return 0;
}

void *producer(void *p) {
    while (1) {
        int item = rand() % 100;  // Producing a random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        insert_item(item);
        printf("Producer produced item %d\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        
        sleep(rand() % 3); // Simulate time taken to produce
    }
    return NULL;
}

void *consumer(void *p) {
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = remove_item();
        printf("Consumer consumed item %d\n", item);

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        sleep(rand() % 3); // Simulate time taken to consume
    }
    return NULL;
}

void insert_item(int item) {
    if (counter < BUFFER_SIZE) {
        buffer[counter++] = item;
    }
}

int remove_item() {
    if (counter > 0) {
        return buffer[--counter];
    }
    return -1;  // Should not reach here
}

void initialize() {
    pthread_mutex_init(&mutex, NULL);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);
    counter = 0;
    srand(time(NULL));
}

void cleanup() {
    sem_destroy(&full);
    sem_destroy(&empty);
    pthread_mutex_destroy(&mutex);
}

