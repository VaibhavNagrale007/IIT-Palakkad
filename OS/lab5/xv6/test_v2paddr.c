#include "types.h"
#include "user.h"
#include "syscall.h"
#include "memlayout.h"

int main(){

	int intTest = 10;
	unsigned int *va = (unsigned int *) (&intTest);

	unsigned int pa;
	int st = v2paddr(&pa, va);

	printf(1, "status : %d \n", st);
	printf(1, "va -> pa : %x  ->  %x \n\n", va, pa);

	
	char charTest = 'c';
	va = (unsigned int *) (&charTest);
	st = v2paddr(&pa, va);

	printf(1, "status : %d \n", st);
	printf(1, "va -> pa : %x -> %x \n\n", va, pa);


	int arr[100];
	va = (unsigned int *) arr;
	st = v2paddr(&pa, va);

	printf(1, "status : %d \n", st);
	printf(1, "va -> pa : %x -> %x \n\n", va, pa);

	va = (void *) (KERNBASE + 100);
	st = v2paddr(&pa, va);

	printf(1, "status : %d \n", st);
	printf(1, "va -> pa : %x -> %x \n\n", va, pa);

	return 0;
}
