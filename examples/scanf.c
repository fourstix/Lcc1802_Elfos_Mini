//myscanf.c
#include "elfoslib.h"


void main(void) {
  char c1 = '.';
  char c2 = '.';
  int i = -1;
  char text[20];
  float f;
	long  v;
  
  

  printf("Enter %%c %%c:\n");
  scanf("%c %c", &c1, &c2);
  printf("\n[%c]:%cx [%c]:%cx\n", c1, c1, c2, c2);
  printf("Enter %%d:\n");
  scanf(" %d", &i);
  printf("\nInt:%d\n", i);
  printf("Enter %%x:\n");
  scanf("%x", &i);
  printf("\nHex: %x\n", i);
  printf("Enter string:\n");
  scanf("%s", text);
  printf("\n%s\n", text);
	printf("Enter float:\n");
  scanf("%f", &f);
  printf("\n%f\n", f);
  printf("Enter Long Hex (8 digits): ");
	scanf("%lx", &v);
	printf("\n%lx\n", v);
	printf("Enter long int: ");
	scanf("%ld", &v);
	printf("\n%ld\n", v);
  }//main
