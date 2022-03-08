//myscanf.c
#include "elfoslib.h"

void main(void) {
  char c1;
	char c2;
  int i;
	int x;
  char text[10];
  float f;
	char* fname = "scandata.txt";
  FILE* fp = fopen(fname, "r");

	if (fp) {
		int n = fscanf(fp, "%c%c %d %s %f %x", &c1, &c2, &i, text, &f, &x);
  	printf("%d args: %c %c %d %s %f %x\n", n, c1, c2, i, text, f, x);
		fclose(fp);
  }//if fp
}//main
