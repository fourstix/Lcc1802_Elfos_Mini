#include "elfoslib.h"

void printFdInfo(int p) {
  char* q = (char *) p;
  int i;
  
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

/*
 * Open and close a non-existent file
 */
void main(){
  char* fname = "noexist.bad";
  int i, fd;
  
  printf("Attempt to open a non-existent file.\n");
  fd = open(fname, O_OPEN);
  printf("File Open result: 0x%x\n", fd);
    
  if (fd == EOF) {
    printf("Expected error.\n");
  } else {
    printf("Unexpected result.\n");
  }
  
  printf("Attempt to close invalid EOF value.\n");
  fd = EOF;
  i = close(fd);
  printf("File Close result: 0x%x\n",i);
    
  if (i == EOF) {    
    printf("Expected result.\n");
  } else {
    printf("Unexpected result.\n");
  }//if-else

  printf("Attempt to close invalid NULL value.\n");
  fd = 0;
  i = close(fd);
  printf("File Close result: 0x%x\n",i);
    
  if (i == EOF) {    
    printf("Expected result.\n");
  } else {
    printf("Unexpected result.\n");
  }//if-else  
}//main
