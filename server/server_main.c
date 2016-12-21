#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <strings.h>

#include "server_common.h"

#define MAX_CONN_REQ_QUEUE 5
#define DEFAULT_PORT 55555


void display_usage()
{
  fprintf(stderr, "Usage: ThroughputServer -p [port number]\n");
  exit(1);
}

int main(int argc, char *argv[])
{
  int socketfd;
  int newsocketfd;
  int port = DEFAULT_PORT;
  int c;
  struct sockaddr_in server_addr;

  opterr = 0;
  while ((c = getopt (argc, argv, "hp:")) != -1)
    switch (c)
      {
      case 'p':
        port = atoi(optarg);
        break;
      case 'h':
      case '?':
        display_usage();
      default:
        abort ();
      }

  
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    fatal_error("ERROR opening socket");
  }

  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  if (bind(socketfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
    fatal_error("ERROR on binding");
  }

  if (listen(socketfd, MAX_CONN_REQ_QUEUE) < 0) {
    fatal_error("ERROR listnening on socket");
  }

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  
  while (1) {
    newsocketfd = accept(socketfd, (struct sockaddr *) &client_addr, &client_addr_len);
    printf("New connection accepted\n");
    if (newsocketfd <0) {
      fatal_error("ERROR on accepting connection");
    }

    int pid = fork();

    if (pid < 0) {
      nonfatal_error("NONFATAL unable to create new thread");
      close(newsocketfd);
    }
    
    if (pid == 0) {
      close(socketfd);
      handle_connection(newsocketfd);
      printf("Nothing more to read closing socket\n");
      exit(0);
    }
    else {
      close(newsocketfd);
    }
  }
}
