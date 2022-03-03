/*
 * Sample file for sprintf
 */
#include "elfoslib.h"

void main(void) {
  char * p = (char *) calloc(256, 1);
  char * hi = "Hello, Sailor!";
  float f = 1.23;
  long  v = 256001L;
  
  if (p) {
    sprintf(p, "String: %s\n", hi);
    puts(p);
  
    sprintf(p, "Ints: %d %d %d\n", 42, 72, -1);
    puts(p);
 
    sprintf(p, "Char: %c:%cx\n", 'a','a');
    puts(p);
    
    sprintf(p, "Hex: %x\n", 42);
    puts(p);    

    sprintf(p, "Long: %ld:%lx\n", 128000L, 128000L);
    puts(p);
    
    sprintf(p, "Long: %ld\n",  v);
    puts(p);
    
    sprintf(p, "Float: %f\n", f);
    puts(p);
    
    sprintf(p, "A tropical year is %f days.", 365.24219);
    puts(p);
    
    free(p);
  }//if p
}//main
