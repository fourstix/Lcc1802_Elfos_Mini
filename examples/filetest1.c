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

void main(){
  char* fname = "test.txt";
  char  data[20];
  int   i, n_read, fd;

  //clear data buffer with nulls
  for(i=0; i<20; i++){
    data[i] = 0;
  }

  fd = open(fname, O_OPEN);
  printf("File Open result: 0x%x\n", fd);
    
  if (fd == EOF) {
    printf("File open failed.\n");
  } else {
    printFdInfo(fd);
    //read first four characters T,e,s,t
    n_read = read(fd, data, 4);
    printf("Number of bytes read: %d\n", n_read);
    printf("Date read: ");
    for(i=0; i<4; i++){
      printf("%c", data[i]);
    } //for
    printf("\n");
    
    printFdInfo(fd);
    
    i = close(fd);
    printf("File Close result: 0x%x\n",i);
    
    if (i == EOF) {    
      printf("File close failed.\n");
    }// if EOF
  }//if-else
}//main
