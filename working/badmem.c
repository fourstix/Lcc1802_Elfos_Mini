#include "elfoslib.h"
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

//calculate C heap size
int elfos_heap_size(void) {
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" ld2 r12,'D',(0x0465),0\n");//set r12 to lomem value
	asm(" ldaD r15,LCCELFOSkHeap\n");//get original k_heap value
	asm(" inc r15\n");//point r15 to k_heap lo byte 	
	asm(" ldn r15\n");//get k_heap.0
	asm(" str r2\n");//save at M(X)
	asm(" glo r12\n");//get lomem.0
	asm(" sd\n");//subtract lomem.0 from k_heap.0
  asm(" plo r13\n");//put lo byte into r13.0
  asm(" dec r15\n");//point back to k_heap.1
	asm(" ldn r15\n");//get k_heap.1
	asm(" str r2\n");//save at M(X)
	asm(" ghi r12\n");//get lomem.1
	asm(" sdb\n");//subtract lomem.1 from k_heap.1 with borrow
  asm(" phi r13\n");//put hi byte into r15.1
	asm(" dec r13\n");//subtract 6 from size in r13
	asm(" dec r13\n");//six decrements is shorter than two byte subtraction
	asm(" dec r13\n");
	asm(" dec r13\n");
	asm(" dec r13\n");//r13 has the size value
	asm(" dec r13\n");//of (k_heap - lowmem - 6)
	asm(" cpy2 r15, r13\n");//copy size into return value
	asm(" Cretn\n");//return with value in r15
}
*/
unsigned int elfos_himem(void){
	return *((unsigned int *) 0x0442);
}

unsigned int elfos_k_heap(void){
	return *((unsigned int *) 0x0468);
}

unsigned int heapsize() {
	unsigned int hi_mem = elfos_himem();
	unsigned int lo_mem = elfos_lomem();
	unsigned int heap_size = (hi_mem - lo_mem) - 6;
	return heap_size;
}
//#define malloc elfos_alloc
//#define free elfos_free
#define FD_SIZE 531

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
	int i, j;
	char *b;
	char *addr[8];
	int too_low = elfos_lomem() - 100; //invalid addr below lowmem
	int too_high = elfos_sp() + 100; // invalid addr above stack pointer
	int waste[512];
	
	//fill a huge array to push down stack
	for (i=0; i<512; i++) {
		waste[i] = 0;
	}
	
	//show heap information before changes
  printHeapInfo(); 
	i = heapsize();
	printf("Heap available for C: %x\n", i);
	
	printf("Negative Test: Allocate 2k blocks until exhausted.\n");
  
	for(j=0; j<8; j++){
		b = malloc(2048);		
		addr[j] = b; //save it for free test
		i = (int) b; //cast pointer to int for printing
		printf("Block addr at %x\n", i);
		printHeapInfo();
	  }//for
	
	printf("Free addresses.\n");
	
	printf("Test data:\n");
	for(j=0; j<8; j++){
		b = addr[j]; //save it for free test
		i = (int) b;//cast pointer to int for printing
		printf("0x%x ", i);
		}//for
	printf("\n");
		
	for(j=0; j<8; j++){
		b = addr[j]; //save it for free test
		i = (int) b;//cast pointer to int for printing
		printf("Free block addr at %x\n", i);
		free(b);
		printHeapInfo();
	}//for
	printf("One more after free.\n");
	b = malloc(1024);		
	i = (int) b; //cast pointer to int for printing
	printf("Block addr at %x\n", i);
	printHeapInfo();
  free(b);

	printf("Negative Test: Free invalid addresses.\n");
  //point previous value of b to invalid value
	b++;
	free(b);
	b = (char *)too_low;
	free(b);
	b = (char *)too_high;
	free(b);
	
  printHeapInfo();	
}//main
