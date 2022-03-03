#ifndef ELFOSLIB_C
#define ELFOSLIB_C

/*
 * NTSDLIB.C 
 */
//jan 7 trying a new vararg approach for printf
//jan 29 cleaning up itoa and ltoa before trying double dabble
//feb 6 switched to double dabble for ltoa. ltoa and itoa now return char *
//feb 17 switched printstr to out(5,x) from out5(x) - putc/out5 still used elsewhere
//feb 27, adding memcmp & memcpy
//mar 28 including float in printf
//April 4 changing prtflt/ftoa to use dubdabx routine from ltoa, add digit count to dubdabx
//April 10 #ifdef nofloats used to exclude float code in nstdlib
//May 26 2013 added strncmp from apple source
//June 25 removed static keyword from strlen
//Sept 29 - adding include for assembly versions of strcpy, strcmp for dhrystone optimization
//Nov 23 added scungy %cx hack for printing single char as hex.
//jan 29 - changing out(5,x) to putc(x)
//Mar 14 2017 inserting conditional NOP's into printstr, printf to slow down on 1806
//17-10-19 register variable declaration in printf
//20-05-22 rearranged dec/inc in memset
 int strncmp(const char *s1, const char *s2, unsigned int n)
/* ANSI sez:
 *   The `strncmp' function compares not more than `n' characters (characters
 *   that follow a null character are not compared) from the array pointed to
 *   by `s1' to the array pointed to by `s2'.
 *   The `strncmp' function returns an integer greater than, equal to, or less
 *   than zero, according as the possibly null-terminated array pointed to by
 *   `s1' is greater than, equal to, or less than the possibly null-terminated
 *   array pointed to by `s2'.  [4.11.4.4]
 */
{
    for ( ; n > 0; s1++, s2++, --n)
	if (*s1 != *s2)
	    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
	else if (*s1 == '\0')
	    return 0;
    return 0;
}

unsigned int strlen(char *str)
{
	unsigned int slen = 0 ;
	while (*str != 0) {
      slen++ ;
      str++ ;
   }
   return slen;
}

char * strcat(char *dst, char *src){
	char *d;

	if (!dst || !src)
		return (dst);

	d = dst;
	for (; *d; d++);
  
	for (; *src; src++){
		*d++ = *src;
  }
	*d = 0;
	return (dst);
}

char* sprintstr(char* p, char *s){
    //p points to end of buffered
    for (; *s; s++){
  		*p++ = *s;
    }
  	*p = 0;
    strcat(p, s);
    //return ptr to end
    return p;
}//sprintstr

#ifndef nofloats
static const float round_nums[8] = {
   0.5,
   0.05,
   0.005,
   0.0005,
   0.00005,
   0.000005,
   0.0000005,
   0.00000005
} ;
static const float mult_nums[8] = {
   1,
   10,
   100,
   1000,
   10000,
   100000,
   1000000,
   10000000
};
char * ftoa(float flt, char *outbfr,unsigned int dec_digits)
{
#define use_leading_plus 0
	unsigned int idx;
	float mult;
	long wholeNum,decimalNum;
   char *output = outbfr ;
   char tbfr[40] ;
   //*******************************************
   //  extract negative info
   //*******************************************
   if (flt < 0.0) {
      *output++ = '-' ;
      flt *= -1.0 ;
   } else {
      if (use_leading_plus) {
         *output++ = '+' ;
      }
   }

   //  handling rounding by adding .5LSB to the floating-point data
   if (dec_digits < 8) {
      flt += round_nums[dec_digits] ;
   }

   //**************************************************************************
   //  get fractional multiplier for specified number of digits.
   //**************************************************************************
	mult=mult_nums[dec_digits];

   wholeNum = flt;
   decimalNum = ((flt - wholeNum) * mult);
	//printf("whole=%ld,fltdec=%lx, decimal=%ld\n",wholeNum,fltdec, decimalNum);

   //*******************************************
   //  convert integer portion
   //*******************************************
   strcpy(output,dubdabx(wholeNum,output,1));
   output+=strlen(output);
   // printf("%.3f: whole=%s, dec=%d\n", flt, tbfr, decimalNum) ;
   if (dec_digits > 0) {
		*output++ = '.' ;
		//*******************************************
		//  convert fractional portion
		//*******************************************
		strcpy(output,dubdabx(decimalNum,output,dec_digits));
	}

   return outbfr;
}
#endif
char * itoa(int s, char *buffer){ //convert an integer to printable ascii in a buffer supplied by the caller
	unsigned int r,k,n;
	unsigned int flag=0;
	char * bptr; bptr=buffer;
	if (s<0){
		*bptr='-';bptr++;
		n=-s;
	} else{
		n=s;
	}
	k=10000;
	while(k>0){
		for(r=0;k<=n;r++,n-=k); // was r=n/k
		if (flag || r>0||k==1){
			*bptr=('0'+r);bptr++;
			flag='y';
		}
		//n=n-r*k;
		k=k/10;
	}

	*bptr='\0';
	return buffer;
}
char * ltoa(long s, char *buffer){ //convert a long integer to printable ascii in a buffer supplied by the caller
	char* bptr=buffer;
	if (s<0){
		*bptr++='-';
		s=-s;
	}
	strcpy(bptr,dubdabx(s,bptr,1)); //uses assembler double-dabble routine
	return buffer;
}

