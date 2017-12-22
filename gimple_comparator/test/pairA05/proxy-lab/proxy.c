/*
 * proxy.c - CS:APP Web proxy
 *
 * TEAM MEMBERS:
 *     Einar SigurÃ°ur SigurÃ°sson - einarss11@ru.is
 *     GuÃ°ni MatthÃ­asson         - gudni11@ru.is
 *     JÃ³n Atli Ã“lafsson         - jonao11@ru.is
 *
 * Proxy.c: This is a simple proxy server application
 * that uses POSIX threads for concurrent server funct-
 * ionality.
 */

#include "csapp.h"
#include <pthread.h>
#include <semaphore.h>

/* Utility variables */
static sem_t mutex;
pthread_rwlock_t rwlock;
int logFd;

/* Function declarations */
int parse_uri(char *uri, char *target_addr, char *path, int  *port);
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr, char *uri, int size);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);
void doitz(int fd);
void Parse_uri(char *uri, char *pathname, char *hostname, int *port);
void *thread(void *vargp);

/*
 * main - Main routine for the web proxy. It sets the gears
 * of extreme awesomeness in motion.
 */
int main(int argc, char **argv)
{
    //Initialize file descriptors, listening port, client socket and pthread object.
    int listenfd, *connfd, port;
    socklen_t socketlen;
    struct sockaddr_in socket;
    pthread_t tid;

    // Invalid number of arguments.
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }
    port = atoi(argv[1]);
    listenfd = Open_listenfd(port);

    /*
     * The server is now listening on the specified port.
     * Next, initialize the read-write lock and semaphore
     * mutex to be used by each pthread.
     */
    pthread_rwlock_init(&rwlock, NULL);
    Sem_init(&mutex, 0, 1);

    if((logFd = open("proxy.log", O_CREAT|O_WRONLY, 0777)) < 0)
    {
        unix_error("unable to open proxy.log");
    }
    // Begin listening for connections.
    while (1)
    {
        socketlen = sizeof(socket);

        connfd = malloc(sizeof(int));
        *connfd = Accept(listenfd, (SA *)&socket, &socketlen);
        // Once a socket connection has been established, create a designated thread.
        if (pthread_create(&tid, NULL, thread, connfd) < 0)
        {
            fprintf(stderr, "Thread_create error: Could not create thread.\n");
        }
    }
    exit(0);
}

/*
 * thread - This function runs separately on different threads for each client.
 * It inithalizes the client socket file descriptor, detaches the current thread,
 * frees up space on the stack and calls doitz.
 */
void *thread(void *vargp)
{
    int clientfd = *((int *) vargp);
    if (pthread_detach(pthread_self()) < 0)
    {
        fprintf(stderr, "thread_detach: Error detaching thread.\n");
        pthread_exit("Error detaching thread.");
    }
    free(vargp);
    doitz(clientfd);
    // Close the socket connection when finished.
    Close(clientfd);
    return NULL;
}

/*
 * parse_hdr - Returns the parameter header line if it is not one of the hardcoded
 * properties.
 */
char *parse_hdr(char *hdr)
{
    char tmp;
    char tmp_hdr[MAXLINE] = {0};
    int i = 0, j = 0;
    /* Get the header type*/
    while ((tmp = *(hdr + i)) != ':' && tmp != '\0')
    {
        *(tmp_hdr + j) = tmp;
        j++;
        i++;
    }
    /*If this does not belong to any of this five kinds */
    if (strcasecmp(tmp_hdr, "Host") && strcasecmp(tmp_hdr, "User-Agent") \
            && strcasecmp(tmp_hdr, "Accept") \
            && strcasecmp(tmp_hdr, "Accept-Encoding") \
            && strcasecmp(tmp_hdr, "Connection") \
            && strcasecmp(tmp_hdr, "Proxy-Connection"))
        return hdr;
    memset(hdr, 0, sizeof(hdr));
    return hdr;
}

