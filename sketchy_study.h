#ifndef HEADER_FILE
#define HEADER_FILE

int NUM_GRADS = 4;

typedef struct specs {
	int* ordered_list;
	int list_index;
	int list_size;
} specs;

typedef struct grad_args {
	int creation_num;
} grad_args;

int main();
specs* make_grad_specs();
void* grad(void* input);
int isDeadlock(int grad_id);
int isCrossDeadlock(specs* current, specs* other);
int isParallelDeadlock(specs* current, specs* other);

#endif