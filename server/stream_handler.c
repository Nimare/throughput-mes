#include <unistd.h>
#include <stdint.h>
#include <strings.h>
#include <arpa/inet.h>

#include "server_common.h"

#define CONTROL_WORD_LENGTH 5
#define CLIENT_SEND 0
#define CLIENT_RECV 1

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
