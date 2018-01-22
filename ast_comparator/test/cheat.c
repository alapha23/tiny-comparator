#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <setjmp.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


void	usage(char*a){}
void	foo(void){}
void	goo(void){}
int	roo(void){return 1;}
int	too(int a){return 1;}
void 	too_arg(char*a, int b){}
int	Accept(int a, void * b, void *c){return 1;}
void    Pthread_create(int a, char* b, void *c, void *d){}

int main(int argc, char *argv [])
{
        int cache_on = 1;
        int port;
        struct sockaddr_in clientaddr;
        int clientlen = sizeof(clientaddr);
        int listenfd;

        if (argc < 2)
                usage(argv[0]);

        if (argc >= 3)
                cache_on = (strcmp(argv[2],"off"));

//        port = atoi(argv[1]);
//        if (port <= 0 || port >= 12600){
//		too_arg("dsasda", 2);
//               exit(1);
//        }

       char * cache_list;
	cache_list = NULL;
//       if (cache_on){
//                printf("cache is on.\n");
//                cache_list = roo();
//        }
//        else
                printf("cache is off.\n");

        // ignore SIGPIPE
	too_arg("tooo", 21);

        listenfd = too(21);
        while (1){
                pthread_t tid;
                int *connfdp = too(sizeof(int));
                *connfdp = -1;
		
//                *connfdp = Accept(listenfd, (void *) &clientaddr, (void *)&clientlen);
//                Pthread_create(&tid, NULL, (void *)NULL, (void *)connfdp);
        }

	too(2);
        return 0;
}

