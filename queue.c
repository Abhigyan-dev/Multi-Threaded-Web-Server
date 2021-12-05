#include "queue.h"
#include "multi_threaded_server.h"

node_t *head = NULL;
node_t *tail = NULL;

void enqueue(request *client_socket)
{
    node_t *newnode = malloc(sizeof(node_t));
    newnode->client_socket = client_socket;
    newnode->next = NULL;
    if (tail == NULL)
    {
        head = newnode;
    }
    else
    {
        tail->next = newnode;
    }
    tail = newnode;
}

//returns NULL if the queue is the empty
//returns the pointer to the clien_socket, if there is one to get

request *dequeue()
{
    if (head == NULL)
    {
        return NULL;
    }
    else
    {
        request *first_node = head->client_socket;
        node_t *temp = head;

        head = head->next;

        if (head == NULL)
        {
            tail = NULL;
        }
        free(temp);
        return first_node;
    }
}