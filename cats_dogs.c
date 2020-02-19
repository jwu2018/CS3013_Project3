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


/*
 * When this is called, a cat is added to the waiting queue for
 * drinking water. If there is at least one dog in the kitchen,
 * it waits until there are no dogs left. If there is one other
 * cat in the kitchen and it is next in the queue, it enters the
 * kitchen and drinks water for a random number of seconds.
 * @param input, A struct containing:
 * 		The id of the cat (the order of creation)
 *      The name of the cat
 */
void* cat(void* input) { 
	struct args* this_cat = (struct args*) input;
	int time_drinking = (rand() % 5) + 1;
	// printf("start: is cat %s\n", this_cat->name);
	printf("%s the cat wants to drink for %d secs.\n", this_cat->name, time_drinking);
	// int* sem_val = (int*) malloc(5);
	
	// char name[20];
	// pthread_getname_np(*pet, name, 20);

	//wait 
	sem_wait(&waiting);
	sem_wait(&cats_lock);
	cats_in++;
	// printf("cats_in was incremented\n");
	if(cats_in == 1) {
		printf("%s the cat is waiting for kitchen to be empty\n", this_cat->name);
		sem_wait(&empty);
	}
	sem_post(&cats_lock);
	sem_post(&waiting);


	// totalPets++;
	sem_wait(&cats_max);
	printf("%s the cat entered the kitchen.\n", this_cat->name);
	cats_drinking++;
	//critical section 
	// printf("time_drinking = %d\n", time_drinking);
	sleep(time_drinking); 
	// sem_getvalue(&cats, sem_val);
	// printf("cats = %d\n", *sem_val);
	// sem_getvalue(&dogs, sem_val);
	// printf("dogs = %d\n", *sem_val);
	// printf("is cat %s\n", this_cat->name);
	// printf("time_drinking = %d\n", time_drinking);
	
	// printf("cats_in = %d\n", cats_in);
	// printf("dogs_in = %d\n", dogs_in);
	// printf("cats_drinking = %d\n", cats_drinking);
	// printf("dogs_drinking = %d\n\n", dogs_drinking);
	
	sem_post(&cats_max);
	cats_drinking--;
	//signal
	printf("%s the cat just exited the kitchen.\n", this_cat->name); 
	sem_wait(&cats_lock);
	cats_in--;
	if(cats_in==0) sem_post(&empty);
	sem_post(&cats_lock);
	// totalPets--;
} 

/*When this is called, a dog is added to the waiting queue for
 * drinking water. If there is at least one cat in the kitchen,
 * it waits until there are no cats left. If there is one other
 * dog in the kitchen and it is next in the queue, it enters the
 * kitchen and drinks water for a random number of seconds.
 * @param input, A struct containing:
 *      The id of the dog (the order of creation)
 *      The name of the dog
 */
void* dog(void* input) {
	struct args* this_dog = (struct args*) input;
	int time_drinking = (rand() % 5) + 1;
	// printf("start: is dog %s\n", this_dog->name);
	printf("%s the dog wants to drink for %d secs.\n", this_dog->name, time_drinking);
	
	// char name[20];
	// pthread_getname_np(pet, name, 20); 

	//wait 
	sem_wait(&waiting);
	sem_wait(&dogs_lock);
	dogs_in++;
	if(dogs_in == 1) {
		printf("\n%s the dog is waiting for kitchen to be empty.\n", this_dog->name);
		sem_wait(&empty);
	}
	sem_post(&dogs_lock);
	sem_post(&waiting);
	
	// totalPets++;
	sem_wait(&dogs_max);
	printf("%s the dog entered the kitchen.\n", this_dog->name); 
	dogs_drinking++;
	//critical section 
	
	sleep(time_drinking); 
	// printf("is dog %s\n", this_dog->name);
	// printf("cats_in = %d\n", cats_in);
	// printf("dogs_in = %d\n", dogs_in);
	// printf("cats_drinking = %d\n", cats_drinking);
	// printf("dogs_drinking = %d\n\n", dogs_drinking);
	
	sem_post(&dogs_max);
	dogs_drinking--;
	// printf("here\n");
	//signal 
	printf("%s the dog just exited the kitchen.\n", this_dog->name); 
	sem_wait(&dogs_lock);
	// printf("here 1\n");
	dogs_in--;
	if(dogs_in==0) sem_post(&empty);
	sem_post(&dogs_lock);
	// printf("here 2\n");
	// totalPets--;
} 

