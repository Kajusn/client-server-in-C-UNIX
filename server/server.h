#ifndef server_H
#define server_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "response.h"

#define MAX_BUFFER_LEN 1024
#define SERVER_PORT 55555
#define SLA 500

void start_server();

#endif
