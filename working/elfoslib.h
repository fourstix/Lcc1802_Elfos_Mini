#ifndef ELFOSLIB_H
#define ELFOSLIB_H

/* nstdlib.h definitions */
#include <nstdlib.h>
/* FILE IO */
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

//One char for unbuffered stdio
char _c_stdin;
char _c_stdout;
char _c_stderr;
/* stdin, stdout, stderr are unbuffered and call elfos kernel directly
 * The other file handles are buffered by the Elf/OS kernel, and the
 * buffered file io functiosn read and write to the Elf/OS dta as a buffer.
 */    
FILE _iob[OPEN_MAX] = { // stdin, stdout, stder + 4 buffered file handles
  {0, &_c_stdin,  &_c_stdin,  _READ  | _UNBUF, 0},  //stdin
  {0, &_c_stdout, &_c_stdout, _WRITE | _UNBUF, 1}, //stdout
  {0, &_c_stderr, &_c_stderr, _WRITE | _UNBUF, 2}, //stderr
  {0, (char *) 0, (char *) 0, 0, 0},
  {0, (char *) 0, (char *) 0, 0, 0},
  {0, (char *) 0, (char *) 0, 0, 0},
  {0, (char *) 0, (char *) 0, 0, 0}
};

/* first 3 file handles are predefined */
#define stdin  (&_iob[0])
#define stdout (&_iob[1])
#define stderr (&_iob[2]) 

/* ElfOS definitions */
#define putc elfosputc
#define getc elfosgetc
#define gets elfosgets
//#define puts elfosputs
#define read elfos_read_file
#define write elfos_write_file
//#define puts elfosputs

/* ElfOS file IO constants */
#define FD_SIZE 531
#define BUFSIZE 32
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
#define fgetc(p)   (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define fputc(x,p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x),p))
#define clearerr(p)   ((p)->flag &= ~(_ERR | _EOF))

/* ElfOS kernel functions */
void elfosputc(char c);
int elfosgetc(void);
char* elfosgets(char* s);
//int elfosputs(char* s);
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
int _fillbuf(FILE* fp);
int _flushbuf(int x, FILE* fp);
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

#include <nstdlib.c>
#include "elfoslib.c"
#endif
