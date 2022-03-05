/*
 * Sample file for fprintf
 */
#include "elfoslib.h"
char* fname = "fprint.txt";

void main(void) {
  char* hi = "Hello, Sailor!";
  float f = 1.23;
  FILE* fp;
  fp = fopen(fname, "w");
  
  if (fp){
  fprintf(fp, "String: %s\n", hi);
  fprintf(fp, "Int: %d\n", 42);
  fprintf(fp, "Char: %c:%cx\n", 'a','a');
  fprintf(fp, "Hex: %x\n", 42);
  fprintf(fp, "Long: %ld:%lx\n", 128000L, 128000L);
  fprintf(fp, "Float: %f\n", f);
  fprintf(fp, "A tropical year is %f days.\n", 365.24219);
  fclose(fp);
}//if fp
}//main