/*
 * open_clientfd_threadsafe - This uses open_clientfd with regard to the
 * semaphore mutex.
 */
int open_clientfd_threadsafe(char *hostname, int port)
{
    // Assign the client socket file descriptor to a socket
    int clientfd;
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;
    // Wait for the mutex and lock it before fetching the host
    P(&mutex);
    struct hostent *hostptr;
    if ((hostptr = gethostbyname(hostname)) == NULL)
    {
        // Unlock the mutex and return if host is not found
        V(&mutex);
        return -2;
    }
    struct sockaddr_in srvaddr;
    // Set the value of the server address
    memset((void *)&srvaddr, 0, sizeof(srvaddr));
    srvaddr.sin_family = AF_INET;
    bcopy((void *)hostptr->h_addr_list[0],
          (void *)&srvaddr.sin_addr.s_addr,
          hostptr->h_length);
    srvaddr.sin_port = htons(port);
    // Unlock the mutex
    V(&mutex);
    // Connect the client socket
    if (connect(clientfd, (SA *)&srvaddr, sizeof(srvaddr)) < 0)
        return -1;
    return clientfd;
}

/*
 * doitz - Handles a HTTP request from the client
 */
void doitz(int fd)
{
    int n = 0;
    int clientfd;
    int port = 80;
    char host[MAXLINE]    = {0},
         path[MAXLINE]    = {0},
         method[MAXLINE]  = {0},
         uri[MAXLINE]     = {0},
         version[MAXLINE] = {0},
         header[MAXBUF]   = {0},
         buf1[MAXBUF]     = {0},
         buf2[MAXBUF]     = {0};
    rio_t rio1, rio2;
    unsigned int datasize = 0;
    // Set all the buffers to 0
    bzero(header, sizeof(header));
    bzero(buf1, sizeof(buf1));
    bzero(buf2, sizeof(buf2));
    bzero(host, sizeof(host));
    bzero(path, sizeof(path));
    bzero(method, sizeof(method));
    bzero(uri, sizeof(uri));
    bzero(version, sizeof(version));
    // Read request
    rio_readinitb(&rio1, fd);
    if ((n = rio_readlineb(&rio1, buf1, MAXLINE)) <= 0)
    {
        if (n < 0)
            fprintf(stderr, "rio_readlieb error: read request line failed!\n");
        return;
    }
    sscanf(buf1, "%s %s %s", method, uri, version);
    // Parse URI
    if (parse_uri(uri, host, path, &port) < 0)
    {
        char *errmsg = "Invalid port number!\n";
        rio_writen(fd, errmsg, strlen(errmsg));
        return;
    }
    // Read header
    while ((n = rio_readlineb(&rio1, buf1, MAXLINE)) > 0)
    {
        if (!strcmp(buf1, "\r\n")) break;
        sprintf(header, "%s%s", header, parse_hdr(buf1));
        bzero(buf1, sizeof(buf1));
    }
    // Connect to server
    if ((clientfd = open_clientfd_threadsafe(host, port)) < 0)
    {
        fprintf(stderr, "Error connecting to server\n");
        if (clientfd == -1)
        {
            char *errmsg = "Connection error!\n";
            rio_writen(fd, errmsg, strlen(errmsg));
        }
        else if (clientfd == -2)
        {
            char *errmsg = "Invalid uri!\n";
            rio_writen(fd, errmsg, strlen(errmsg));
        }
        close(clientfd);
        return;
    }
    rio_readinitb(&rio2, clientfd);
    // Forward HTTP request to the server
    sprintf(buf1, "GET %s HTTP/1.0\r\nHost: %s\r\n%s%s%s", \
            path, host,
            "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n",
            "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n",
            "Accept-Encoding: gzip, deflate\r\n");
    sprintf(buf1, "%s%s%s%s\r\n", \
            buf1,
            "Connection: close\r\n",
            "Proxy-Connection: close\r\n",
            header);
    if (rio_writen(clientfd, buf1, strlen(buf1)) < 0)
    {
        fprintf(stderr, "rio_writen error: fail to send request!\n");
        close(clientfd);
        return;
    }
    memset(buf1, 0, sizeof(buf1));
    // Read response
    while ((n = rio_readnb(&rio2, buf2, MAXBUF)) > 0)
    {
        datasize += n;
        if (rio_writen(fd, buf2, n) < 0)
        {
            fprintf(stderr, \
                    "rio_writen error: fail to send response!\n");
            close(clientfd);
            return;
        }
        bzero(buf2, sizeof(buf2));
    }
    // TODO: Log request
    char logstring[MAXLINE];
    struct sockaddr_in clientsock;
    socklen_t cs_len = sizeof(clientsock);
    getsockname(fd, &clientsock, &cs_len);
	format_log_entry(logstring, &clientsock, uri, datasize);
	pthread_rwlock_wrlock(&rwlock);
	Rio_writen(logFd,logstring, strlen(logstring));
	pthread_rwlock_unlock(&rwlock);  
    datasize = 0;
    close(clientfd);
}


