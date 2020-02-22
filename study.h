#ifndef HEADER_FILE
#define HEADER_FILE

int NUM_GRADS = 4;

typedef struct list {
	int* ordered_list;
	int list_index;
	int list_size;
} list;

typedef struct grad_specs {
	char* name;
	char** objects;
	int current_object;
	int num_objects;
} grad_specs;

int main();
void* grad(void* input);
int* make_random_list(int num);

#endif