//myscanf.c
#include "elfoslib.h"

void main() {
  char* p_int = " 1000 ";
  char* p_flt = "1.234";
  char* n_int = "-200";
  char* n_flt = "-3.14";
  char* x_lc = "01ab";
  char* x_uc = "0xFEFE";
  char* p_ld = "128000";
  char* n_ld = "-128000";
  char* p_lx = "0xFEFE0101";

  int i;
  unsigned int u;
  float f;
  long  l;
  unsigned long x;
  
  i = atoi(p_int);
  printf("Int [%s] %d\n", p_int, i);
  i = atoi(n_int);
  printf("Int [%s] %d\n", n_int, i);
  u = atox(x_lc);
  printf("Hex [%s] %x\n", x_lc, u);
  u = atox(x_uc);
  printf("Hex [%s] %x\n", x_uc, u);
  
  f = atof(p_flt);
  printf("Float [%s] %f\n", p_flt, f);
  f = atof(n_flt);
  printf("Float [%s] %f\n", n_flt, f);
  
  l = atold(p_ld);
  printf("Long [%s] %ld\n", p_ld, l);
  l = atold(n_ld);
  printf("Long [%s] %ld\n", n_ld, l);
  x = atolx(p_lx);
  printf("Long [%s] %lx\n", p_lx, x);
}//main
