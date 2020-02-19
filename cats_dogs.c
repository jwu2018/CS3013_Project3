// Project 3
// CS 3013
// Jeffrey Huang and Jyalu Wu
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <stdlib.h>

sem_t empty, waiting, cats_lock, dogs_lock, cats_max, dogs_max;
int cats_in, dogs_in, cats_drinking, dogs_drinking;
int totalPets = 0;

struct args {
	int id;
	char* name;
} args;

void* cat(void* input) { 
	struct args* this_cat = (struct args*) input;
	int time_drinking = 0;
	// printf("is cat %d\n", this_cat->id);
	// int* sem_val = (int*) malloc(5);
	
	// char name[20];
	// pthread_getname_np(*pet, name, 20);

	//wait 
	sem_wait(&waiting);
	sem_wait(&cats_lock);
	cats_in++;
	// printf("cats_in was incremented\n");
	if(cats_in == 1) {
		printf("cat waiting for kitchen to be empty\n");
		sem_wait(&empty);
	}
	sem_post(&cats_lock);
	sem_post(&waiting);

	// printf("%s the cat entered the kitchen\n", name); 
	// totalPets++;
	sem_wait(&cats_max);
	cats_drinking++;
	//critical section 
	time_drinking = (rand() % 5) + 1;
	// printf("time_drinking = %d\n", time_drinking);
	sleep(time_drinking); 
	// sem_getvalue(&cats, sem_val);
	// printf("cats = %d\n", *sem_val);
	// sem_getvalue(&dogs, sem_val);
	// printf("dogs = %d\n", *sem_val);
	printf("is cat %d\n", this_cat->id);
	printf("cats_in = %d\n", cats_in);
	printf("dogs_in = %d\n", dogs_in);
	printf("cats_drinking = %d\n", cats_drinking);
	printf("dogs_drinking = %d\n\n", dogs_drinking);
	
	sem_post(&cats_max);
	cats_drinking--;
	//signal
	// printf("%s the cat just exiting the kitchen\n", name); 
	sem_wait(&cats_lock);
	cats_in--;
	if(cats_in==0) sem_post(&empty);
	sem_post(&cats_lock);
	// totalPets--;
} 

void* dog(void* input) {
	struct args* this_dog = (struct args*) input;
	int time_drinking = 0;
	// printf("is dog\n");
	
	// char name[20];
	// pthread_getname_np(pet, name, 20); 

	//wait 
	sem_wait(&waiting);
	sem_wait(&dogs_lock);
	dogs_in++;
	if(dogs_in == 1) {
		printf("dog waiting for kitchen to be empty\n");
		sem_wait(&empty);
	}
	sem_post(&dogs_lock);
	sem_post(&waiting);
	// printf("%s the cat entered the kitchen\n", name); 
	// totalPets++;
	sem_wait(&dogs_max);
	dogs_drinking++;
	//critical section 
	time_drinking = (rand() % 5) + 1;
	// printf("time_drinking = %d\n", time_drinking);
	sleep(time_drinking); 
	printf("is dog %d\n", this_dog->id);
	printf("cats_in = %d\n", cats_in);
	printf("dogs_in = %d\n", dogs_in);
	printf("cats_drinking = %d\n", cats_drinking);
	printf("dogs_drinking = %d\n\n", dogs_drinking);
	
	sem_post(&dogs_max);
	dogs_drinking--;
	// printf("here\n");
	//signal 
	// printf("%s the cat just exiting the kitchen\n", name); 
	sem_wait(&dogs_lock);
	// printf("here 1\n");
	dogs_in--;
	if(dogs_in==0) sem_post(&empty);
	sem_post(&dogs_lock);
	// printf("here 2\n");
	// totalPets--;
} 


int main()  { 
	sem_init(&empty, 0, 1);
	sem_init(&waiting, 0, 1); 
	sem_init(&cats_lock, 0, 1);
	sem_init(&dogs_lock, 0, 1);
	sem_init(&cats_max, 0, 2);
	sem_init(&dogs_max, 0, 2);
	cats_in = 0;
	dogs_in = 0;
	cats_drinking = 0;
	dogs_drinking = 0;
	// make list of empty threads
	pthread_t pets[22];
	// make list of thread names ex. Loki
	// char names[22][15] = {"Abby", "Bob", "Carl", "Doggo", "Egg", "Frederickson", "Goat",
	// 	"Harold", "Igloo", "Jerfy", "Kiki", "Lemon", "Maddy", "Nom", "Oprah", "Pineapple",
	// 	"Queen", "Rice", "Spam", "Tom", "Unix", "Void"};
	// pthread_t t1,t2; 


	// random loop
	int i = 0;
	int num_cats = 4;//13;
	int num_dogs = 3;//9;
	int total_pets = num_cats + num_dogs;
	int random_num = 0;
	time_t t;

	srand((unsigned) time(&t));

	for (; i < total_pets; i++) {
		printf("this is pet number %d\n", i+1);
		struct args *Pet = (struct args *)malloc(sizeof(struct args));
		Pet->id = i;
		Pet->name = "";

		if (num_cats == 0) {
			printf("no more cats, creating dog %d\n", 4 - num_dogs);
			pthread_create(&(pets[i]),NULL, (void*) &dog, Pet);
			num_dogs--;
		}
		else if (num_dogs == 0) {
			printf("no more dogs, creating cat %d\n", 5 - num_cats);
			pthread_create(&(pets[i]),NULL,(void*) &cat, Pet); 
			num_cats--;
		}
		
		else if (random_num % 2 == 0) { // make cat
			random_num = rand();
			printf("making cat %d\n", 5 - num_cats);
			pthread_create(&(pets[i]),NULL,(void*) &cat, Pet); 
			num_cats--;
		}
		else { // make dog
			random_num = rand();
			printf("making dog %d\n", 4 - num_dogs);
			pthread_create(&(pets[i]),NULL, (void*) &dog, Pet);
			num_dogs--;	
		}
	}

	// cats loop
	// int i = 0;
	// for (; i < 13; i++) {
	// 	// create thread starting at cat
	// 	pthread_create(&(pets[i]),NULL,(void*) &cat,NULL); 
	// 	// pthread_setname_np(pets[i], names[i]);
	// }

	// // dogs loop
	// for (; i < 22; i++) {
	// 	// create thread starting at dog
	// 	pthread_create(&(pets[i]),NULL, (void*) &dog,NULL); 
	// 	// pthread_setname_np(pets[i], names[i]);
	// }
	// pthread_create(&t1,NULL,thread,NULL); 
	// sleep(2); 
	// pthread_create(&t2,NULL,thread,NULL); 
	printf("attempting to join threads\n");
	i = 0;
	for (; i < 22; i++) {
		pthread_join(pets[i],NULL); 	
	}
	// pthread_join(t1,NULL); 
	// pthread_join(t2,NULL); 
	printf("here\n");
	sem_destroy(&empty); 
	sem_destroy(&cats_lock); 
	sem_destroy(&dogs_lock); 
	sem_destroy(&cats_max);
	sem_destroy(&dogs_max);

	return 0; 
} 
