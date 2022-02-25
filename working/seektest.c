
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