char* sprintint(char* p, int s){ //print an integer
	char buffer[8];
	itoa(s,buffer);
	//return pointer to end 
  return sprintstr(p, buffer);
}

char* sprintlint(char* p, long s){ //print a long integer
	char buffer[12];
	ltoa(s, buffer);
  return sprintstr(p, buffer);
}

#ifndef nofloats

char* sprintflt(char* p, float s){ //print a float
	char buffer[20]; //need room for two longs and the .
  ftoa(s, buffer, 3);
	return sprintstr(p, buffer);
}

#endif

char* sputxn(char *p, unsigned char x){ //print a nibble as ascii hex
	if (x<10){
		*p++ = (x+'0');
	} else {
		*p++ = (x+'A'-10);
	}
  //make sure it ends with null
  *p = 0;
  return p;
}//sputxn
char* sputx(char* p, unsigned char x){ //print a unsigned char as ascii hex
	p = sputxn(p, x>>4);
	p = sputxn(p, x & 0x0F);
  *p = 0;
  //return pointer to end of buffer
  return p;
}//sputx

char* sputdx(char* p, unsigned int d) {//print an interger hex value
  p = sputx(p, d >> 8);
  p = sputx(p, d & 255);
  *p = 0;
  return p;
}//sputdx

char* sputlx(char* p, unsigned long x) {//print an interger hex value
  unsigned int  lo = (unsigned int)(x & 0x0000FFFFL);
  unsigned int  hi = (unsigned int)(x >> 16);
  p = sputdx(p, hi);
  p = sputdx(p, lo);
  *p = 0;
  return p;
}//sputdx

/******************************************************************************
 *  vsprintf - variable sprintf routine
 *    buf - ptr to character buffer
 *    fmt - ptr to format string 
 *    arglist - ptr to argument list 
 *    argslot - int position of fmt string in caller (1 based)
 *  returns - int number of arguments processed
 *
 *  vsprintf supports a simplified set of formats without width or qualifiers
 *    %d or %i - print an integer value 
 *    %x or %X - print an integer value as hex 
 *    %s - print a string 
 *    %c - print single character value
 *    %cx - ptint a single character as hex
 *    %ld - print a long value as ascii
 *    %lx - print long value as hex 
 *    %f - print a float value with 3 decimal places
 ******************************************************************************/
int vsprintf(char* buf, char *fmt, int* arglist, int argslot){ //limited implementation of printf
//								supports only c,d,s,x,l formats without width or other qualifiers
  //int argslot = 0;	//used to align longs
  //int * this=(int *)&pptr;
  unsigned char c,xord;
	register char* ptr = fmt; //try to save on loads/spills
  int rc = 0; //return code is the number of arguements processed
  
  //advance argument pointer to first argument in list
  arglist++; 

 //process format string and consume arguments from list
  while(*ptr) {
		c = *ptr++;
		if (c!='%'){
			*buf++ = c;
      *buf = 0; //make sure buffer terminates as string
			//asm(" nop1806\n nop1806\n nop1806\n"); //17-03-13
		} else{
			c = *ptr++;
			switch (c){
				case 'i': case 'd': 
          buf = sprintint(buf, *arglist++);
          argslot++;
          rc++;
				break;
        
				case 's': 
					buf = sprintstr(buf, (char *) *arglist++);
					argslot++;
          rc++;
        break;

				case 'c'://scungy support for hex printing as %cx
					if (*ptr=='x'){ //if there's an x
						ptr++; //skip over the x
            //print 1 byte as hex
						buf = sputx(buf, ((unsigned char) (*arglist++)&255)); 
					} else{
					  //print as char
          	*buf++ = (char)(*arglist++);
      		  *buf = 0;
					}
          argslot++;
          rc++;
				break;
          
				case 'x': case 'X': 
		      buf = sputdx(buf, *arglist++);					
					argslot++;
          rc++;
				break;

				case 'l': //longs can be dec or hex
          //long values must align on an even boundary
          if (argslot&1) {
            arglist++;
            argslot++;
          }//if argslot is odd
          xord = *ptr++;
					if (xord == 'd'){
  					buf = sprintlint(buf, *(long *) arglist);//treat as a pointer to long												  
          } else{
            buf = sputlx(buf, *(long *) arglist);            
          }//if-else xord = d        
        //longs consume two slots on arg list   
        arglist += 2;  
        argslot += 2;
        rc++;
        break;
  
#ifndef nofloats
				case 'f': //float
          //flotas must align on an even boundary
          if (argslot&1){
            arglist++;
            argslot++;
          }//if argslot is odd
          buf = sprintflt(buf, *(float *) arglist);//treat as a pointer to float
          //longs consume two slots on arg list
          arglist += 2;
          argslot += 2;
          rc++;
        break;
#endif

				default:
					*buf++ = '%';
          *buf++ = c;
          *buf = 0; //buffer always ends with null
        break;  
			} //switch
		} //%
	} //while
  return rc;
} //vsprintf

