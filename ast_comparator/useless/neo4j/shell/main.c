
#include "main.h"

int main(int args, char **argv, char **environ)
{
  char cmdline[MAXLINE];

  emit_header();
  while(1)
  {
    printf("neo4j> ");
    Fgets(cmdline, MAXLINE, stdin);
    if(feof(stdin))
    {
      exit(0);
    }
    eval(cmdline);
  }
}

void emit_header(void)
{

	// Specify a scope

}

void eval(char *cmdline)
{
	char *argv[MAXARGS];
	/* bg = 1 when the job is running in bg */
	int bg;
	pid_t pid;

	bg = parseline(cmdline, argv);
	if(argv[0] == NULL)
	 return ;

	if(!builtin_command(argv))
	{
	  if((pid = Fork()) == 0)
	  {
	    if(execve(argv[0], argv, environ)<0)	    
	      printf("%s: Command not found.\n", argv[0]);	  
	  }
	}
	
	if(!bg)
	{
	  int status;
	  if(waitpid(pid, &status, 0) < 0)
	  {
	    error("waitfg: waitpid error");
	  }	 
	}else printf("%d %s", pid, cmdline);	
}

int builtin_command(char **argv)
{
  if(!strcmp(argv[0], "quit"))
    exit(0);
  if(!strcmp(argv[0], "exit"))
    exit(0);
  if(!strcmp(argv[0], "&"))
    return 1;
  /* returns 0 if it is not a builtin command */
  return 0;
}

int parseline(const char *cmdline, char **argv)
{
  char array[MAXLINE];
  char *buf = array;
  char *delim;
  int argc;		/* number of arguments */
  int bg;

  strcpy(buf, cmdline);
  buf[strlen(buf)-1] = ' ';
  while(*buf && (*buf == ' '))
    buf++;
  argc = 0;
  while((delim = strchr(buf, ' ')))
  {
    argv[argc++] = buf;
    *delim = '\0';
    buf = delim + 1;
    while(*buf && (*buf == ' '))
      buf++;
  }
  argv[argc] = NULL;

  if(argc == 0)
    return 1;
  if((bg = (*argv[argc-1] == '&') != 0))
    argv[--argc] = NULL;
  return bg;
}

