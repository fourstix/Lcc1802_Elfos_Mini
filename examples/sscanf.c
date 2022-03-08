//myscanf.c
#include "elfoslib.h"

void main(void) {
  char c1 = '.';
  char c2 = '.';
  int i = -1;
  int x;
  char text[10];
  float f;
  int n;
  char* p = "Ab 1000 Hello! -123.500 0FFF";
    
  n = sscanf(p, "%c%c %d %s %f %x", &c1, &c2, &i, text, &f, &x);
  printf("%d args: %c %c %d %s %f %x\n", n, c1, c2, i, text, f, x);
  }//main
