#ifndef _MULTI_THREADED_SERVER_H_
#define _MULTI_THREADED_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <pthread.h>
#include <semaphore.h>

#define SERVER_PORT 5000
#define MAX_INPUT 4096
#define THREAD_POOL_SIZE 200
#define BUFSIZE 4096
#define PATH_MAX 4096

typedef struct
{
	int fd;
	int pid,size;
	double arrival_time, dispatch_time, completion_time;
} request;

void error(char *str);
int cmp_func(const void *first, const void *second);
void *thread_function(void *arg);

#endif