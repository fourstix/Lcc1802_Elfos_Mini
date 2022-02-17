#include "elfoslib.h"

//Debug function to print the header of the file descriptor
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
  char* fname = "create.new";
  char  data[20];
  int o_flgs = O_CREATE | O_TRUNCATE;
  int i, fd, n_writ;

  //Put test string into data buffer
  strcpy(data, "New File.");  
  
  fd = open(fname, o_flgs);
  printf("File create file result: 0x%x\n", fd);
    
  if (fd != EOF) {
    printf("FD addr: 0x%x\n", fd);
    printFdInfo(fd);
    //write characters and null into file 
    n_writ = elfos_write_file(fd, data, 10);
    printf("File Write result: 0x%x\n", n_writ);
    printFdInfo(fd);
    
    close(fd);      
    } else {
      printf("File create failed.\n");
    }    
}//main
