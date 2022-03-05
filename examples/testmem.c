#include "elfoslib.h"

//Kernel functions
/*
int elfos_himem(){//get Himem value from Elf/os
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" ld2 r15,'D',(0x0442),0\n");//get himem value
	asm(" Cretn\n");//return to c
}

int elfos_heap_bottom(){//get K_HEAP value
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" ld2 r15,'D',(0x00468),0\n");//get hkeep value
	asm(" Cretn\n");//return to c
}
*/
unsigned int elfos_himem(void){
	return *((unsigned int *) 0x0442);
}

unsigned int elfos_k_heap(void){
	return *((unsigned int *) 0x0468);
}


void printHeapInfo(void) {
  int i;
	
	i = elfos_sp();
	printf("Stack Pointer at %x\n", i);
	i = elfos_himem();
	printf("Himem = %x\n", i);
	i = elfos_k_heap();
	printf("K_Heap: %x\n",i);
	i = elfos_lomem();
	printf("Lowmem =  %x\n", i);	
}//printHeapInfo

void main(){
	int i;
	char *b1, *b2;
	char *t = "true";
	char *f = "false";
	//show heap information before changes
  printHeapInfo(); 
	
	printf("Test: Allocate two 1k blocks on heap.\n");
	
	b1 = malloc(1024);		
	i = (int) b1; //cast pointer to int for printing
	printf("First block addr at %x\n", i);
	printHeapInfo();
	i = elfos_valid_block(b1);
	printf("Valid flag = %s\n", i ? t : f);

	b2 = malloc(1024);		
	i = (int) b2; //cast pointer to int for printing
	printf("Second block addr at %x\n", i);
	printHeapInfo();
	i = elfos_valid_block(b2);
	printf("Valid flag = %s\n", i ? t : f);

	printf("Free blocks.\n");
	free(b1);
  free(b2);
	printHeapInfo();
}//main