/*
 * Runs the simulation. There are 9 dogs and 13 cats that want
 * to drink water from one of two self-refilling bowls in the
 * kitchen. Cats and dogs can't be mixed - only one type can be
 * in the kitchen at a time. 
 */
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

	// random loop vars
	int i = 0;
	int num_cats = 13;//13;
	int num_dogs = 9;//9;
	int total_pets = num_cats + num_dogs;
	int random_num = 0;
	time_t t;
	struct args *pets_args[total_pets];

	// make list of empty threads
	pthread_t pets[total_pets];

	// make list of thread names ex. Loki
	char names[52][30] = {"Abby", "Bob", "Carl", "Doggo", "Egg", "Fredrickson", "Goat",
      "Harold", "Igloo", "Jerfy", "Kiki", "Lemon", "Maddy", "Nom", "Oprah", "Pineapple",
      "Queen", "Rice", "Spam", "Tom", "Unix", "Void", "War", "Xavier", "Yoyo", "Zebra",
      "Addison", "Bobbison", "Carlson", "Doggoson", "Eggson", "Fredricksonson", "Goatson",
      "Haroldson", "Iglooson", "Jerfyson", "Kikison", "Lemonson", "Maddison", "Nomson",
      "Oprahson", "Pineappleson", "Queenson", "Riceson", "Spamson", "Tomison", "Unixson",
      "Voidson", "Warson", "Xavierson", "Yoyoson", "Zebrason"
    };

	// make pets in random order
    srand((unsigned) time(&t));

    for (; i < total_pets; i++) {
    		// printf("this is pet number %d\n", i+1);
        pets_args[i] = (struct args *)malloc(sizeof(struct args));
        pets_args[i]->id = i;
        pets_args[i]->name = names[i];
        		// Pet->name 
        		// printf("the name of the pet is %s\n", Pet->name);

        if (num_cats == 0) {
    			// printf("no more cats, creating dog %d\n", 4 - num_dogs);
            pthread_create(&(pets[i]),NULL, (void*) &dog, pets_args[i]);
            num_dogs--;
        }
        else if (num_dogs == 0) {
        			// printf("no more dogs, creating cat %d\n", 5 - num_cats);
             pthread_create(&(pets[i]),NULL,(void*) &cat, pets_args[i]); 
             num_cats--;
        }
        // make cat
		else if (random_num % 2 == 0) {
			random_num = rand();
			// printf("making cat %d\n", 5 - num_cats);
			pthread_create(&(pets[i]),NULL,(void*) &cat, pets_args[i]); 
			num_cats--;
		}
        // make dog
		else {
			random_num = rand();
			// printf("making dog %d\n", 4 - num_dogs);
			pthread_create(&(pets[i]),NULL, (void*) &dog, pets_args[i]);
			num_dogs--;	
		}
	}

	// printf("attempting to join threads\n");
	i = 0;
	for (; i < total_pets; i++) {
		pthread_join(pets[i],NULL); 	
	}
	// pthread_join(t1,NULL); 
	// pthread_join(t2,NULL); 
	// printf("here\n");
	sem_destroy(&empty); 
	sem_destroy(&cats_lock); 
	sem_destroy(&dogs_lock); 
	sem_destroy(&cats_max);
	sem_destroy(&dogs_max);

	// free pet arg structs
	i = 0;
	for (; i < total_pets; i++) {
		free(pets_args[i]);
	}

	return 0; 
} 
