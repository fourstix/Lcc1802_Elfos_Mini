#ifndef ELFOSLIB_H
#define ELFOSLIB_H

/* nstdlib.h definitions */
//#include <nstdlib.h>

#ifndef putchar
#define putchar putc
#endif
unsigned int strlen(char *str);

//char * strcat(char* d, char* s);

char * itoa(int, char *);
char * ltoa(long, char *);
#ifndef nofloats
char * ftoa(float, char *,unsigned int);
#endif


int printf(char *fmt, ...);
int sprintf(char *p, char* fmt, ...);
int scanf(char* fmt, ...);
int sscanf(char* s, char* fmt, ...);

//void exit(int); //halt with a numeric error message
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
int memcmp(const void *Ptr1, const void *Ptr2, unsigned int Count);
void *memset(void *s, int c, unsigned int n); //sets n bytes of memory at s to c
void* memcpy(void* dest, const void* src, unsigned int count);

//implmented in nstdlib.inc as assembler code 
char * dubdabx(long, char *, int);
char * strcpy(char *, const char*);
int strcmp(char* s1, char* s2);

/* 
 * ctype definitions 
 */
#ifndef _C_TYPE
#define _C_TYPE
#define isspace(c) \
  (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ')
#define isdigit(c) (c >= '0' && c <= '9')
#define isalpha(c)  (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')))
#define isupper(c)  ((c >= 'A') && (c <= 'Z'))
#define islower(c)  ((c >= 'a') && (c <= 'z'))
#define isxdigit(c) (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || \
  (c >= 'a' && c <= 'f')
  
//maximum size of ascii numeric field  
#define MAX_NUM_SIZE  11  
//define base unction for string numeric routines
long atol(char *p, int base);

#ifndef nofloats
float atof(char *s);
#endif

#define atoi(p) ((int)atol((p), 10))
#define atox(p) ((unsigned int)atol((p), 16))
#define atou(p) ((unsigned int)atol((p), 10))
#define atolx(p) ((unsigned long)atol((p), 16))
#define atold(p) (atol((p), 10))
#endif

/* FILE IO */
#ifndef OPEN_MAX
#define OPEN_MAX 7

typedef struct _iobuf {
    int   cnt;
    char* ptr; //next character position
    char* base; //location of buffer
    int  flag; //mode of file access
    int  fd; //file descriptor
  } FILE;
  
  /* Buffered IO Flags */
#define  _READ  0x01
#define  _WRITE 0x02
#define  _UNBUF 0x04
#define  _EOF   0x08
#define  _ERR   0x10  

//One char buffers for pushback iob
char _c_iob[OPEN_MAX];

/* stdin, stdout, stderr are unbuffered and call elfos kernel directly
 * The other file handles are buffered by the Elf/OS kernel, 
 * so no buffering needed.
 */    
FILE _iob[OPEN_MAX] = { // stdin, stdout, stder + 4 buffered file handles
  {0, _c_iob,  _c_iob,  _READ  | _UNBUF, 0},  //stdin
  {0, _c_iob+1, _c_iob+1, _WRITE | _UNBUF, 1}, //stdout
  {0, _c_iob+2, _c_iob+2, _WRITE | _UNBUF, 2}, //stderr
  {0, _c_iob+3, _c_iob+3, 0, 0},
  {0, _c_iob+4, _c_iob+4, 0, 0},
  {0, _c_iob+5, _c_iob+5, 0, 0},
  {0, _c_iob+6, _c_iob+6, 0, 0}
};

/* first 3 file handles are predefined */
#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2]) 

/* ElfOS definitions */
#define putc elfosputc
#define getc elfosgetc
#define gets elfosgets
#define puts elfosputs

//#define puts elfosputs
#define read elfos_read_file
#define write elfos_write_file
//#define puts elfosputs

/* ElfOS file IO constants */
#define FD_SIZE 531
//#define BUFSIZE 32
#define MAXLINE 255
#define EOF (-1)
#define NULL 0
#define O_OPEN     0
#define O_CREATE   1
#define O_TRUNCATE 2
#define O_APPEND   4
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2

//type used in File I/O fsetpos and fgetpos
#define fpos_t long
//type used for File I/O fread and fwrite
#define size_t unsigned int

//Macros used for buffered File I/O
#define feof(p)    (((p)->flag & _EOF) != 0)
#define ferror(p)  (((p)->flag & _ERR) != 0)
#define fileno(p)  ((p))->fd)
#define clearerr(p)   ((p)->flag &= ~(_ERR | _EOF))
#endif

/* ElfOS kernel functions */
void elfosputc(char c);
int elfosgetc(void);
char* elfosgets(char* s);
int elfosputs(char* s);
unsigned int elfos_sp(void);
unsigned int elfos_lomem(void);

/* ElfOS Memory functions */
void free(void* p);
void* malloc(int size);
void* calloc(int n, int size);

/* ElfOS Unbuffered IO functions */
int open(char* fname, int flags);
int close(int fd);
int read(int fd, char* buf, int count);
int write(int fd, char* buf, int count);
long lseek(int fd, long offset, int origin);

/* ElfOS Buffered IO functions */
FILE *fopen(char* name, char* mode);
int fclose(FILE* fp);
int fflush(FILE* fp);
char *fgets(char* out, int size, FILE *in);
int fputs(char* s, FILE* fp);
int ungetc(int ch, FILE* fp);
int fseek(FILE* fp, long offset, int origin);
long ftell(FILE* fp);
int fgetpos(FILE* fp, fpos_t *pos);
int fsetpos(FILE* fp, fpos_t *pos);
void rewind(FILE* fp);
size_t fread(void *ptr, size_t size, size_t nobj, FILE* fp);
size_t fwrite(void *ptr, size_t size, size_t nobj, FILE* fp);
int fprintf (FILE *fp, char *fmt, ...);
int fscanf(FILE *fp, char* fmt, ...);
//#include <nstdlib.c>
#include "elfoslib.c"
#endif
