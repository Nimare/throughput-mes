#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#define MAX_CONN_REQ_QUEUE 5
#define CONTROL_WORD_LENGTH 5
#define CLIENT_SEND 0
#define CLIENT_RECV 1

void nonfatal_error(const char *msg)
{
  perror(msg);
}

void fatal_error(const char *msg)
{
  perror(msg);
  exit(1);
}
void send_stream_endmark(int socket)
{
  ssize_t n;
  unsigned char end_mark = 0xFF;
  n = write(socket, &end_mark, 1);
  if (n != 1) {
    nonfatal_error("Unable to send stream end mark");
  }
}
void recv_random_payload(int socket, uint32_t length)
{
  ssize_t n;
  ssize_t remaining = length;
  char buffer[4096];
  while (remaining != 0) {
    if (remaining <= 4096) {
      n = read(socket, buffer, remaining);
    } else {
      n = read(socket, buffer, 4096);
    }
    remaining -= n;
  }
}

void send_random_payload(int socket, uint32_t length)
{
  ssize_t n;
  ssize_t remaining = length;
  char buffer[4096];
  while (remaining != 0) {
    if (remaining <= 4096) {
      n = write(socket, buffer, remaining);
    } else {
      n = write(socket, buffer, 4096);
    }
    remaining -= n;
  }
}

void handle_connection(int socket)
{
  ssize_t n;
  char buffer[4096];
  bzero(buffer, 4096);
  while (1) {
      printf("Reading control word socket\n");
      n = read(socket, buffer, CONTROL_WORD_LENGTH);
      printf("Read %ld bytes on the socket\n",n);
      if (n < 0) {
	nonfatal_error("Socket read error");
	return;
      }
      else if (n == 0) {
	nonfatal_error("Remote socket closed before sending control word\n");
	return;
      }
      else if (n != 5) {
	nonfatal_error("Bad control word size");
      }
      uint32_t length = ntohl(*(uint32_t *)(buffer+1));
      printf("length %ul\n", length);
      if (buffer[0] == CLIENT_SEND) {
	recv_random_payload(socket, length);
	send_stream_endmark(socket);
      } else if (buffer[0] == CLIENT_RECV) {
	send_random_payload(socket, length);
	send_stream_endmark(socket);
      } else {
	nonfatal_error("faulty control word");
	return;
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
   #if 0
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
    #endif
    //remove
    handle_connection(newsocketfd);
    close(newsocketfd);
  }
}
