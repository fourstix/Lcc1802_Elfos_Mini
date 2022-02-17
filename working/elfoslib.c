#ifndef ELFOSLIB_C
#define ELFOSLIB_C
//single character output for elfos
void elfosputc(char c){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" glo  regArg1\n");
	//asm(" plo  r14\n"); // added instruction to set RE.0 to D
	asm(" sep  r4\n");
	asm(" dw 0x0330\n"); //the elfos routine will end with a return to my caller
	asm(" Cretn\n");//return to c
}

//single character input from elfos terminal
char elfosgetc(void){
	//Set up the RE.1 byte before call
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" sep  r4\n");
	asm(" dw  0x0336\n"); //call kernel read key function
	asm(" plo retVal\n");//put character in low byte
	asm(" ldi 0\n"); //clear out high byte
	asm(" phi retVal\n");
	asm(" Cretn\n");//return to c
}

//Read string from terminal (fixed buffer length of 256)
//returns 0 if input finished with <ENTER>
//returns 1 if input finished with <CTRL><C>
int elfosgets(char* s){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" glo regArg1\n");//put buffer ptr into rf for O_INPUT
	asm(" plo r15\n");
	asm(" ghi regArg1\n");
	asm(" phi r15\n");	
	asm(" sep r4\n");
	asm(" dw  0x0339\n"); //call kernel read key function
	asm(" ldi 0\n"); //get df flag for return
	asm(" phi retVal\n");//zero out hi byte
	asm(" shlc\n");//shift df into lo byte
	asm(" plo retVal\n");//put return into lo byte
	asm(" Cretn\n");//return to c
}

//get stack pointer
int elfos_sp(void){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r15,sp\n"); //copy stack pointer into return value
	asm(" Cretn\n");//return to c
}

//get K_LOWMEM value
int elfos_lomem(void){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" ld2 r15,'D',(0x0465),0\n");//get lomem value
	asm(" Cretn\n");//return to c
}

//Allocate a block of memory from the kernel
void * elfos_alloc(int size){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	// asm(" cpy2 r12, regArg1\n");
	//regArg1 is r12 and holds the size value
	asm(" ldaD r7, 0x00\n");//allocate temp non-aligned block
	asm(" sep  r4\n");//Call kernel alloc function
	asm(" dw  0x036c\n");//r15 has address to block after call
	asm(" lbnf $$alloc\n");//df = 0 means allocated ok
  asm(" ldaD r15, 0x0000\n");//set ptr to null
	asm("$$alloc: Cretn\n");//return ptr value in r15	
}

//De-allocate a block of memory from the kernel
void elfos_free(void* p){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r15, regArg1\n"); //put pointer into R15
	asm(" sep  r4\n");//Call kernel dealloc function
	asm(" dw  0x036f\n");
	asm(" Cretn\n");//return ptr value in r15	
}

//Test that pointer points to valid memory block
int elfos_valid_block(void* p){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" ldaD r15, 0xFFFF\n");//set r15 to true
	asm(" dec regArg1\n"); //back up ptr 3 byts to block type
	asm(" dec regArg1\n");
	asm(" dec regArg1\n");
	asm(" ldn regArg1\n");//get the block type value
	asm(" smi 0x02\n");//the only valid type to free is 0x02
	asm(" lbz $$valid\n");
	asm(" ldaD r15, 0x0000\n");//set r15 to false
	asm("$$valid: Cretn\n");//return result
}

