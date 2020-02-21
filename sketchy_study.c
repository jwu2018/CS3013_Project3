// Project 3
// CS 3013
// Jeffrey Huang and Jyalu Wu

#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <stdlib.h>
#include "sketchy_study.h"

specs *(*all_specs)[];

int main() {
	time_t t;
	srand((unsigned) time(&t));
	// specs all_specs[NUM_GRADS];
	pthread_t grads[NUM_GRADS];
	grad_args* args = (grad_args*) malloc(sizeof(grad_args));
	specs* dummy[NUM_GRADS];
	// specs *(*all_specs)[];
	// all_specs = (specs**) malloc(sizeof(specs*));

	// initialize all_specs
	for (int n = 0; n < NUM_GRADS; n++) {
		dummy[n] = ((specs*) malloc(sizeof(specs)));
	}
	all_specs = &dummy;

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
	int num_steps = (rand() % 4) + 1;
	int i = num_steps - 1;
	// printf("rand = %d", rand());
	int* list = (int*) malloc(sizeof(int));
	specs* grad_specs = (specs*) malloc(sizeof(specs));



	// initialize list
	for (int z = 0; z < num_steps; z++) {
		list[z] = z + 1;
	}

	// shuffle list
	for (; i > 0; i--) {
		j = rand() % (i+1);
		temp = list[i];
		list[i] = list[j];
		list[j] = temp;
	}

	printf("list: ");
	for (int i = 0; i < num_steps; i++) {
		printf("%d ", list[i]);
	}
	printf("\n");

	grad_specs->ordered_list = list;
	grad_specs->current_station = list[0];
	grad_specs->list_size = num_steps;
	return grad_specs;
}

void* grad(void* input) {
	int num_steps;
	int station;
	int* list;
	int id;
	int num_rounds;

	grad_args* args = (grad_args*) input;
	specs* grad_specs = (specs *)malloc(sizeof(specs));
	num_rounds = (rand() % 10) + 1;
	printf("num_rounds = %d\n", num_rounds);
	id = args->creation_num;

	for (int round = 0; round < num_rounds; round++) {
		// make specs
		grad_specs = make_grad_specs();
		num_steps = grad_specs->list_size;
		list = grad_specs->ordered_list;
		station = grad_specs->current_station;
		// update global
		(*all_specs)[id] = grad_specs;
		// for each instruction
		for (int i_index = 0; i_index < num_steps; i_index++) {

			if (isDeadlock) {
				printf("is deadlock\n");
			}




			// if is a deadlock
			// 	wait and signal
			// else if spot 1 is free
			// 	if it's the last instruction
			// 		sleep, done
			// 	else if spot 2 free
			// 		reserve spot 2
			// 		go to spot 1
			// 		sleep
			// 		if is last instruction, done
			// 	else wait and signal
			// else wait and signal
		}
	}


}

int isDeadlock(int grad_id) {
	specs* current = (*all_specs)[grad_id];
	specs* other;

	for (int other_id = 0; other_id < NUM_GRADS; other_id++) {
		if (other_id != grad_id) {
			other = (*all_specs)[other_id];
			if (isCrossDeadlock(current, other)) return 1;
			if (isParallelDeadlock(current, other)) return 1;
		}
	}
	return 0;
}

int isCrossDeadlock(specs* current, specs* other) {
	int current_size = current->list_size;
	int other_size = other->list_size;
	int* current_list = current->ordered_list;
	int* other_list = other->ordered_list;

	if (current_size >= 2 && other_size >= 2) {
		if (current_list[0] == other_list[1] && current_list[1] == other_list[0])
			return 1;
	}
	if (current_size >=3 && other_size >= 3) {
		if (current_list[2] == other_list[1] && current_list[1] == other_list[2])
			return 1;
	}
	return 0;
}

int isParallelDeadlock(specs* current, specs* other) {
	specs* bigger;
	specs* smaller;
	int* bigger_list;
	int* smaller_list;
	
	if (current->list_size > other->list_size) {
		bigger = current;
		bigger_list = current->ordered_list;
		smaller = other;
		smaller_list = other->ordered_list;
	}
	else {
		bigger = other;
		bigger_list = other->ordered_list;
		smaller = current;
		smaller_list = current->ordered_list;
	}
	if (bigger->list_size != 4) return 0;
	if (smaller->list_size < 2) return 0;
	if (bigger_list[2] == smaller_list[0] && bigger_list[3] == smaller_list[1]) return 1;

	return 0;
}