/******************************************************************************
 *  sprintf - sprintf routine to print to a character buffer
 *    buf - ptr to character buffer
 *    fmt - format string 
 *  returns - int number of arguments processed
 *
 *  sprintf supports a simplified set of formats without width or qualifiers
 *    %d or %i - print an integer value 
 *    %x or %X - print an integer value as hex 
 *    %s - print a string 
 *    %c - print single character value
 *    %cx - ptint a single character as hex
 *    %ld - print a long value as ascii
 *    %lx - print long value as hex 
 *    %f - print a float value with 3 decimal places
 ******************************************************************************/
int sprintf(char * buffer, char * fmt, ...) {
  int n = 0;
  int* args = (int *) &fmt;
  //format string is second arguement in caller arg list
  n = vsprintf(buffer, fmt, args, 2);
  return n;
}//sprintf

/******************************************************************************
 *  printf - printf routine to print to std out
 *    fmt - format string 
 *  returns - int number of arguments processed
 *
 *  printf supports a simplified set of formats without width or qualifiers
 *    %d or %i - print an integer value 
 *    %x or %X - print an integer value as hex 
 *    %s - print a string 
 *    %c - print single character value
 *    %cx - ptint a single character as hex
 *    %ld - print a long value as ascii
 *    %lx - print long value as hex 
 *    %f - print a float value with 3 decimal places
 ******************************************************************************/
int printf(char *fmt, ...){
  int n = 0;
  int* args = (int *) &fmt;
  char * p = (char *) calloc(256, 1);
  if (p) {
    //format string is first arguement in caller arg list
    n = vsprintf(p, fmt, args, 1);
    puts(p);
    free(p);
    }//if p
    return n;
}//printf

/******************************************************************************
 *  fprintf - fprintf routine to print to a file
 *    fp - FILE * ptr created with fopen()
 *    fmt - format string 
 *  returns - int number of arguments processed or EOF if a file error occurs
 *
 *  fprintf supports a simplified set of formats without width or qualifiers
 *    %d or %i - print an integer value 
 *    %x or %X - print an integer value as hex 
 *    %s - print a string 
 *    %c - print single character value
 *    %cx - ptint a single character as hex
 *    %ld - print a long value as ascii
 *    %lx - print long value as hex 
 *    %f - print a float value with 3 decimal places
 ******************************************************************************/

int fprintf (FILE *fp, char *fmt, ...) {
  int n = 0;
  int rc = 0;
  int* args = (int *) &fmt;
  char * p = (char *) calloc(256, 1);
  
  if (p) {
    int rc;
    //format string is second arguement in caller arg list
    n = vsprintf(p, fmt, args, 2);
    rc = fputs(p, fp);
    free(p);
    }//if p
  return (rc == EOF) ? EOF : n;
}//fprintf

/*
void printf(char *pptr,...){ //limited implementation of printf
//								supports only c,d,s,x,l formats without width or other qualifiers
	unsigned char c,xord;
	register char* ptr=pptr; //try to save on loads/spills
	int argslot=0;	//used to align longs
	int * this=(int *)&pptr;
	this++; argslot++; //advance argument pointer and slot #
    while(*ptr) {
		c=*ptr; ptr++;
		if (c!='%'){
			putc(c);
			asm(" nop1806\n nop1806\n nop1806\n"); //17-03-13
		} else{
			c=*ptr;ptr++;
			switch (c){
				case 'i': case 'd':
					printint(*this++);
					argslot+=1; //next argument slot
					break;
				case 's':
					printstr((char*) *this++);
					argslot+=1; //next argument slot
					break;
				case 'c'://scungy support for hex printing as %cx
					if (*ptr=='x'){ //if there's an x
						ptr++; //skip over the x
						putx(((unsigned int) *this++)&255); //print 1 byte as hex
					} else{
						putc((unsigned int) *this++);		//print as char
					}
					argslot+=1; //next argument slot
					break;
				case 'x': case 'X':
					putx(((unsigned int) *this)>>8);
					putx(((unsigned int) *this++)&255);
					argslot+=1; //next argument slot
					break;
				case 'l': //longs can be dec or hex
					if (*ptr){ //as long as there's something there
						xord=*ptr++;
						if (argslot&1) {
							this++;
							argslot++;
						}
						if(xord=='d'){
							printlint(*(long *)this);//treats "this" as a pointer to long
							this+=2;				// and advances it 4 bytes
						} else{
							putx(((unsigned int) *this)>>8);
							putx(((unsigned int) *this++)&255);
							putx(((unsigned int) *this)>>8);
							putx(((unsigned int) *this++)&255);
						}
						argslot+=2;
						break;
					}
#ifndef nofloats
				case 'f': //float
					if (*ptr){ //as long as there's something there
						if (argslot&1) { //adjust alignment
							this++;
							argslot++;
						}
						printflt(*(float *)this);//treats "this" as a pointer to float
						this+=2;				// and advances it 4 bytes
						argslot+=2;
						break;
					}
#endif
				default:
					putc('%');putc(c);
			} //switch
		} //%
	} //while
} //prtf

void exit(int code){
	printf("exit %d\n",code);
	while(1);
}
*/
int memcmp(const void *Ptr1, const void *Ptr2, unsigned int Count){
	unsigned char* p1; unsigned char *p2;
    int v = 0;
    p1 = (unsigned char *)Ptr1;
    p2 = (unsigned char *)Ptr2;

    while(Count-- > 0 && v == 0) {
        v = *(p1++) - *(p2++);
    }

    return v;
}

