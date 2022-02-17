
#include "elfoslib.h"
/*
//short version of seek
int elfos_short_seek_file(int fd, int offset, int origin){
  asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
  asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
  asm(" cpy2 r7, regArg2\n"); //put int offset value into r7
	asm(" cpy2 r13, regArg1\n"); //put file descriptor into r13
	asm(" ld2 r12,'O',sp,(6+3)\n"); //put origin value into r12
  asm(" ghi r7\n");//sign extend into r8
  asm(" shl\n");//shift msb into df
  asm(" lbdf $$neg\n");//if negative offset fill with FF
  asm(" ldaD r8,0\n");//if positive offset fill with 00
  asm(" lbr $$seek\n");//call kernel seek function 
  asm("$$neg: ldaD r8,0xFFFF\n");//sign extend negative offset
  asm("$$seek: sep  r4\n");//Call kernel seek function
  asm(" dw   0x030f\n");//Call kernel close file function 
  asm(" lbdf $$err\n");//check df for failure
  asm(" cpy2 r15, r7\n");//move r7 into retval
  asm(" lbr $$done\n");//jump to return
  asm("$$err: plo r15\n");//put error code into r15
  asm(" ldi 0xFF\n");//set position to negative value
  asm(" phi r15\n");
  asm("$$done: popr r7\n");//retore reg variable to r7 
  asm(" Cretn\n"); 
}
*/

//Debug function to print the header of the file descriptor
void printFdInfo(int p) {
  char* q = (char *) p;
  int i;
  
  printf("FD addr: 0x%x\n", p);  
  //print the first 19 bytes in filedes
  for(i=0; i<19; i++){
    char c = q[i]; //get byte as char
    //format bytes as shown in kernel documentation
    if (i == 4 || i == 6 || i==8 || i==9) {
      printf("\n");
    }
    if (i == 13 || i == 15) {
      printf("\n");
    }
    printf("%cx ", c);// %cx is hex value format for char      
  }//for
  printf("\n");  
}//printFdInfo


main(){
  char* fname = "poem.txt";  
  int  fd, i, pos, n_read;
  char data[20]; //buffer for reading character data
  
  //open file for reading
  fd = open(fname, O_OPEN);

   if(fd != EOF){
     //read next 5 chars
     n_read = read(fd, data, 5);
     data[n_read]= 0; //put null at end of data to make string
     
     printf("Next %d chars: %s\n", n_read, data);   
     printFdInfo(fd);
     printf("Moving ahead 6 characters.\n");
     pos = lseek(fd, 6, SEEK_CUR);
     printf("Cursor position is: %d\n", pos);
     printf("fd after call 0x%x\n", fd);
     printFdInfo(fd);
     //read next 3 chars
     n_read = read(fd, data, 3);
     data[n_read]= 0; //put null at end of data to make string     
     printf("Next %d chars: %s\n", n_read, data);
     printf("Moving to 7 characters back from end of file.\n");
     pos = lseek(fd, -7, SEEK_END);  
     printf("Cursor position is: %d\n", pos);
     printf("fd after call 0x%x\n", fd);
     printFdInfo(fd);     
     //read next 4 chars
     n_read = read(fd, data, 4);
     data[n_read]= 0; //put null at end of data to make string   
     printf("Next %d chars: %s\n", n_read, data); 
     printf("Move back to the beginning.\n");
     pos = lseek(fd, 0, SEEK_SET);  
     printf("Cursor position is: %d\n", pos);
     printf("fd after call 0x%x\n", fd);
     printFdInfo(fd);     
     //read next 5E chars
     n_read = read(fd, data, 5);
     data[n_read]= 0; //put null at end of data to make string   
     printf("Next %d chars: %s\n", n_read, data); 

     close(fd);
   } else {
     printf("File open failed.\n");
   } 
}//main
