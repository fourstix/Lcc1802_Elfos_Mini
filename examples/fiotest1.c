#include "elfoslib.h"

/*
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

void printFileInfo(FILE* pf) {
   int i;
   if (pf) {
     i = (int) pf->base;    
     printf("Base:  0x%x\n", i);
     i = (int) pf->ptr;
     printf("Ptr:  0x%x\n", i);
     i = pf->flag;
     printf("Flag:  0x%x\n", i);
     i = pf->cnt;
     printf("Cnt:  %d\n", i);
     i = pf->fd;
     printf("Fd:  %d\n", i);
     if (i > 2) { //0,1,2 are std io handles
       printFDInfo(i);
     }
   } else {
     printf("File Pointer is NULL.\n");
   }
}//printFileInfo
*/

void main(){
  char* fname = "fread.txt";
  char* bye = "Good-bye!\n";
    
  FILE* fp = fopen(fname, "r");

  if (fp) {
    int c;
    
    while((c = fgetc(fp)) != EOF){
      fputc(c, stdout);
    }//while
    printf("\n");
    fclose(fp);  
    // printFileInfo(fp);
    
    printf("Stdio echo test: enter 'q' to quit:\n");
    while((c = fgetc(stdin)) != 'q') {
      fputc(c, stdout);
    }// while
    fputc('\n',stdout);    
    fputs(bye, stdout);        
  } else {
    printf("File open failed.\n");
  }//if-else fp
}//main
