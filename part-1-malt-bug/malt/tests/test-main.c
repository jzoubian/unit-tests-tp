#include <stdlib.h>
#include <string.h>

#define SIZE (10*1024*1024)

void call_c(void)
{
	void * ptr = malloc(2*SIZE);
	memset(ptr, 0, 2*SIZE);
	free(ptr);
}

void call_b(void)
{
	void * ptr = malloc(SIZE);
	memset(ptr, 0, SIZE);
	free(ptr);
	call_c();
}

void call_a(void)
{
	void * ptr = malloc(SIZE);
	memset(ptr, 0, SIZE);
	free(ptr);
	call_b();
}

int main(int argc, char ** argv)
{
	call_a();
}