/*
 * clienterror - returns an error message to the client
 */
/* $begin clienterror */
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Proxy Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
/* $end clienterror */

/*
 * parse_uri - URI parser
 *
 * Given a URI from an HTTP proxy GET request (i.e., a URL), extract
 * the host name, path name, and port.  The memory for hostname and
 * pathname must already be allocated and should be at least MAXLINE
 * bytes. Return -1 if there are any problems.
 */
int parse_uri(char *uri, char *host, char *path,  int *port)
{
    int i = 0, p = 0, h = 0, p_hd = 0;
    char tmp;
    char newport[MAXLINE];
    for (i = 0, h = 0, p = 0, p_hd = 0; (tmp = *(uri + i)) != '\0'; i++)
    {
        if (tmp == ':' && i > 6 && !p_hd)
        {
            p_hd = i;
        }
        else if (i > 6 && tmp == '/')
        {
            p = i;
            break;
        }
        else if (i > 6 && !p_hd)
        {
            *(host + h) = *(uri + i);
            h++;
        }
    }
    /* Get the path*/
    for (i = 0; (tmp = *(uri + p)) != '\0'; p++, i++)
    {
        *(path + i) = tmp;
    }
    /* Decide the port number*/
    if (p_hd == 0)
        *port = 80;
    else
    {
        for (i = 0; (tmp = *(uri + p_hd)) != '/' && tmp != '\0'; i++, p_hd++)
        {
            if (i > 4) return -1;
            if (tmp < 48 || tmp > 57) return -1;
            *(newport + i) = tmp;
        }
        if ((*port = atoi(newport)) > 65535 || port < 0) return -1;
    }
    return 0;
}

/*
 * format_log_entry - Create a formatted log entry in logstring.
 *
 * The inputs are the socket address of the requesting client
 * (sockaddr), the URI from the request (uri), and the size in bytes
 * of the response from the server (size).
 */
void format_log_entry(char *logstring, struct sockaddr_in *sockaddr,
                      char *uri, int size)
{
    time_t now;
    char time_str[MAXLINE];
    unsigned long host;
    unsigned char a, b, c, d;

    /* Get a formatted time string */
    now = time(NULL);
    strftime(time_str, MAXLINE, "%a %d %b %Y %H:%M:%S %Z", localtime(&now));

    /*
     * Convert the IP address in network byte order to dotted decimal
     * form. Note that we could have used inet_ntoa, but chose not to
     * because inet_ntoa is a Class 3 thread unsafe function that
     * returns a pointer to a static variable (Ch 13, CS:APP).
     */
    host = ntohl(sockaddr->sin_addr.s_addr);
    a = host >> 24;
    b = (host >> 16) & 0xff;
    c = (host >> 8) & 0xff;
    d = host & 0xff;


    /* Return the formatted log entry string */
    sprintf(logstring, "%s: %d.%d.%d.%d %s\n", time_str, a, b, c, d, uri);
}

