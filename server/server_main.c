#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define MAX_CONN_REQ_QUEUE 5
#define CONTROL_WORD_LENGTH 5
typedef enum ServerState {SERVER_IDLE = 0,
			  SERVER_SEND = 1,
			  SERVER_RECV = 2} ServerState;

void nonfatal_error(const char *msg)
{
  perror(msg);
}

void fatal_error(const char *msg)
{
  perror(msg);
  exit(-1);
}

void handle_connection(int socket)
{
  int n;
  char buffer[4096];
  bzero(buffer, 4096);
  ServerState state = SERVER_IDLE;
  while (1) {
    switch (state) {
    case SERVER_IDLE:
      printf("Reading socket\n");
      n = read(socket, buffer, CONTROL_WORD_LENGTH);
      printf("Read %d bytes on the socket\n",n);
      if (n < 0) {
	nonfatal_error("Socket read error");
	return;
      }
      else if (n == 0) {
	nonfatal_error("Remote socket closed before sending control word\n");
	return;
      }
      printf("Control Word received %s 0x%02x 0x%02x 0x%02x 0x%02x",
	     buffer[0]?"Server recv":"Server send",
	     buffer[1],
	     buffer[2],
	     buffer[3],
	     buffer[4]);
      break;
    case SERVER_SEND:
      break;
    case SERVER_RECV:
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  int socketfd;
  int newsocketfd;
  struct sockaddr_in server_addr;

  
  socketfd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketfd < 0) {
    fatal_error("ERROR opening socket");
  }

  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(55555);

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
