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

// represent if the station is taken/reserved:
// 		0 = empty or unreserved
//		1,2,3,4 = the id of the grad that is taking/reserving it
int stn[4];

pthread_cond_t stn_1, stn_2, stn_3, stn_4, stn_1_res, stn_2_res, stn_3_res, stn_4_res;

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

	// initialize stn and stn_res
	for (int i = 0; i < NUM_GRADS; i++) {
		stn[i] = 0;
		// stn_res[i] = 0;
	}

	// initialize all_specs
	for (int n = 0; n < NUM_GRADS; n++) {
		dummy[n] = ((specs*) malloc(sizeof(specs)));
	}
	all_specs = &dummy;

	// make grad threads
    for (int i = 0; i < NUM_GRADS; i++) {
    	// grads_args[i] = (args *)malloc(sizeof(args));
    	// grads_args[i] = make_grad_args();
    	args->creation_num = i;
    	// printf("creation num = %d\n", args->creation_num)
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
	// printf("making the grad specs\n");
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

	// printing the list out
	printf("list: ");
	for (int i = 0; i < num_steps; i++) {
		printf("%d ", list[i]);
	}
	printf("\n");

	grad_specs->ordered_list = list;
	grad_specs->list_index = 0;
	grad_specs->list_size = num_steps;

	// printf("list size: %d\n", num_steps);
	return grad_specs;
}

void* grad(void* input) {
	printf("i'm starting over\n");
	// int instr_cnt_max;
	int* list;
	int num_steps; // number of steps in the list
	int station_index; // index of station in the list
	int station; // current station
	int id; // grad 1, 2, 3, or 4
	// unsigned short int
	int num_rounds; // number of instruction sets the grad needs to go thru
	// int instr_cnt;
	// int stn_sts; //station status
	int stn_res_sts; //station reservation status
	// int r;

	grad_args* args = (grad_args*) input;
	specs* grad_specs = (specs *)malloc(sizeof(specs));
	num_rounds = 1;// (rand() % 10) + 1;
	id = args->creation_num;
	// id = pthread_self();

	// printf("num_rounds for grad %d = %d\n", id, num_rounds);

	for (int round = 1; round <= num_rounds; round++) { // for each round
		printf("round: %d\n", round);
		// make specs
		printf("making specs for grad %d\n", id);
		grad_specs = make_grad_specs();
		num_steps = grad_specs->list_size;
		list = grad_specs->ordered_list;
		station_index = grad_specs->list_index;
		station = list[station_index];
		// update global
		(*all_specs)[id] = grad_specs;
		// for each instruction
		// printf("num_steps = %d\n", num_steps);
		for (int instr_cnt = 0; instr_cnt < num_steps; instr_cnt++) { // for each instruction
			printf("grad %d instruct count: %d\n", id, instr_cnt);


			printf("stn: ");
			for (int i = 0; i < 4; i++) {
				printf("%d ", stn[i]);
			}
			printf("\n");


			// reset stn_res_sts
			// stn_res_sts = 0;
			while (!stn_res_sts) {
				if (isDeadlock(id)) {
					// wait for station cv and signal the next thread
					printf("is deadlock\n");
					if(station==1) {
						pthread_cond_wait(&stn_1, &grad_mutex);
						// pthread_cond_signal(&stn_1);
					}
					else if(station==2) {
						pthread_cond_wait(&stn_2, &grad_mutex);
						// pthread_cond_signal(&stn_2);
					}
					else if(station==3) {
						pthread_cond_wait(&stn_3, &grad_mutex);
						// pthread_cond_signal(&stn_3);
					}
					else if(station==4) {
						pthread_cond_wait(&stn_4, &grad_mutex);
						// pthread_cond_signal(&stn_4);
					}
				}
				else { // not deadlock
					// check spot 1 status
					// printf("checking spot 1 status\n");
					// printf("stn[%d] = %d\n", station_index, stn[station_index]);
					if (stn[station_index] == 0 || stn[station_index] == id) { // check station status
						if (instr_cnt == num_steps - 1) { // last instruction
							// clear previous from stn
							if (station_index != 0) {
								int prev_station = list[station_index - 1];
								stn[prev_station] = 0;
							}
							// printf("last instruction %d == %d\n", instr_cnt, num_steps-1);
							printf("last instruction\n");
							printf("grad %d is doing stuff at station %d\n", id, station);
							// sleep((rand() % 5) + 1);
							break;
						}
						else {
							// check spot 2 status
							// printf("checking spot 2 status\n");
							// printf("stn[%d] = %d\n", station_index+1, stn[station_index+1]);
							if (stn[station_index + 1] == 0) {
								// clear previous from stn
								if (station_index != 0) {
									int prev_station = list[station_index - 1];
									stn[prev_station] = 0;
								}
								stn[station_index] = id;
								stn[station_index + 1] = id;
								stn_res_sts = 1;
								printf("grad has reserved station %d\n", list[station_index + 1]);
								printf("grad %d is doing stuff at station %d\n", id, station);
								// sleep((rand() % 5) + 1);
								break;
							} // if
							else {
								// wait for reservation cv and signal the next thread
								printf("spot 1 not free\n");
								if(station==1) {
									pthread_cond_wait(&stn_1_res, &grad_mutex);
									// pthread_cond_signal(&stn_1_res);
								}
								else if(station==2) {
									pthread_cond_wait(&stn_2_res, &grad_mutex);
									// pthread_cond_signal(&stn_2_res);
								}
								else if(station==3) {
									pthread_cond_wait(&stn_3_res, &grad_mutex);
									// pthread_cond_signal(&stn_3_res);
								}
								else if(station==4) {
									pthread_cond_wait(&stn_4_res, &grad_mutex);
									// pthread_cond_signal(&stn_4_res);
								}
							} // else
						} // else
					} // if
					else {
						// wait for station cv and signal the next thread
						printf("can't reserve\n");
						if(station==1) {
							pthread_cond_wait(&stn_1, &grad_mutex);
							// pthread_cond_signal(&stn_1);
						}
						else if(station==2) {
							pthread_cond_wait(&stn_2, &grad_mutex);
							// pthread_cond_signal(&stn_2);
						}
						else if(station==3) {
							pthread_cond_wait(&stn_3, &grad_mutex);
							// pthread_cond_signal(&stn_3);
						}
						else if(station==4) {
							pthread_cond_wait(&stn_4, &grad_mutex);
							// pthread_cond_signal(&stn_4);
						}
					} // else

				}
			} // while stn_res_sts != 1
			printf("exited while loop\n");

			// update specs
			station_index++;
			station = list[station_index];
			stn_res_sts = 0;
			grad_specs->list_index = station_index;
	
		} // for each instruction
		printf("exited loop for all instructions\n");
		// clear previous from stn
		if (station_index != 0) {
			int prev_station = list[station_index - 1];
			stn[prev_station] = 0;
		}
	} // for each round
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