// kkill, kill app by name
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h> 

// constants
#define MAXNAME 80
#define PROCESSPOSITION 1
#define PROGRAMPOSITION 7
const char *psflags="ef";
const char *signal="9";
enum { NONE=0, WORD, LINE };

// functions
int readfileentry(int fd, char *line);
unsigned int isseparationchar(char t);
int findsimple(char text[], char token[]);
void showusage();

int exactsearch=0;

int main(int argc, char *argv[])
{
  int i, i1, fd, process, nread, count=0;
  char tmpfile[L_tmpnam + 1];
  char program[MAXNAME], line[MAXNAME*5], line2[MAXNAME*5];
  tmpnam(tmpfile);
  
  // parse command line options
   while ((i = getopt(argc, argv, ":e")) != -1)
    switch (i) {
     case 'e':
      exactsearch=1;
     break;
     case '?':
      showusage();
     break;
     default:
      abort();
   break; }
  
  // file to keep track for size
  if (optind == argc)
   showusage();
  
  i=fork();
  if (i>0) {
   printf("kkiller active with process %d\n", i);
   return 0;
  }
  
  while (1) {
   for (i=optind, i1=0;i<argc;i++) {
    strcpy(program, argv[i]);
    sprintf(line, "ps -%s|grep %s >%s\n", psflags, program, tmpfile);
    system(line);
    fd=open(tmpfile, O_RDONLY);
    while ((nread=readfileentry(fd, line))>-1) {
     if (i1==PROCESSPOSITION)
      process=atoi(line);
     if (i1==PROGRAMPOSITION && (findsimple(line, program))) {
      sprintf(line2, "kill -%s %d >/dev/null 2>&1\n", signal, process);
      if (WEXITSTATUS(system(line2))==0)
       ++count;
     }
    if (nread==LINE)
     i1=0;
    if (nread==WORD)
     ++i1;
   }
   close(fd);
   }
   sleep(1);
  } 
  
  
  unlink(tmpfile);
    
 return count;
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
    if (nread==0) 
     return -1;
    
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

// find command in text
int findsimple(char text[], char token[])
{ 
  int i, n, hit=0;
  char ttoken[MAXNAME];
  
  if (exactsearch) {
   if ((i=strcmp(text, token))==0)
    return 1;
   return 0;
  }
  
   for (i=hit=0;i<strlen(text);i++) {
    if (text[i]==token[0]) {
     hit=i+1;
     for (n=0;n<strlen(token);n++)
      ttoken[n]=text[i+n];
     ttoken[n]='\0';
     if (!strcmp(ttoken, token))
      break;
    }
   }
   
 return hit;
}

// show kkill usage
void showusage()
{
  printf("kkill -e (exact name search) <program 1> <program 2> <program 3> ..\n");
 
 exit(-1);
}
