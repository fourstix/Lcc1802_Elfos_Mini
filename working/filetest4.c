#include "elfoslib.h"

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

void main(){
  char* fname = "poem.txt";
  char* pLine;
  int o_flgs = O_CREATE | O_APPEND;
  int fd, n_writ, size;

  //Put test string into data buffer
  pLine = "I heard a wise man say,\n";  
  size = strlen(pLine);
  
  fd = open(fname, o_flgs);
  printf("File open result: 0x%x\n", fd);
    
  if (fd != EOF) {
    printFdInfo(fd);
    //write characters and null into file 
    n_writ = write(fd, pLine, size);
    printf("Number of bytes written: %d\n", n_writ);

    printFdInfo(fd);
    
    pLine = "'Give pounds and crowns and guineas,\n";
    size = strlen(pLine);    

    //write characters and null into file 
    n_writ = write(fd, pLine, size);
    printf("Number of bytes written: %d\n", n_writ);

    printFdInfo(fd);
    
    pLine = "But never your heart away.'\n";
    size = strlen(pLine);
        
    //write characters and null into file 
    n_writ = write(fd, pLine, size);
    printf("Number of bytes written: %d\n", n_writ);

    printFdInfo(fd);

    close(fd);      
    } else {
      printf("File open failed.\n");
    }    
}//main
