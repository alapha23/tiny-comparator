#ifndef __UTILSH
#define __UTILSH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void error(char *msg)
{
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}

pid_t Fork(void)
{
  pid_t pid;
  if((pid = fork()) < 0)
  {
    error("fork error");
  }
}

char *Fgets(char *ptr, int n, FILE *stream)
{
  char *rptr;
  if(((rptr =(char *)fgets(ptr, n, stream)) == NULL) && ferror(stream))
    error("Fgets error");
  return rptr;
}

#endif
