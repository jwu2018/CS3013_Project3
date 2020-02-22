#include <stdio.h> 
#include <pthread.h> 
#include <unistd.h> 
#include <stdlib.h>
#include "study.h"

pthread_mutex_t mutex, station1, station2, station3, station4;

// list *(*all_lists)[];

int main() {
	time_t t;
	srand((unsigned) time(&t));
	// specs all_specs[NUM_GRADS];
	pthread_t grads[NUM_GRADS];
	int total_obj = 0;
	
	char** bob_objects = (char**) malloc(sizeof(char*));
	char** beef_objects = (char**) malloc(sizeof(char*));
	char** bilbo_objects = (char**) malloc(sizeof(char*));
	char** ginger_objects = (char**) malloc(sizeof(char*));
	// grad_args* args = (grad_args*) malloc(sizeof(grad_args));
	// grad_specs* args = (grad_specs*) malloc(sizeof(grad_specs));
	// list* dummy[NUM_GRADS];
	// specs *(*all_specs)[];
	// all_lists = (list**) malloc(sizeof(list*));

	// initialize stn and stn_res
	// for (int i = 0; i < NUM_GRADS; i++) {
	// 	stn[i] = 0;
	// 	// stn_res[i] = 0;
	// }

	// initialize all_lists
	// for (int n = 0; n < NUM_GRADS; n++) {
	// 	dummy[n] = ((list*) malloc(sizeof(list)));
	// }
	// all_lists = &dummy;

	// make grad threads

	char names[4][30] = {"Bobbigail", "Beef Jerky", "Bilbo Baggins", "Ginger"};
	char objects[20][30] = {"Chair", "Hand", "Sari", "Freezer", "Belt", "Suit",
		"Chili", "Egg", "Whale", "Laptop", "Sandals", "Pants", "Bikini", "Ribs",
		"Potato", "Whiteboard", "Noodles", "Table", "Jewelry", "Cauliflower"
	};


    for (int i = 0; i < NUM_GRADS; i++) {
    	int num_obj = (rand() % 5) + 1;
    	grad_specs* args = (grad_specs*) malloc(sizeof(grad_specs));

		for (int j = 0; j < num_obj; j++) {
			// printf("adding object %s to the list for %d\n",objects[total_obj+j], i);
			if (i == 0) {
				bob_objects[j] = objects[total_obj+j];
			}
			else if (i == 1) {
				beef_objects[j] = objects[total_obj+j];
			}
			else if (i == 2) {
				bilbo_objects[j] = objects[total_obj+j];
			}
			else {
				ginger_objects[j] = objects[total_obj+j];
			}
		}
		total_obj+= (num_obj);

    	args->name = names[i];
    	args->current_object = 0;
    	args->num_objects = num_obj;
    	if (i == 0) {
    		args->objects = bob_objects;
    	}
    	else if (i == 1) {
    		args->objects = beef_objects;
    	}
    	else if (i==2) {
    		args->objects = bilbo_objects;
    	}
    	else {
    		args->objects = ginger_objects;
    	}
    	
    	// printf("creation num = %d\n", args->creation_num)
		pthread_create(&(grads[i]),NULL,(void*) &grad, args);
		// free(args);
	}
	printf("attempting to join threads\n");
	for (int i = 0; i < NUM_GRADS; i++) {
		pthread_join(grads[i],NULL);	
	}

	return 0;
}

void* grad(void* input) {
	int* list;
	int num_steps; // number of steps in the list
	// unsigned long int id; // grad 1, 2, 3, or 4
	char* id;
	char* current_obj;
	int num_objs;
	int rand_num = (rand() % 5) + 1;

	char methods[4][30] = {"soaking", "shocking", "squeezing", "scorching"};
	


	grad_specs* specs = (grad_specs*) input; // grad_id
	// id = specs->grad_id;
	id = specs->name;
	num_objs = specs->num_objects;
	current_obj = specs->objects[specs->current_object];
	// id = pthread_self();
	printf("Howdy doo I'm %s and I have the %s.\nI need to go to stations ", id, current_obj);

	num_steps = (rand() % 4) + 1;
	// printf("num steps: %d\n", num_steps);
	list = make_random_list(num_steps);
	printf("list: ");
	for (int i = 0; i < num_steps; i++) {
		printf("%d ", list[i]);
	}
	printf("in that order.\n\n");

	// printf("list of objects: ");
	// for (int i = 0; i <num_objs; i++) {
	// 	printf("%s ", specs->objects[i]);
	// }
	// printf("\n");

	for (int i = 0; i < num_objs; i++) {
		pthread_mutex_lock(&mutex);
		for (int index = 0; index < num_steps; index++) {
			// printf("%s is locking station %d\n", id, list[index]);
			if (list[index] == 1) {
				pthread_mutex_lock(&station1);
				printf("I, %s, have total control over station %d.\n", id, list[index]);
			}
			else if (list[index] == 2) {
				pthread_mutex_lock(&station2);
				printf("I, %s, have total control over station %d.\n", id, list[index]);
			}
			else if (list[index] == 3) {
				pthread_mutex_lock(&station3);
				printf("I, %s, have total control over station %d.\n", id, list[index]);
			}
			else if (list[index] == 4) {
				pthread_mutex_lock(&station4);
				printf("I, %s, have total control over station %d.\n", id, list[index]);
			}
		}
		pthread_mutex_unlock(&mutex);

		for (int item = 0; item < num_steps; item++) {
			// printf("%s is doing stuff at station %d\n", id, list[item]);
			sleep(rand_num);
			printf("I, %s, am %s the %s at station %d.\n", id, methods[list[item]-1], current_obj, list[item]);
			if (list[item] == 1) {
				pthread_mutex_unlock(&station1);
			}
			else if (list[item] == 2) {
				pthread_mutex_unlock(&station2);
			}
			else if (list[item] == 3) {
				pthread_mutex_unlock(&station3);
			}
			else if (list[item] == 4) {
				pthread_mutex_unlock(&station4); 
			}
		}
		printf("I, %s, am discarding the %s ", id, current_obj);
		specs->current_object = specs->current_object + 1;
		current_obj = specs->objects[specs->current_object];
		printf("and have picked the %s as my next victim.\n", current_obj);
	}
}

int* make_random_list(int num_steps) {
	int j = 0;
	int temp = 0;
	int i = num_steps - 1;
	// printf("rand = %d", rand());
	int* list = (int*) malloc(sizeof(int));
	// specs* grad_specs = (specs*) malloc(sizeof(specs));

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
	return list;
}