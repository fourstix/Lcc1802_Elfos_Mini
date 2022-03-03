/*
 * Sample file for printf
 */
#include "elfoslib.h"

void main(void) {
  char* hi = "Hello, Sailor!";
  float f = 1.23;
  
  printf("String: %s\n", hi);
  printf("Int: %d\n", 42);
  printf("Char: %c:%cx\n", 'a','a');
  printf("Hex: %x\n", 42);
  printf("Long: %ld:%lx\n", 128000L, 128000L);
  printf("Float: %f\n", f);
  printf("A tropical year is %f days.\n", 365.24219);
}//main
