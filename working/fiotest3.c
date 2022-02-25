#include "elfoslib.h"

void main(){
  char* fname = "fwrite.txt";    
  FILE* fp;
  int   c_test;
    
  fp = fopen(fname, "r");  
  
  while((c_test = fgetc(fp)) != EOF) {
    //push back lowercase g as uppercase G
    if ((char) c_test == 'g') {
      ungetc('G', fp);
    } else {
      fputc(c_test, stdout);
    }//if
  }//while
  
  fclose(fp);
       
  printf("Stdin ungetc test:\n");
  printf("(x replaced by * and s by $)\n");
  printf("Enter q to quit.\n");
  while ((c_test = fgetc(stdin)) != 'q') {  
    if (c_test == 'x' || c_test == 'X') {
      ungetc('*', stdin);  
    } else if (c_test == 's' || c_test == 'S') {
      ungetc('$', stdin);  
    } else {
      fputc(c_test, stdout);
    } //if-else c_test
  } //while
  printf("\nDone!\n");
}//main
