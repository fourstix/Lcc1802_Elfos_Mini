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
  {0, &_c_stdin,  &_c_stdin,   _READ | _UNBUF, 0},  //stdin
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

/* ElfOS file IO constants */
#define FD_SIZE 531
#define BUFSIZE 512
#define EOF (-1)
#define NULL 0
#define O_OPEN     0
#define O_CREATE   1
#define O_TRUNCATE 2
#define O_APPEND   4
#define SEEK_SET  0
#define SEEK_CUR  1
#define SEEK_END  2

#define feof(p)    (((p)->flag & _EOF) != 0)
#define ferror(p)  (((p)->flag & _ERR) != 0)
#define fileno(p)  ((p))->fd)
#define fgetc(p)   (--(p)->cnt >= 0 ? (unsigned char) *(p)->ptr++ : _fillbuf(p))
#define fputc(x,p) (--(p)->cnt >= 0 ? *(p)->ptr++ = (x) : _flushbuf((x),p))

/* ElfOS kernel functions */
void elfosputc(char c);
char elfosgetc(void);
int elfosgets(char* s);
//int elfosputs(char* s);
int elfos_sp(void);
int elfos_lomem(void);
//void* elfos_alloc(int size);
//void elfos_free(void* p);
//int elfos_valid_block(void* p);
//void elfos_init_fd(int fd);
//int elfos_open_file(int fd, char* name, int flags);
//int elfos_close_file(int fd);
//int elfos_read_file(int fd, char* buf, int count);
//int elfos_write_file(int fd, char* buf, int count);

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

#include <nstdlib.c>
#include "elfoslib.c"
#endif
