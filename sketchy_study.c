// Project 3
// CS 3013
// Jeffrey Huang and Jyalu Wu

#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <stdlib.h>
#include "sketchy_study.h"

#define NAME1 1
#define NAME2 2
#define NAME3 3
#define NAME4 4

int stn[4], stn_res[4];

//cv to indicate when a station's status is updated
pthread_cond_t stn_1, stn_2, stn_3, stn_4;

pthread_mutex_t grad_mutex;

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
	printf("making the grad specs\n");
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
	int instr_cnt_max;
	int num_steps;
	int station;
	int* list;
	int id;
	int num_rounds;

	grad_args* args = (grad_args*) input;
	int grad_id = args->creation_num;
	specs* grad_specs = (specs *)malloc(sizeof(specs));
	num_rounds = (rand() % 10) + 1;
	printf("num_rounds = %d\n", num_rounds);
	// id = args->creation_num;

	// for (int round = 0; round < num_rounds; round++) {
	// 	// make specs
	// 	grad_specs = make_grad_specs();
	// 	num_steps = grad_specs->list_size;
	// 	list = grad_specs->ordered_list;
	// 	station = grad_specs->current_station;
	// 	// update global
	// 	(*all_specs)[id] = grad_specs;
	// 	// for each instruction
	// 	for (int i_index = 0; i_index < num_steps; i_index++) {

	// 		if (isDeadlock) {
	// 			printf("is deadlock\n");
	// 		}
	// 	}
	// }


	// printf("in thread\n");
	// printf("list size: %d\n", size);
	// for (int i = 0; i < size; i++) {
	// 	printf("%d ", list[i]);

	// 		// if is a deadlock
	// 		// 	wait and signal
	// 		// else if spot 1 is free
	// 		// 	if it's the last instruction
	// 		// 		sleep, done
	// 		// 	else if spot 2 free
	// 		// 		reserve spot 2
	// 		// 		go to spot 1
	// 		// 		sleep
	// 		// 		if is last instruction, done
	// 		// 	else wait and signal
	// 		// else wait and signal
	// 	}
	// }


	int instr_cnt;
	int curr_stn_sts;
	int curr_stn_res_sts;
	int nxt_stn_sts;
	int nxt_stn_res_sts;
	int r;

	printf("starting runs\n");
	for(r=0;r<num_rounds;r++){
		instr_cnt = 0;
		grad_specs = make_grad_specs();
		// instr_cnt_max = instr->list_size;
		// num = args->creation_num;
		// printf("here\n");
		instr_cnt_max = grad_specs->list_size;
		// printf("here 2\n");
		list = grad_specs->ordered_list;
		// printf("here 3\n");
		*all_specs[grad_id] = *grad_specs;
		
		printf("starting object instruction set\n");
		while(instr_cnt<instr_cnt_max){

			curr_stn = list[instr_cnt];
			curr_stn_sts = stn[curr_stn];

			pthread_mutex_lock(&grad_mutex);
			if(instr_cnt+1<instr_cnt_max){
				stn_nxt = list[instr_cnt+1];
				stn_res_sts = stn_res[stn_next];
				while(stn_sts stn_res_sts!=0){
					if(stn_nxt==1) pthread_cond_wait(&stn_1, &grad_mutex);
					else if(stn_nxt==2) pthread_cond_wait(&stn_2, &grad_mutex);
					else if(stn_nxt==3) pthread_cond_wait(&stn_3, &grad_mutex);
					else if(stn_nxt==4) pthread_cond_wait(&stn_4, &grad_mutex);
				}
			}
			while(stn_sts!=0){
				if(stn_curr==1) pthread_cond_wait(&stn_1, &grad_mutex);
				else if(stn_curr==2) pthread_cond_wait(&stn_2, &grad_mutex);
				else if(stn_curr==3) pthread_cond_wait(&stn_3, &grad_mutex);
				else if(stn_curr==4) pthread_cond_wait(&stn_4, &grad_mutex);
			}

			pthread_mutex_unlock(&grad_mutex);

			sleep(1);

			instr_cnt++;

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