void* memcpy(void* dest, const void* src, unsigned int count) {
        char* dst8 = (char*)dest;
        char* src8 = (char*)src;

        while (count--) {
            *dst8++ = *src8++;
        }
        return dest;
    }

void *memset(void *s, int c, unsigned int n) //sets memory at s to c for n bytes
{
    unsigned char* p=s;
    while(n){
        *p = (unsigned char)c;
        p++;
        n--;
	}
    return s;
}
void nstdlibincluder(){
	asm("\tinclude nstdlib.inc\n"); //strcpy, strcmp
}

//single character output for elfos
void elfosputc(char c){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" glo  regArg1\n");
	//asm(" plo  r14\n"); // added instruction to set RE.0 to D
	asm(" sep  r4\n");
	asm(" dw 0x0330\n"); //the elfos routine will end with a return to my caller
	asm(" popr r7\n");//retore reg variable to r7 
	asm(" Cretn\n");//return to c
}

//single character input from elfos terminal
int elfosgetc(void){
	//Set up the RE.1 byte before call
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" sep  r4\n");
	asm(" dw  0x0336\n"); //call kernel read key function
	asm(" plo retVal\n");//put character in low byte
	asm(" ldi 0\n"); //clear out high byte
	asm(" phi retVal\n");
	asm(" popr r7\n");//retore reg variable to r7 
	asm(" Cretn\n");//return to c
}

//Read string from terminal (max length of 256)
char* elfosgets(char* s){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" glo regArg1\n");//put buffer ptr into rf for O_INPUT
	asm(" plo r15\n");
	asm(" ghi regArg1\n");
	asm(" phi r15\n");
	asm(" pushr r15\n");//save ptr to beginning of buffer	
	asm(" sep r4\n");
	asm(" dw  0x0339\n"); //call kernel read key function
	asm(" popr r15\n");//restore pointer to buffer for retval
	asm(" popr r7\n");//retore reg variable to r7 
	asm(" Cretn\n");//return to c
}

//Write a string to terminal
int elfosputs(char* s){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" glo regArg1\n");//put buffer ptr into rf for O_INPUT
	asm(" plo r15\n");
	asm(" ghi regArg1\n");
	asm(" phi r15\n");
	asm(" sep r4\n");
	asm(" dw  0x0333\n"); //call kernel read key function
	asm(" ldi 0\n");//non-negative return value
	asm(" phi retVal\n");
	asm(" plo retVal\n");
	asm(" popr r7\n");//retore reg variable to r7 
	asm(" Cretn\n");//return to c
}

//get stack pointer
unsigned int elfos_sp(void){
	//no elfos api are called so need to restore RE.1
	//asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r15,sp\n"); //copy stack pointer into return value
	asm(" Cretn\n");//return to c
}

//get K_LOWMEM value
/*
int elfos_lomem(void){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" ld2 r15,'D',(0x0465),0\n");//get lomem value
	asm(" Cretn\n");//return to c
}
*/

//Allocate a block of memory from the kernel
void * elfos_alloc(int size){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	// asm(" cpy2 r12, regArg1\n");
	//regArg1 is r12 and holds the size value
	asm(" ldaD r7, 0x00\n");//allocate temp non-aligned block
	asm(" sep  r4\n");//Call kernel alloc function
	asm(" dw  0x036c\n");//r15 has address to block after call
	asm(" lbnf $$alloc\n");//df = 0 means allocated ok
  asm(" ldaD r15, 0x0000\n");//set ptr to null
	asm("$$alloc: popr r7\n");//retore reg variable to r7 
	asm(" Cretn\n");//return ptr value in r15	
}

//De-allocate a block of memory from the kernel
void elfos_free(void* p){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" cpy2 r15, regArg1\n"); //put pointer into R15
	asm(" sep  r4\n");//Call kernel dealloc function
	asm(" dw  0x036f\n");
	asm(" popr r7\n");//retore reg variable to r7
	asm(" Cretn\n");//return ptr value in r15	
}
/*
//Test that pointer points to valid memory block
int elfos_valid_block(void* p){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" ldaD r15, 0xFFFF\n");//set r15 to true
	asm(" dec regArg1\n"); //back up ptr 3 byts to block type
	asm(" dec regArg1\n");
	asm(" dec regArg1\n");
	asm(" ldn regArg1\n");//get the block type value
	asm(" smi 0x02\n");//the only valid type to free is 0x02
	asm(" lbz $$valid\n");
	asm(" ldaD r15, 0x0000\n");//set r15 to false
	asm("$$valid: Cretn\n");//return result
}
*/


