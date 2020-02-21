// Project 3
// CS 3013
// Jeffrey Huang and Jyalu Wu

#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <stdlib.h>
#include "sketchy_study.h"

struct specs** all_specs;

int main() {
	time_t t;
	srand((unsigned) time(&t));
	specs all_specs[NUM_GRADS];
	pthread_t grads[NUM_GRADS];
	grad_args* args = (grad_args*) malloc(sizeof(grad_args));

	// initialize all_specs
	for (int n = 0; n < NUM_GRADS; n++) {
		all_specs[n] = *((specs*) malloc(sizeof(specs)));
	}

    for (int i = 0; i < NUM_GRADS; i++) {
    	// grads_args[i] = (args *)malloc(sizeof(args));
    	// grads_args[i] = make_grad_args();
    	args->creation_num = i;
		pthread_create(&(grads[i]),NULL,(void*) &grad, args);
        
	}
	printf("attempting to join threads\n");
	for (int i = 0; i < NUM_GRADS; i++) {
		pthread_join(grads[i],NULL); 	
	}

	return 0;
}

// Fisher-Yates shuffle algorithm
specs* make_grad_specs() {
	// int i = NUM_GRADS - 1;
	int j = 0;
	int temp = 0;
	int size = (rand() % 4) + 1;
	int i = size - 1;
	// printf("rand = %d", rand());
	int* list = (int*) malloc(sizeof(int));
	specs* grad_specs = (specs*) malloc(sizeof(specs));



	// initialize list
	for (int z = 0; z < size; z++) {
		list[z] = z + 1;
		// printf("%d ", list[z]);
	}
	// printf("\n");

	// shuffle list
	for (; i > 0; i--) {
		j = rand() % (i+1);
		// printf("swapping %d and %d\n", list[j], list[i]);
		temp = list[i];
		list[i] = list[j];
		list[j] = temp;
	}

	// printf("thread list size: %d\n", size);
	printf("list: ");
	for (int i = 0; i < size; i++) {
		printf("%d ", list[i]);
	}
	printf("\n");
	// printf("done printing list\n");

	grad_specs->ordered_list = list;
	// printf("list ok\n");
	grad_specs->current_station = list[0];
	// printf("station ok\n");
	grad_specs->list_size = size;
	// printf("size ok\n");
	// printf("finished making grad args\n");
	return grad_specs;
}

void* grad(void* input) {
	int size;
	int station;
	int* list;
	int num;
	grad_args* args = (grad_args*) input;
	specs* grad_specs = (specs *)malloc(sizeof(specs));

	num = args->creation_num;
	grad_specs = make_grad_specs();
	// printf("here\n");
	size = grad_specs->list_size;
	// printf("here 2\n");
	list = grad_specs->ordered_list;
	// printf("here 3\n");
	*all_specs[num] = *grad_specs;
	// printf("in thread\n");
	// printf("list size: %d\n", size);
	// for (int i = 0; i < size; i++) {
	// 	printf("%d ", list[i]);
	// }
	// printf("\n");
}