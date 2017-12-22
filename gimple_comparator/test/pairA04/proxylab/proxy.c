/*
 * A simple web proxy handling basic client-server connection,
 * concurrency, and data/object transmission. 
 *
 * Name: Yang Zhang
 * Andrew ID: yangz3
 * Partner: Liang He (lianghe)
 */
#include <stdio.h>
#include "csapp.h"

/* Default http request port & my listening port */
#define DEFAULT_HTTP_REQUEST_PORT 80
#define DEFAULT_LISTEN_PORT 49974

/* You won't lose style points for including these long lines in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *accept_hdr = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
static const char *accept_encoding_hdr = "Accept-Encoding: gzip, deflate\r\n";
static const char *connection_hdr = "Connection: close\r\n";
static const char *proxy_connection_hdr = "Proxy-Connection: close\r\n";

/* Helper functions */
void *thread(void *arg);
void process(int fd);
void prepare_request(char *new_request, char *host, char *cgiargs);
void parse_uri(char *uri, char *host, char *cgiargs, int *port);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);

int main(int argc, char *argv[])
{
    int port;
    socklen_t clientlen;
    int listenfd;
    int *connfdp;
    struct sockaddr_in clientaddr;
    pthread_t tid;

    /* Check the arguments*/
    if (argc != 2) {
        if (argc > 2) {
            fprintf(stderr, "More arguments than expectation\n");
            exit(1);
        }
        if (argc < 2) {
            // Use the default listen port
            port = DEFAULT_LISTEN_PORT;
        }
    }
    else {
        port = atoi(argv[1]);
    }

    /* Ignore SIGPIPE signal */
    Signal(SIGPIPE, SIG_IGN);

    /* Listen the port for connections */
    if ((listenfd = Open_listenfd(port)) < 0) {
        fprintf(stderr, "Errors in Open_listenfd function\n");
        exit(1);
    }

    /* Connections */
    while (1) {
        connfdp = (int *)Malloc(sizeof(int));
        clientlen = sizeof(clientaddr);
        *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Pthread_create(&tid, NULL, (void *)thread, (void *)connfdp);
    }

    return 0;
}

/* t_func - the calling funciton for each thread
 * Free the pointer to the connection file discriptor, deal with HTTP
 * requests and responses, finally close the connection
 */
void *thread(void *arg) 
{
    int clientfd = (int)*((int *)arg);
    Pthread_detach(Pthread_self());
    Free(arg);
    process(clientfd);
    Close(clientfd);
    return NULL;
}

/* process - handle one HTTP request/response transaction
 * 1. Get HTTP request including header information from the client
 * 2. Get host, cgi and hostport by parsing URI
 * 3. Set new HTTP request for the server
 * 4. Forward new request to the server
 * 5. Get response from the server and forward it to the client
 */  
void process(int clientfd)
{
    rio_t client_rio, server_rio;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char host[MAXLINE], cgiargs[MAXLINE];
    char new_request[MAXLINE], response_buf[MAXLINE];
    int len;
    int hostport, serverfd;

    /* Get HTTP request including header information from the client */
    Rio_readinitb(&client_rio, clientfd); 
    Rio_readlineb(&client_rio, buf, MAXLINE); // read a text line from client_rio
    sscanf(buf, "%s %s %s", method, uri, version); // set method, uri and version 

    // check if the method is GET
    if (strcasecmp(method, "GET") != 0) {
        clienterror(clientfd, method, "501", "Not Implemented",
                    "Proxy only supports GET method");
        return;
    }

    /* Parse URI */
    parse_uri(uri, host, cgiargs, &hostport);

    /* Set new HTTP request for the server */
    prepare_request(new_request, host, cgiargs);

    /* Forward the new HTTP request to the server */
    if ((serverfd = open_clientfd_r(host, hostport)) < 0) {
        printf("Proxy cannot connect to the server\n");
        clienterror(clientfd, host, "404", "Not Found", 
                    "The requested resource could not be found");
        return;
    }

    if (rio_writen(serverfd, new_request, strlen(new_request)) < 0) {
        printf("Errors in rio_writen\n");
        return;
    }

    /* Get and forward response from the server to the client */
    Rio_readinitb(&server_rio, serverfd);
    while ((len = Rio_readlineb(&server_rio, response_buf, MAXLINE)) > 0) {
        Rio_writen(clientfd, response_buf, len);
    }

    Close(serverfd);
}
/*
 * parse_uri - parse URI 
 *             default port 80.
 * uri: the URI of the request
 * host: the name of the host
 * file: the rest of CGI besides the host and the port
 */
void parse_uri(char *uri, char *host, char *cgiargs, int *port) 
{
    char *uri_p;
    char port_t[MAXLINE];

    if((uri_p = strstr(uri, "http://")) == NULL){
        fprintf(stderr, "Invalid URI");
        exit(1);
    }

    if (strstr((uri_p + strlen("http://")), ":") != NULL){ 
        // there is port number in the URI
        sscanf(uri, "http://%[^:]:%[^/]%s", host, port_t, cgiargs);
        *port = atoi(port_t);
    } 
    else{
        // use the default port number
        sscanf(uri, "http://%[^/]%s", host, cgiargs);
        *port = DEFAULT_HTTP_REQUEST_PORT;
    }
}
/*
 * prepare_request - set new HTTP request for the server from the proxy
 * new_request: new request content
 * host: the name of the host
 * cgiargs: the arguments for CGI
 */
void prepare_request(char *new_request, char *host, char *cgiargs) 
{
    sprintf(new_request, "GET %s HTTP/1.0\r\n", cgiargs);
    strcat(new_request, "Host: ");
    strcat(new_request, host);
    strcat(new_request, "\r\n");
    strcat(new_request, user_agent_hdr);
    strcat(new_request, accept_hdr);
    strcat(new_request, accept_encoding_hdr);
    strcat(new_request, connection_hdr);
    strcat(new_request, proxy_connection_hdr);
    strcat(new_request, "\r\n");
}

/*
 * clienterror - returns an error message to the client
 * fd: the connection file discriptor
 * cause: the factor with errors
 * errnum: the number of the error
 * shortmsg: short statement of the error
 * longmsg: long statement of the error
 */
void clienterror(int fd, char *cause, char *errnum, 
         char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE], body[MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Proxy Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Proxy</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    Rio_writen(fd, buf, strlen(buf));
    Rio_writen(fd, body, strlen(body));
}
