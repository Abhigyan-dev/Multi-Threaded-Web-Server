#ifndef _HANDLER_H
#define _HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>

#define MAX_INPUT 4096
#define BUFSIZE 4096
#define PATH_MAX 4096

int getFileSize(int client_socket);
void *handle_connection(int client_socket, double arrival_time, double dispatch_time,int pid);

#endif