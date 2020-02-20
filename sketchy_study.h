#ifndef HEADER_FILE
#define HEADER_FILE

int NUM_GRADS = 4;

struct args {
	int* ordered_list;
} args;

int main();
int* make_ordered_list();
void* thread();

#endif