/*
void elfos_init_fd(int fd){
  asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" cpy2 r13, regArg1\n");//copy buff addr into r13 to calcualte dta
	asm(" glo  r13\n");//dta is 19 after filedes
	asm(" adi  19\n");
	asm(" plo  r13\n");//put lob byte into r13
	asm(" ghi  r13\n");//propagate carry into hi byte
	asm(" adci 0\n");//add carry bit to hi byte
	asm(" phi  r13\n");//r13 now points to dta
	asm(" ldi  0\n");
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n"); 
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n"); 
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n"); 
	asm(" str  regArg1\n");//first four bytes are zero
	asm(" inc  regArg1\n");
	asm(" ghi  r13\n");//put dta address next
	asm(" str  regArg1\n");
  asm(" inc  regArg1\n");
	asm(" glo  r13\n");//put dta address next
	asm(" str  regArg1\n");
  asm(" inc  regArg1\n");
	asm(" ldaD r15,0x0D\n");//next 13 bytes are zero
	asm("$$rpt: ldi  0\n");
	asm(" str  regArg1\n");//zero out next byte
	asm(" inc  regArg1\n");
	asm(" dec r15\n");//decrement counter
	asm(" glo r15\n");//check if counter has gone to zero
	asm(" lbnz $$rpt\n");//if not keep going
}
*/

//Call the kernel file open function
int elfos_open_file(int fd, char *name, int flags){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" cpy2 r15, regArg2\n"); //put pointer to filename into R15
	asm(" cpy2 r13, regArg1\n"); //put pointer to file descriptor into r13
	asm(" ld2 r7,'O',sp,(6+3)\n"); //put pointer to flags into r7
	asm(" sep  r4\n");//Call kernel dealloc function
	asm(" dw   0x0306\n");//Call kernel open file function
  asm(" lbdf $$err\n");//DF=1 means error occured
  asm(" ldaD r15,0\n");//clear retval on success 
  asm(" lbr $$done\n");
  asm("$$err: plo r15\n");//put error code into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
	asm("$$done: popr r7\n");//retore reg variable to r7 
  asm(" Cretn\n"); 
}

//Call the kernel file close function
int elfos_close_file(int fd){
  asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" cpy2 r13, regArg1\n"); //put fd into RD
  asm(" sep  r4\n");//Call kernel dealloc function
  asm(" dw   0x0312\n");//Call kernel close file function
  asm(" lbdf $$err\n");//DF=1 means error occured
  asm(" ldaD r15,0\n");//clear retval on success 
  asm(" lbr $$done\n");
  asm("$$err: plo r15\n");//put error code into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
	asm("$$done: popr r7\n");//retore reg variable to r7
  asm(" Cretn\n");//return value in r15
}

//Read bytes from the file
int elfos_read_file(int fd, char* buf, int count){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" cpy2 r15, regArg2\n"); //put pointer to buffer into R15
	asm(" cpy2 r13, regArg1\n"); //put pointer to file descriptor into r13
	asm(" ld2 r12,'O',sp,(6+3)\n"); //put pointer to count into r12
  asm(" sep  r4\n");//Call kernel dealloc function
  asm(" dw   0x0309\n");//Call kernel close file function
  asm(" lbdf $$err\n");//DF = 1 means error
  asm(" cpy2 r15, r12\n");//put count read into r15
  asm(" lbr $$done\n");//retval is count read
  asm("$$err: plo r15\n");//put error code in D into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
	asm("$$done: popr r7\n");//retore reg variable to r7
  asm(" Cretn\n");
}

//Write bytes to the file
int elfos_write_file(int fd, char* buf, int count){
	asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
	asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
	asm(" cpy2 r15, regArg2\n"); //put pointer to buffer into R15
	asm(" cpy2 r13, regArg1\n"); //put pointer to file descriptor into r13
	asm(" ld2 r12,'O',sp,(6+3)\n"); //put pointer to count into r12
  asm(" sep  r4\n");//Call kernel dealloc function
  asm(" dw   0x030C\n");//Call kernel close file function
  asm(" lbdf $$err\n");//DF = 1 means error
  asm(" cpy2 r15, r12\n");//put count written into r15
  asm(" lbr $$done\n");//retval is count written
  asm("$$err: plo r15\n");//put error code in D into r15
  asm(" ldi 0xFF\n");//set retval to negative
  asm(" phi r15\n");
	asm("$$done: popr r7\n");//retore reg variable to r7
  asm(" Cretn\n");
}

//uses pointer to pass and return a long value
long elfos_long_seek_file(int fd, long* offset, int origin){
  asm(" ldaD r15,LCCELFOSsavRE\n ldn r15\n phi r14\n");
  asm(" pushr r7\n");//save reg variable in r7 (sp += 2)
  asm(" cpy2 r15, regArg2\n"); //put int offset value into r7
	asm(" cpy2 r13, regArg1\n"); //put file descriptor into r13
	asm(" ld2 r12,'O',sp,(6+2+1)\n"); //put origin value into r12
  asm(" lda r15\n");//get the hi byte of hiword
  asm(" phi r8\n");//put hiword into r8
  asm(" lda r15\n");//get the lo byte of hiword
  asm(" plo r8\n");//put hiword into r8
  asm(" lda r15\n");//get the hi byte of loword
  asm(" phi r7\n");//put loword into r7
  asm(" ldn r15\n");//get the hi byte of loword
  asm(" plo r7\n");//put loword into r7
  asm(" sep  r4\n");//Call kernel seek function
  asm(" dw   0x030f\n");//Call kernel close file function 
  asm(" lbdf $$err\n");//check df for failure
  asm(" cpy2 r12, r8\n");//set hiword of long retval
  asm(" cpy2 r13, r7\n");//set loword of long retval
  asm(" lbr $$done\n");//jump to return
  asm("$$err: plo r13\n");//put error code into retval loword
  asm(" ldi 0xFF\n");//set retval to negative value
  asm(" phi r13\n"); //make retval loword negative
  asm(" plo r12\n"); //make retval negative
  asm(" phi r12\n"); //make retval negative
  asm("$$done: popr r7\n");//retore reg variable to r7 
  asm(" Cretn\n"); //12:13 is long retval
}

