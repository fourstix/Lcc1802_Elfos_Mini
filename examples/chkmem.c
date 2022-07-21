#include "elfoslib.h"

//function to get high memory limit from Elf/OS
unsigned int elfos_himem(void){
	return *((unsigned int *) 0x0442);
}

//low memory limit function
//unsigned int elfos_lomem(void) is defined in header

//function to get heap pointer from Elf/OS
unsigned int elfos_k_heap(void){
	return *((unsigned int *) 0x0468);
}

void main(){
	unsigned int hi, k, lo, mem;
	
	//show heap information
	hi = elfos_himem();	
	printf("Himem  = 0x%x\n", hi);
	lo = elfos_lomem();
	printf("Lowmem = 0x%x\n", lo);	
	k = elfos_k_heap();
	printf("K_Heap:  0x%x\n", k);
	
	if (k != lo) {
		printf("Error: K_heap does not point to bottom of heap.\n");
	}
	
	//find the remaining heap memory
	mem = hi - k;
	//use %ld since %u format is not available
	printf("Heap available for C: 0x%x or %ld bytes\n", mem, (long) mem);
}//main
