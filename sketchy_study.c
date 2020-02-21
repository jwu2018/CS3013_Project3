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
	int ordered_list[4] = {0, 1, 2, 3};
	int *a = ordered_list;

	for (; i > 0; i--) {
		j = rand() % (i+1);
		temp = ordered_list[i];
		ordered_list[i] = ordered_list[j];
		ordered_list[j] = temp;
	}
	return a;
}

void* thread() {
	printf("in thread\n");

	args* instr;
	int instr_cnt;
	int instr_cnt_max;
	int stn_sts; //station status
	int stn_res_sts; //station reservation status
	int r;

	for(r=0;r<RUNS;r++){
		instr = make_ordered_list();
		instr_cnt = 0;
		instr_cnt_max = instr->list_size;
		

		while(instr_cnt<instr_cnt_max){
			stn_sts = stn[instr->ordered_list[instr_cnt]];

			pthread_mutex_lock(&grad_mutex);
			while(stn_sts!=0){
				if(stn_sts==1) pthread_cond_wait(&stat_1, &grad_mutex);
				else if(stn_sts==2) pthread_cond_wait(&stn_2, &grad_mutex);
				else if(stn_sts==3) pthread_cond_wait(&stn_3, &grad_mutex);
				else if(stn_sts==4) pthread_cond_wait(&stn_4, &grad_mutex);
			}
			while(stn_sts_res!=0){
				if(stn_sts==1) pthread_cond_wait(&stat_1, &grad_mutex);
				else if(stn_sts==2) pthread_cond_wait(&stn_2_res, &grad_mutex);
				else if(stn_sts==3) pthread_cond_wait(&stn_3_res, &grad_mutex);
				else if(stn_sts==4) pthread_cond_wait(&stn_4_res, &grad_mutex);
			}
			if(instr_cnt+1<instr_cnt_max){
				stn_res_sts = stn_res[instr->ordered_list[instr_cnt+1]];
				while(stn_sts!=0){
					if(stn_sts==1) pthread_cond_wait(&stat_1_res, &grad_mutex);
					else if(stn_sts==2) pthread_cond_wait(&stn_2, &grad_mutex);
					else if(stn_sts==3) pthread_cond_wait(&stn_3, &grad_mutex);
					else if(stn_sts==4) pthread_cond_wait(&stn_4, &grad_mutex);
				}
				while(stn_sts!=0){
					if(stn_sts==1) pthread_cond_wait(&stat_1, &grad_mutex);
					else if(stn_sts==2) pthread_cond_wait(&stn_2_res, &grad_mutex);
					else if(stn_sts==3) pthread_cond_wait(&stn_3_res, &grad_mutex);
					else if(stn_sts==4) pthread_cond_wait(&stn_4_res, &grad_mutex);
				}
			}

			pthread_mutex_unlock(&grad_mutex);

			sleep(1);

			instr_cnt++;
		}
	}
}