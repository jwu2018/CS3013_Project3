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
#define RUNS 5

int stn[4], stn_res[4];

pthread_cond_t stn_1, stn_2, stn_3, stn_4, stn_1_res, stn_2_res, stn_3_res, stn_4_res;

pthread_mutex_t grad_mutex;

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
	int instr_cnt_max;
	int station;
	int* list;
	grad_args* args = (grad_args*) input;
	int grad_id = args->creation_num;
	specs* grad_specs = (specs *)malloc(sizeof(specs));

	// printf("in thread\n");
	// printf("list size: %d\n", size);
	// for (int i = 0; i < size; i++) {
	// 	printf("%d ", list[i]);
	// }
	// printf("\n");

	int instr_cnt;
	int stn_sts; //station status
	int stn_res_sts; //station reservation status
	int r;

	printf("starting runs\n");
	for(r=0;r<RUNS;r++){
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

			stn_sts = stn[list[instr_cnt]];

			pthread_mutex_lock(&grad_mutex);
			while(stn_sts!=0){
				if(stn_sts==1) pthread_cond_wait(&stn_1, &grad_mutex);
				else if(stn_sts==2) pthread_cond_wait(&stn_2, &grad_mutex);
				else if(stn_sts==3) pthread_cond_wait(&stn_3, &grad_mutex);
				else if(stn_sts==4) pthread_cond_wait(&stn_4, &grad_mutex);
			}
			while(stn_res_sts!=0){
				if(stn_sts==1) pthread_cond_wait(&stn_1, &grad_mutex);
				else if(stn_sts==2) pthread_cond_wait(&stn_2_res, &grad_mutex);
				else if(stn_sts==3) pthread_cond_wait(&stn_3_res, &grad_mutex);
				else if(stn_sts==4) pthread_cond_wait(&stn_4_res, &grad_mutex);
			}
			if(instr_cnt+1<instr_cnt_max){
				stn_res_sts = stn_res[list[instr_cnt+1]];
				while(stn_res_sts!=0){
					if(stn_res_sts==1) pthread_cond_wait(&stn_1, &grad_mutex);
					else if(stn_res_sts==2) pthread_cond_wait(&stn_2, &grad_mutex);
					else if(stn_res_sts==3) pthread_cond_wait(&stn_3, &grad_mutex);
					else if(stn_res_sts==4) pthread_cond_wait(&stn_4, &grad_mutex);
				}
				while(stn_res_sts!=0){
					if(stn_res_sts==1) pthread_cond_wait(&stn_1_res, &grad_mutex);
					else if(stn_res_sts==2) pthread_cond_wait(&stn_2_res, &grad_mutex);
					else if(stn_res_sts==3) pthread_cond_wait(&stn_3_res, &grad_mutex);
					else if(stn_res_sts==4) pthread_cond_wait(&stn_4_res, &grad_mutex);
				}
			}

			pthread_mutex_unlock(&grad_mutex);

			sleep(1);

			instr_cnt++;

		}
	}
}