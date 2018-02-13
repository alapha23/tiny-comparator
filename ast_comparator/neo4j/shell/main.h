#ifndef __MAINH
#define __MAINH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include "utils.h"

#define MAXARGS 128
#define MAXLINE 1024

extern char **environ;

void eval(char *cmdline);
int parseline(const char *cmdline, char **argv);
int builtin_command(char **argv);
#endif

