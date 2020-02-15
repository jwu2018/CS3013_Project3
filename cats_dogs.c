// Project 3
// CS 3013
// Jeffrey Huang and Jyalu Wu
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 

sem_t empty, cats, dogs;
int cats_in, dogs_in;
int totalPets = 0;


void* cat() { 
	// char name[20];
	// pthread_getname_np(*pet, name, 20);

	//wait 
	sem_wait(&cats);
	cats_in++;
	if(cats_in==1) sem_wait(&empty);
	sem_post(&cats);
	// printf("%s the cat entered the kitchen\n", name); 
	// totalPets++;

	//critical section 
	sleep(1); 
	printf("cats_in = %d\n", cats_in);
	printf("dogs_in = %d\n", dogs_in);
	
	//signal 
	// printf("%s the cat just exiting the kitchen\n", name); 
	sem_wait(&cats);
	cats_in--;
	if(cats_in==0) sem_post(&empty);
	sem_post(&cats);
	// totalPets--;
} 

void* dog() {
	// char name[20];
	// pthread_getname_np(pet, name, 20); 

	//wait 
	sem_wait(&dogs);
	dogs_in++;
	if(dogs_in==1) sem_wait(&empty);
	sem_post(&dogs);
	// printf("%s the cat entered the kitchen\n", name); 
	// totalPets++;

	//critical section 
	sleep(1); 
	printf("cats_in = %d\n", cats_in);
	printf("dogs_in = %d\n", dogs_in);
	
	//signal 
	// printf("%s the cat just exiting the kitchen\n", name); 
	sem_wait(&dogs);
	dogs_in--;
	if(dogs_in==0) sem_post(&empty);
	sem_post(&dogs);
	// totalPets--;
} 


int main()  { 
	sem_init(&empty, 0, 1); 
	sem_init(&cats, 0, 1);
	sem_init(&dogs, 0, 1);
	cats_in = 0;
	dogs_in = 0;
	// make list of empty threads
	pthread_t pets[22];
	// make list of thread names ex. Loki
	// char names[22][15] = {"Abby", "Bob", "Carl", "Doggo", "Egg", "Frederickson", "Goat",
	// 	"Harold", "Igloo", "Jerfy", "Kiki", "Lemon", "Maddy", "Nom", "Oprah", "Pineapple",
	// 	"Queen", "Rice", "Spam", "Tom", "Unix", "Void"};
	// pthread_t t1,t2; 

	// cats loop
	int i = 0;
	for (; i < 13; i++) {
		// create thread starting at cat
		pthread_create(&(pets[i]),NULL,(void*) &cat,NULL); 
		// pthread_setname_np(pets[i], names[i]);
	}

	// dogs loop
	for (; i < 22; i++) {
		// create thread starting at dog
		pthread_create(&(pets[i]),NULL, (void*) &dog,NULL); 
		// pthread_setname_np(pets[i], names[i]);
	}
	// pthread_create(&t1,NULL,thread,NULL); 
	// sleep(2); 
	// pthread_create(&t2,NULL,thread,NULL); 
	i = 0;
	for (; i < 22; i++) {
		pthread_join(pets[i],NULL); 	
	}
	// pthread_join(t1,NULL); 
	// pthread_join(t2,NULL); 
	sem_destroy(&empty); 
	sem_destroy(&cats); 
	sem_destroy(&dogs); 
	return 0; 
} 
