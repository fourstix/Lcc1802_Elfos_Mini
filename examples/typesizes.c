#include "elfoslib.h"

void main(void){
  int i;
   
  i = sizeof(char);
  printf("Char size = %d\n", i);
  i = sizeof(int);
  printf("Int size = %d\n", i);
  i = sizeof(long);
  printf("Long size = %d\n", i);
  i = sizeof(char *);
  printf("Ptr size = %d\n", i);
  i = sizeof(float);
  printf("Float size = %d\n", i);
  i = sizeof(double);
  printf("Double size = %d\n", i);
}