/*
Initialize the file descriptor as follows:
fildes:     db      0,0,0,0
            dw      dta
            db      0,0
flags:      db      0
sector:     db      0,0,0,0
offset:     dw      0,0
            db      0,0,0,0
dta:        $       ; 512 bytes follow filedes area						
*/
void elfos_init_fd(int fd){
  asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r13, regArg1\n");//copy buff addr into r13 to calcualte dta
	asm(" glo  r13\n");//dta is 19 after filedes
	asm(" adi  19\n");
	asm(" plo  r13\n");//put lob byte into r13
	asm(" ghi  r13\n");//propagate carry into hi byte
	asm(" adci 0\n");//add carry bit to hi byte
	asm(" phi  r13\n");//r13 now points to dta
	asm(" ldi  0\n");
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n"); 
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n"); 
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n"); 
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n");
	asm(" ghi  r13\n");//put dta address next
	asm(" str  regArg1\n");
  asm(" inc  regArg1\n");
	asm(" glo  r13\n");//put dta address next
	asm(" str  regArg1\n");
  asm(" inc  regArg1\n");
	asm(" ldaD r15,0x0D\n");//next 13 bytes are zero
	asm("$$rpt: ldi  0\n");
	asm(" str  regArg1\n");//zero out next byte
	asm(" inc  regArg1\n");
	asm(" dec r15\n");//decrement counter
	asm(" glo r15\n");//check if counter has gone to zero
	asm(" lbnz $$rpt\n");//if not keep going
}

//Call the kernel file open function
int elfos_open_file(int fd, char *name, int flags){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" cpy2 r15, regArg2\n"); //put pointer to filename into R15
	asm(" cpy2 r13, regArg1\n"); //put pointer to file descriptor into r13
	asm(" ld2 r7,'O',sp,(6+3)\n"); //put pointer to flags into r7
	asm(" sep  r4\n");//Call kernel dealloc function
	asm(" dw   0x0306\n");//Call kernel open file function
  asm(" lbdf $$err\n");//DF=1 means error occured
  asm(" ldaD r15,0\n");//clear retval on success 
  asm(" lbr $$done\n");
  asm("$$err: plo r15\n");//put error code into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
	asm("$$done: popr r7\n");//retore reg variable to r7 
  asm(" Cretn\n"); 
}

//Call the kernel file close function
int elfos_close_file(int fd){
  asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r13, regArg1\n"); //put fd into RD
  asm(" sep  r4\n");//Call kernel dealloc function
  asm(" dw   0x0312\n");//Call kernel close file function
  asm(" lbdf $$err\n");//DF=1 means error occured
  asm(" ldaD r15,0\n");//clear retval on success 
  asm(" lbr $$done\n");
  asm("$$err: plo r15\n");//put error code into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
  asm("$$done: Cretn\n");//return value in r15
}

//Read bytes from the file
int elfos_read_file(int fd, char* buf, int count){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r15, regArg2\n"); //put pointer to buffer into R15
	asm(" cpy2 r13, regArg1\n"); //put pointer to file descriptor into r13
	asm(" ld2 r12,'O',sp,(6+1)\n"); //put pointer to count into r12
  asm(" sep  r4\n");//Call kernel dealloc function
  asm(" dw   0x0309\n");//Call kernel close file function
  asm(" lbdf $$err\n");//DF = 1 means error
  asm(" cpy2 r15, r12\n");//put count read into r15
  asm(" lbr $$done\n");//retval is count read
  asm("$$err: plo r15\n");//put error code in D into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
  asm("$$done: Cretn\n");
}

//Write bytes to the file
int elfos_write_file(int fd, char* buf, int count){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r15, regArg2\n"); //put pointer to buffer into R15
	asm(" cpy2 r13, regArg1\n"); //put pointer to file descriptor into r13
	asm(" ld2 r12,'O',sp,(6+1)\n"); //put pointer to count into r12
  asm(" sep  r4\n");//Call kernel dealloc function
  asm(" dw   0x030C\n");//Call kernel close file function
  asm(" lbdf $$err\n");//DF = 1 means error
  asm(" cpy2 r15, r12\n");//put count written into r15
  asm(" lbr $$done\n");//retval is count written
  asm("$$err: plo r15\n");//put error code in D into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
  asm("$$done: Cretn\n");
}

