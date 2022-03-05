#include "elfoslib.h"

void printFileInfo(FILE *fp) {
  printf("File no: %x\n", fp->fd);
  printf("Flags: %x\n", fp->flag);
  printf("File buffer: %x\n", fp->base);
  printf("Char count: %d\n", fp->cnt);
  printf("Char ptr: %x\n",fp->ptr);
}//printFileInfo

void main(){
  char* fname1 = "fread.txt";
  char* fname2 = "out.txt";    
  FILE* fp1;
  FILE* fp2;
  int   c_test, i;
  char  verse[64];
  long  pos;
  
    
  fp1 = fopen(fname1, "r");
  fp2 = fopen(fname2, "w");  
  //pos = ftell(fp);
  //printf("File position: %ld\n", pos);
  
  if(fp1){
//    for (i=0; i<5; i++){
//      c_test = fgetc(fp1);
//      fputc(c_test, stdout);      
//    }//for
    
  i = fread(verse, 1, 52, fp1);

  printf("Bytes read: %d\n", i);
 
  if (fp2){
    fputs("Poem XIII:\n", fp2); 
    i = fwrite(verse, 1, i, fp2);
    printf("Bytes written: %d\n", i);
    fclose(fp1);
  }//if fp2
  
  fclose(fp2);
  //show verse on stdio
  i = fwrite(verse, 1, i, stdout);
  printf("characters: %d\n", i);
  }
  
  //Bad idea but it is possible use fread with stdin
  printf("Enter 6 characters:\n");
  fread(verse, 1, 6, stdin);
  //force a null at the end of six chars
  //Don't do as Donny Don't does....
  verse[6]=0;
  printf("\nEcho Test: %s\n",verse);  
  printf("\nDone!\n");
}//main
