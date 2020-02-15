// Project 3
// CS 3013
// Jeffrey Huang and Jyalu Wu

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

// #include "common.h"
// #include "common_threads.h"

// #include "zemaphore.h"
// #endif

int max;
int loops;
int *buffer;

int use  = 0;
int fill = 0;

sem_t empty;
sem_t full;
sem_t mutex;

#define CMAX (10)
int consumers = 1;

void do_fill(int value) {
    buffer[fill] = value;
    fill++;
    if (fill == max)
	fill = 0;
}

int do_get() {
    int tmp = buffer[use];
    use++;
    if (use == max)
	use = 0;
    return tmp;
}

// int sem_wait(sem_t *s) {
// 	*s = *s - 1;
// 	if (*s < 0) {
// 		//wait
// 	}
// }

// int sem_post(sem_t *s) {
// 	*s = *s + 1;

// }

// void Sem_init(Zem_t *z, int value) {
//     z->value = value;
//     Cond_init(&z->cond);
//     Mutex_init(&z->lock);
// }

// void Sem_wait(Zem_t *z) {
//     Mutex_lock(&z->lock);
//     while (z->value <= 0)
// 	Cond_wait(&z->cond, &z->lock);
//     z->value--;
//     Mutex_unlock(&z->lock);
// }

// void Sem_post(Zem_t *z) {
//     Mutex_lock(&z->lock);
//     z->value++;
//     Cond_signal(&z->cond);
//     Mutex_unlock(&z->lock);
// }

void *producer(void *arg) {
    int i;
    for (i = 0; i < loops; i++) {
	sem_wait(&empty);
	sem_wait(&mutex);
	do_fill(i);
	sem_post(&mutex);
	sem_post(&full);
    }

    // end case
    for (i = 0; i < consumers; i++) {
	sem_wait(&empty);
	sem_wait(&mutex);
	do_fill(-1);
	sem_post(&mutex);
	sem_post(&full);
    }

    return NULL;
}
                                                                               
void *consumer(void *arg) {
    int tmp = 0;
    while (tmp != -1) {
		sem_wait(&full);
		sem_wait(&mutex);
		tmp = do_get();
		sem_post(&mutex);
		sem_post(&empty);
		printf("%lld %d\n", (long long int) arg, tmp);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
		fprintf(stderr, "usage: %s <buffersize> <loops> <consumers>\n", argv[0]);
		exit(1);
    }
    max   = 2; //atoi(argv[1]);
    loops = 10; //atoi(argv[2]);
    consumers = 9; //atoi(argv[3]);
    assert(consumers <= CMAX);

    buffer = (int *) malloc(max * sizeof(int));
    assert(buffer != NULL);
    int i;
    for (i = 0; i < max; i++) {
		buffer[i] = 0;
    }

    sem_init(&empty, 0, max); // max are empty 
    sem_init(&full, 0, 0);    // 0 are full
    sem_init(&mutex, 0, 1);   // mutex

    pthread_t pid, cid[CMAX];
    pthread_create(&pid, NULL, producer, NULL); 
    for (i = 0; i < consumers; i++) {
		pthread_create(&cid[i], NULL, consumer, (void *) (long long int) i); 
    }
    pthread_join(pid, NULL); 
    for (i = 0; i < consumers; i++) {
		pthread_join(cid[i], NULL); 
    }
    return 0;
}