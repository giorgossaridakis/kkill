// kkill, kill app by name
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h> 

#define MAXNAME 80
#define PROCESSPOSITION 1
#define PROGRAMPOSITION 7
enum { NONE=0, WORD, LINE };
const char *psflags="ef";

int readfileentry(int fd, char *line);
unsigned int isseparationchar(char t);

int main(int argc, char *argv[])
{
  int i, i1, fd, process, nread;
  char tmpfile[L_tmpnam + 1];
  char program[MAXNAME], line[MAXNAME*2];
  tmpnam(tmpfile);
  
  if (argc<2)
   return -1;
  
  for (i=1, i1=0;i<argc;i++) {
   strcpy(program, argv[i]);
   sprintf(line, "ps -%s|grep %s >%s\n", psflags, program, tmpfile);
   system(line);
   fd=open(tmpfile, O_RDONLY);
   while ((nread=readfileentry(fd, line))>-1) {
    if (i1==PROCESSPOSITION)
     process=atoi(line);
    if (i1==PROGRAMPOSITION && !strcmp(program, line)) {
//      printf("process: %d\n", process);
     sprintf(line, "kill -9 %d\n", process);
     system(line);
    }
    if (nread==LINE)
     i1=0;
    if (nread==WORD)
     ++i1;
   }
   close(fd);
  }
  
  unlink(tmpfile);
    
 return 0;
}

// read entry from file
int readfileentry(int fd, char *line)
{
  char buf[1];
  int i=0, nread;

    while ((nread=read(fd, buf, sizeof(buf)))) {
     if (isseparationchar(buf[0])) {
      if (i==0) // no characters in line, separation character skipped
       continue;
      break; // break read, separation character not recorded
     }
     line[i++]=buf[0];
    }
    line[i]='\0';

    // file ended, close file descriptor
    if (nread==0) {
     close(fd);
     return -1;
    }
    
 return isseparationchar(buf[0]);
}

// word separation characters
unsigned int isseparationchar(char t) 
{
  if (t==' ' || t=='\r')
    return WORD;
  if (t=='\n')
   return LINE;

 return NONE;
}