//get K_LOWMEM value
unsigned int elfos_lomem(void){
  return *((unsigned int *) 0x0465);
}//elf_lomem

//Test that pointer points to valid memory block
int elfos_valid_block(void* p){
	char *ptr = ((char *) p) - 3; //block type located 3 bytes before block
	return (*ptr == 0x02); //only type 2 is valid, anything else is invalid 
}//elfos_valid_block

/*
Initialize the file descriptor as follows:
fildes:     db      0,0,0,0
            dw      dta
            db      0,0
flags:      db      0
sector:     db      0,0,0,0
offset:     dw      0,0
            db      0,0,0,0
dta:        $       ; 512 bytes follow filedes area						
*/
void elfos_init_fd(int fd){
	char *p = (char *)fd;
	int  dta = fd + 19; //dta is 19 bytes after filedes
	unsigned char hi_dta = (dta & 0xFF00) >> 8;
	unsigned char lo_dta = (dta & 0x00FF);
	int i;
	
	//clear first 4 bytes
	for(i=0; i< 4; i++) {
		*p++ = 0;
	}
	//next two bytes are dta address
	*p++ = hi_dta;
	*p++ = lo_dta;	
	//the rest of filedes is zero initially
	for(i=0; i< 13; i++) {
		*p++ = 0;
	}//for	
}//elfos_init_fd

/*
 * C Memory allocate and free functions
 */
//Function to free a block of memory
void free(void* p){
	unsigned int i = (unsigned int) p;  //cast to int for address comparisons
	unsigned int lomem = elfos_lomem() + 5; //must be above this
	unsigned int stack = elfos_sp(); //cannot be above stack
	if ((i != 0) && (i > lomem) && (i < stack)) {
		if(elfos_valid_block(p)) {
			elfos_free(p);	
		} //if elfos_valid_block
	} //if i 
}//free

//Allocate a block of memory from the heap
void* malloc(int size){
	void* p = elfos_alloc(size);
		
	if (p != 0) {
		unsigned int stack = elfos_sp();		
		unsigned int top = (int) p + size + 256; //top of block plus 1 page margin
		
	  //check that block is not too near stack
		if (top > stack) {
			//too close, free block and fail
			elfos_free(p);
			p = 0;
		} //if i > stack
	} //if p != 0
return p;
}//malloc()

//Allocate and clear a block of memory from the heap
void* calloc(int n, int size){
	unsigned int count = n * size;
	void* p = malloc(count);
	char* q = p;
	unsigned int i;
	
	if (q != 0) {
		//zero out all bytes in storage area
		for(i=0; i<count; i++) {
			q[i] = 0;
		}//for
	}//if q != 0
	return p;
}//calloc

/*
 * Unbuffered File I/O functions
 */
//Unbuffered I/O function to open a file
int open(char* fname, int flags) {
  int retv = EOF;
  if (fname) {
    char* p = (char *) malloc(FD_SIZE);
    if (p) {
      int err;
      int fd = (int) p;
      elfos_init_fd(fd); //set up file descriptor
      err = elfos_open_file(fd, fname, flags);
      if (!err) {
        retv = (int) p;
      } else {
        free(p);
      }//if-else err     
    }//if p    
  }//if fname != NULL
  return retv;
}//open

//Unbuffered I/O function to close a file
int close(int fd) {
  int retv = EOF;
	//if fd is not null and not EOF
  if (fd && (fd != EOF)) {
    char* p = (char*) fd;
    int err = elfos_close_file(fd);
    free(p); //free file descriptor memory  
    if (!err) {
      retv = 0;
    }//if !err
  }//if fd not zero and not EOF
  return retv;
}//close

//takes a long value and returns a long value
long lseek(int fd, long offset, int origin){
  return elfos_long_seek_file(fd, &offset, origin);
}//lseek

/* 
Buffered File I/O - internal functions

*/
int _fillbuf(FILE* fp){
  if ((fp->flag & (_READ | _EOF | _ERR )) != _READ) {
    //anything but read is an errorS
    return EOF;
  }//if _READ
  if (fp == stdin) {
    //for stdin call kernel function
    fp->ptr = fp->base;
    *fp->ptr = getc();
    fp->cnt = 0;    
  } else {
    fp->ptr = fp->base;
    fp->cnt = read(fp->fd, fp->ptr, BUFSIZE);
    if (--fp->cnt < 0){
      if(fp->cnt == -1) {
        fp->flag |= _EOF;
      } else {
        fp->flag |= _ERR;
      }//if-else cnt <0
      fp->cnt = 0;
      return EOF;
    } // if --cnt < 0
  }//if-else
  return (int) *fp->ptr++;
}//_fillbuf

