// Project 3
// CS 3013
// Jeffrey Huang and Jyalu Wu

#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <stdlib.h>
#include "sketchy_study.h"

int main() {
	struct args* grads_args[NUM_GRADS];
	pthread_t grads[NUM_GRADS];

    for (int i = 0; i < NUM_GRADS; i++) {
    	grads_args[i] = (struct args *)malloc(sizeof(struct args));
    	grads_args[i]->ordered_list = make_ordered_list();
		pthread_create(&(grads[i]),NULL,(void*) &thread, grads_args[i]); 
        
	}

	// printf("attempting to join threads\n");
	for (int i = 0; i < NUM_GRADS; i++) {
		pthread_join(grads[i],NULL); 	
	}

	return 0;
}

// Fisher-Yates shuffle algorithm
int* make_ordered_list() {
	time_t t;
	srand((unsigned) time(&t));

	int i = NUM_GRADS - 1;
	int j = 0;
	int temp = 0;
	int* ordered_list = {1, 2, 3, 4};

	for (; i > 0; i--) {
		j = rand() % (i+1);
		temp = ordered_list[i];
		ordered_list[i] = ordered_list[j];
		ordered_list[j] = temp;
	}
	return ordered_list;
}

void* thread() {
	printf("in thread\n");

}