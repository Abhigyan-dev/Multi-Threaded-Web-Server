#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdio.h>
#include <stdlib.h>
#include "multi_threaded_server.h"

//node:
struct node
{
    struct node *next;
    request *client_socket;
};

typedef struct node node_t;

void enqueue(request *client_socket);
request *dequeue();

#endif