int _flushbuf(int x, FILE* fp){
  int nc; //number of characters
  
  if (fp < _iob || fp >= _iob + OPEN_MAX) {
    return EOF; // error: invalid pointer
  } //if fp invalid
  if((fp->flag & (_WRITE | _ERR)) != _WRITE) {
    return EOF; //don't flush std io or invalid handles
  }//fp->flag
  
  if (fp == stdout || fp == stderr)  {
    //for stdout and stderr just write to elfos and fix cnt and ptr
    putc(x);
    fp->ptr = fp->base;
    *fp->ptr = (char) x;
    fp->cnt = 0;   
  } else {
    // printf("Buffered!\n");
    nc = fp->ptr - fp->base;
    if (write(fp->fd, fp->base, nc) != nc){    
       fp->flag |= _ERR;
       return EOF;
    }//if write
    //printFileInfo(fp);
     fp->ptr = fp->base;
     *fp->ptr++ = (char) x;
     fp->cnt = BUFSIZE-1;
  }//if-else
  return x;  
}//_flushbuf

/* 
Buffered I/O - external functions
*/
FILE *fopen(char* name, char* mode){
  int   fd;
  FILE* fp;
  
  if(*mode != 'r' && *mode != 'w' && *mode != 'a') {
    return NULL;
  }//if  
  for(fp = _iob; fp < _iob + OPEN_MAX; fp++) {
    if ((fp -> flag & (_READ | _WRITE)) == 0) {
      break; //found free spot
    }//if
  }//for
  
  if (fp >= _iob + OPEN_MAX) {
    //no free slots
    return NULL;
  }
  if(*mode == 'w') {
    fd = open(name, O_CREATE | O_TRUNCATE);
  } else if (*mode == 'a'){
    fd = open(name, O_CREATE | O_APPEND);
  } else {
    fd = open(name, O_OPEN);
  }//if-else if *mode
  if (fd == EOF) {
    return NULL;
  }//if fd

  //set up initial file ptr
  fp->fd = fd;
  fp->base = (char *) malloc(BUFSIZE); //buffer is 32 bytes
  //if malloc failed, close fd and return error
  if (fp->base == NULL) { 
    close(fd);
    return NULL;
  }//if fp-base
  //cnt is nothing read yet or entire buffer available for writing
  fp->cnt  = (*mode == 'r') ? 0 : BUFSIZE;  
  fp->ptr  = fp->base; //set ptr to base of buffer initially
  fp->flag = (*mode == 'r') ? _READ  : _WRITE;
  
  return fp;
}//fopen


int fflush(FILE* fp) {
  int rc;
  if (fp < _iob || fp >= _iob + OPEN_MAX) {
    return EOF; // error: invalid pointer
  } //if fp invalid

  //printFileInfo(fp);
  rc = _flushbuf(0, fp);
  fp->ptr = fp->base;
  if (fp == stdout || fp == stderr) {
    fp->cnt = 1;   
  } else {
    fp->cnt = BUFSIZE;  
  }//if stdout or stderr
  return rc;
}//fflush


int fclose(FILE* fp) {
  int rc;
  if (fp < _iob || fp >= _iob + OPEN_MAX) {
    return EOF; // error: invalid pointer
  }//if fp invalid
  
  if(fp == stdin || fp == stdout || fp == stderr) {
    //closing stdio handles does nothing,
    // but fix ptr and cnt just in case
    fp->ptr = fp->base;
    fp->cnt = 0;
    rc = 0; 
  } else if ((rc = fflush(fp)) != EOF) {
    free(fp->base); //free buffer
    close(fp->fd);  //free Filedes & DTA
    fp->fd = 0;
    fp->ptr = NULL;
    fp->base = NULL;
    fp->cnt = 0;
    fp->flag &= ~(_READ | _WRITE);
  } //if-else if flag
  return rc;
}//fclose

int ungetc(int ch, FILE* fp){
  if (fp < _iob || fp >= _iob + OPEN_MAX) {
    return EOF; // error: invalid pointer
  }//if fp invalid
  //don't push back EOF
  if (ch != EOF) {
    //back up ptr and set to value
    *(--fp->ptr) = (unsigned char) ch;
    fp->cnt++;
  }//if ch != EOF
  return ch;
}//ungetc

char *fgets(char *out, int size, FILE *in){
  int ch;
  char *buf = out;

  while (--size > 0 && (ch = fgetc(in)) != EOF) {
    *buf++ = ch;
    if (ch == '\n' || ch == '\r') {
      //EOL can be CR in Elf/OS
      break;
    }//if EOL
  }// while
  *buf = '\0';
  return (ch == EOF && buf == out) ? NULL : out;
}// fgets

