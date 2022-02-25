#include "elfoslib.h"

void main(){
  char* fname = "fwrite.txt";
  char* bye = "good-bye!\n";
      
  FILE* fp;
  fp = fopen(fname, "w");
  
  if(fp) {  
    fputc('H', fp);
    fputc('e', fp);
    fputc('l', fp);
    fputc('l', fp);
    fputc('o', fp);
    fputc(',', fp);
    fputc(' ', fp);
    fputs(bye, fp);  
    fflush(fp);
    
    fputs("You say, 'Yes', I say, 'No'\n", fp);
    fputs("You say, 'Stop' and I say, 'go, go, go'\n", fp);
    fputs("Oh no,\nYou say, 'goodbye' and I say, 'Hello',\nHello, hello,\n", fp);
    fputs("I don't know why you say, 'goodbye', I say, 'Hello'\n", fp);
    fclose(fp);
  } else {
    printf("File open failed.\n");
  }//if fp
}//main