//uses pointer to pass and return a long value
long elfos_long_seek_file(int fd, long* offset, int origin){
  asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
  asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
  asm(" cpy2 r15, regArg2\n"); //put int offset value into r7
	asm(" cpy2 r13, regArg1\n"); //put file descriptor into r13
	asm(" ld2 r12,'O',sp,(6+2+1)\n"); //put origin value into r12
  asm(" lda r15\n");//get the hi byte of hiword
  asm(" phi r8\n");//put hiword into r8
  asm(" lda r15\n");//get the lo byte of hiword
  asm(" plo r8\n");//put hiword into r8
  asm(" lda r15\n");//get the hi byte of loword
  asm(" phi r7\n");//put loword into r7
  asm(" ldn r15\n");//get the hi byte of loword
  asm(" plo r7\n");//put loword into r7
  asm(" sep  r4\n");//Call kernel seek function
  asm(" dw   0x030f\n");//Call kernel close file function 
  asm(" lbdf $$err\n");//check df for failure
  asm(" cpy2 r12, r8\n");//set hiword of long retval
  asm(" cpy2 r13, r7\n");//set loword of long retval
  asm(" lbr $$done\n");//jump to return
  asm("$$err: plo r13\n");//put error code into retval loword
  asm(" ldi 0xFF\n");//set retval to negative value
  asm(" phi r13\n"); //make retval loword negative
  asm(" plo r12\n"); //make retval negative
  asm(" phi r12\n"); //make retval negative
  asm("$$done: popr r7\n");//retore reg variable to r7 
  asm(" Cretn\n"); //12:13 is long retval
}

//Function to free a block of memory
void free(void* p){
	int i = (int) p;  //cast to int for address comparisons
	int lomem = elfos_lomem() + 5; //must be above this
	int stack = elfos_sp(); //cannot be above stack
	if ((i != 0) && (i > lomem) && (i < stack)) {
		if(elfos_valid_block(p)) {
			elfos_free(p);	
		} //if elfos_valid_block
	} //if i 
}//free

//Allocate a block of memory from the heap
void* malloc(int size){
	void* p = elfos_alloc(size);
		
	if (p != 0) {
		int stack = elfos_sp();		
		int top = (int) p + size + 256; //top of block plus 1 page margin
		
	  //check that block is not too near stack
		if (top > stack) {
			//too close, free block and fail
			elfos_free(p);
			p = 0;
		} //if i > stack
	} //if p != 0
return p;
}//malloc()

//Allocate and clear a block of memory from the heap
void* calloc(int n, int size){
	int count = n * size;
	void* p = malloc(count);
	char* q = p;
	int i;
	
	if (q != 0) {
		//zero out all bytes in storage area
		for(i=0; i<count; i++) {
			q[i] = 0;
		}//for
	}//if q != 0
	return p;
}//calloc

//Unbuffered I/O function to open a file
int open(char* fname, int flags) {
  int retv = EOF;
  if (fname) {
    char* p = (char *) malloc(FD_SIZE);
    if (p) {
      int err;
      int fd = (int) p;
      elfos_init_fd(fd); //set up file descriptor
      err = elfos_open_file(fd, fname, flags);
      if (!err) {
        retv = (int) p;
      } else {
        free(p);
      }//if-else err     
    }//if p    
  }//if fname != NULL
  return retv;
}//open

//Unbuffered I/O function to close a file
int close(int fd) {
  int retv = EOF;
	//if fd is not null and not EOF
  if (fd && (fd != EOF)) {
    char* p = (char*) fd;
    int err = elfos_close_file(fd);
    free(p); //free file descriptor memory  
    if (!err) {
      retv = 0;
    }//if !err
  }//if fd not zero and not EOF
  return retv;
}//close

//Unbuffered I/O function for reading bytes
int read(int fd, char* buf, int count){
	return elfos_read_file(fd, buf, count);
}

//Unbuffered I/O function for writing bytes
int write(int fd, char* buf, int count){
	return elfos_write_file(fd, buf, count);
}

//takes a long value and returns a long value
long lseek(int fd, long offset, int origin){
  return elfos_long_seek_file(fd, &offset, origin);
}//lseek
#endif