int fputs(char* s, FILE* fp){
  int c;
  while(c = *s++) {
    fputc(c, fp);
  }//while 
  return ferror(fp) ? EOF : 0;
} //fputs 

int fseek(FILE* fp, long offset, int origin){
  unsigned int nc;  //number chars to flush
  long rc = 0;      //return code
  if (fp == stdin || fp == stdout || fp == stderr){
    //fseek does nothing on the stdio functions, but not an error
    return 0;
  }
  if (fp->flag & _READ){
    if (origin == SEEK_CUR){
      offset -= fp->cnt;   //remember chars in buffer   
    }//if SEEK_CUR
    rc = lseek(fp->fd, offset, origin);
    fp->cnt = 0;  //no chars in buffer after seek
  } else if (fp->flag & _WRITE){
    if((nc = fp->ptr - fp->base) > 0){
      //write out any chars in buffer
      if(write(fp->fd, fp->base, nc) != nc){
        rc = EOF;
      }//if write 
    }//if nc > 0
    if (rc != EOF){
      rc = lseek(fp->fd, offset, origin);
    }
  }//if-else fp->flag  
  return (rc == EOF) ? EOF : 0;  
}//fseek

long ftell(FILE* fp){
  long pos;
  if (fp == stdin || fp == stdout || fp == stderr){
    //ftell returns 0 for stdio functions
    return 0L;
  }
  pos = lseek(fp->fd, 0L, SEEK_CUR);
  //adjust the position for the buffer
  if (fp->flag & _READ) {
    /*
     * Reading.  Any unread characters (including
     * those from ungetc) cause the position to be
     * smaller than that in the underlying object.
     */
     pos -= fp->cnt;
  } else if (fp->flag & _WRITE) {
    /*
     * Writing.  Any buffered characters cause the
     * position to be greater than that in the
     * underlying object.
     */
     pos += (fp->ptr - fp->base);
  }
  return pos;
}//ftell

int fgetpos(FILE *fp, fpos_t *pos){
  //return 0 on success and 1 on failure
  return ((*pos = ftell(fp)) == (fpos_t) EOF);
}//fgetpos

int fsetpos(FILE *fp, fpos_t *pos) {
  return (fseek(fp, (long)*pos, SEEK_SET));
}//fsetpos

void rewind(FILE* fp){
  //set file to beginning and clear errors
  fseek(fp, 0L, SEEK_SET);
  clearerr(fp);
}//rewind

size_t fread(void *ptr,  size_t size, size_t nobj, FILE* fp){  
  size_t count = size * nobj;
  char *p = ptr;
  int nc = 0;
	int rc = 0;
  int cb = count;
  
  //Per ANSI spec
  if (cb == 0) {
    return 0;
  }//if cb == 0
  if (fp < _iob || fp >= _iob + OPEN_MAX) {
    return 0; // error: invalid pointer
  } //if fp invalid
  if((fp->flag & (_READ | _ERR)) != _READ) {
    return 0; //don't read invalid handles
  }//if fp->flag
  if (fp == stdin) {
      //ANSI says to call fgetc() repeatedly, nobj*size times
      //this blocks everything else, until all the bytes are read
      //Not necessarily a good thing, but not an error
      for(nc = 0; nc < count; nc++) {
        *p++ = fgetc(stdin);
      }//for
      rc = nobj;
  } else {
    //if characters in read buffer, use those first
    nc = (fp->cnt > cb) ? cb : fp->cnt;
    
    while(fp->cnt--) {
      *p++ = *fp->ptr++;
      cb--;
      if (cb == 0) {
        break;
      }//if cb == 0
    }//while

    //write out the reing bytes
    if (cb > 0) {
      nc += read(fp->fd, p, cb);
    } //if cb > 0

    if (nc == count){
      rc = nobj;
    } else {
      rc = nc / size;
    } //if nc == count
  }//if-else fp
  return rc;
}//fread

size_t fwrite(void *ptr,  size_t size, size_t nobj, FILE* fp){
  size_t count = size * nobj;
  int nc = 0; //byte counter
  int rc = 0; //return code
  if (fp < _iob || fp >= _iob + OPEN_MAX) {
    return 0; // error: invalid pointer
  } //if fp invalid
  if((fp->flag & (_WRITE | _ERR)) != _WRITE) {
    return 0; //don't write to invalid handles
  }//fp->flag
  if (fp == stdout || fp == stderr){
    char *p = ptr;
    //Per Ansi: fwrite calls fputc() nobj*size times, for stdio
    for(nc = 0; nc < count; nc++){
      fputc(*p++, fp);
    }//for 
    rc = nobj;
  } else {
    //flush buffer before writing
    if((fp->ptr - fp->base) > 0){
      //write out any chars in buffer
      rc = fflush(fp);
      }//if write               
    if (rc != EOF) {
        nc = write(fp->fd, ptr, count);
        if (nc != EOF) {
          //if write was okay don't do division
          if (nc == count) {
            rc = nobj;
          } else {
          rc = nc / size; //rc is number of size objects written
          }//if-else nc == count
        } //if nc != EOF
    } else {
			//return 0 on error
			rc = 0;
		}// if -else rc != EOF
  }//if-else fp
  return rc;
} //fwrite

#endif
