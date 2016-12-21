#pragma once

#include <stdio.h>
#include <stdlib.h>

static inline void nonfatal_error(const char *msg)
{
  perror(msg);
}

static inline void fatal_error(const char *msg)
{
  perror(msg);
  exit(1);
}

void handle_connection(int socket);
