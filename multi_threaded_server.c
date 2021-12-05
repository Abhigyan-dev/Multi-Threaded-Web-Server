#include "multi_threaded_server.h"
#include "server.h"
#include "handler.h"
#include "helper.h"
#include "queue.h"

pthread_t thread_pool[THREAD_POOL_SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaphore;

struct timeval arrival, dispatch, completion, beginning, ending;

request *buffer[MAX_INPUT];
char *schedulingAlgo;
int fillptr, client_num;

void *thread_function(void *arg)
{
    while (1)
    {
        sleep(10);

        //Wait for a new request
        sem_wait(&semaphore);

        //Use mutex lock so that the accessing the queue becomes an atomic operation.
        pthread_mutex_lock(&mutex);

        //Store the dispatch time: time at which the request first starts getting handled.
        gettimeofday(&dispatch, NULL);

        request *req;
        int p_client;

        //Based on the scheduling algorithm, we have:

        //First Input First Output
        if (strcmp(schedulingAlgo, "FIFO") == 0)
        {
            req = (request *)dequeue();
            p_client = req->fd;
        }

        // Else if any one of Shortest File First, High Priority Static Content or High Priority Static Content
        else if (strcmp(schedulingAlgo, "SFF") == 0)
        {
            req = (request *)buffer[0];
            p_client = req->fd;

            buffer[0] = buffer[fillptr - 1];
            buffer[fillptr - 1] = NULL;
            fillptr--;
            if (fillptr > 1)
            {
                qsort(buffer, fillptr, sizeof(*buffer), cmp_func);
            }
        }

        if (req->pid < 10)
            printf("pid=%d,  size=%d\n", req->pid, req->size);
        else
            printf("pid=%d, size=%d\n", req->pid, req->size);
        fflush(stdout);

        //Get the dispatch time- the time at which the server first started handling the client's request.
        req->dispatch_time = (double)dispatch.tv_usec / 1000000 + (double)dispatch.tv_sec;

        pthread_mutex_unlock(&mutex);

        handle_connection(p_client, req->arrival_time, req->dispatch_time, req->pid);

        if (client_num == 90)
        {
            gettimeofday(&ending, NULL);
            double throughput_batch = (double)(ending.tv_usec - beginning.tv_usec) / 1000000 + (double)(ending.tv_sec - beginning.tv_sec);
            throughput_batch = throughput_batch / 90;
            //printf("\nThroughput: %f\n", throughput_batch);
        }
    }
}

int main(int argc, char **argv)
{
    int server_socket, client_socket;

    //Defining the scheduling algorithm
    schedulingAlgo = argv[1];

    //Initialising:
    sem_init(&semaphore, 0, 0);
    fillptr = 0;
    client_num = 0;

    //Creating thread pool
    for (int i = 0; i < THREAD_POOL_SIZE; i++)
        pthread_create(&thread_pool[i], NULL, thread_function, NULL);

    //Create the server and get it's socket id:
    server_socket = create_server(SERVER_PORT);

    //Now that we have created a server, we will start and keep listening for incoming client requests.

    //Keep listening
    while (1)
    {
        //printf("Waiting for connection on port %d\n", SERVER_PORT);
        //fflush(stdout);

        //Accept waits until a client tries to connect, in which case it accepts.
        //Once we accept the connection, define a new socket client_socket - for file transfer between client and server.

        client_socket = accept(server_socket, (struct sockaddr *)NULL, NULL);

        if (client_socket < 0)
        {
            error("Accept failed");
        }

        //Store the arrival time of the request:
        gettimeofday(&arrival, NULL);

        // printf("Connected!");
        // fflush(stdout);

        //Once connected to the client - check for lock:
        //Updating buffer is the critical section of our code, so it is inside thread lock:
        pthread_mutex_lock(&mutex);

        //Beginning time is the time at which server started handling this batch of requests.
        if (client_num == 0)
            gettimeofday(&beginning, NULL);
        client_num++;

        request *new_request;
        new_request = malloc(sizeof(request));
        new_request->fd = client_socket;
        new_request->size = getFileSize(new_request->fd);
        new_request->arrival_time = (double)arrival.tv_usec / 1000000 + (double)arrival.tv_sec;
        new_request->pid = client_num;

        // printf("\n pid = %d ,size-of-file = %d\n", client_num, new_request->size);
        // fflush(stdout);

        //If scheduling algorithm to use is First Input First Output
        if (strcmp(schedulingAlgo, "FIFO") == 0)
        {
            //In case of FIFO, queue is the buffer:
            enqueue(new_request);
        }

        //If scheduling algorithm to Shortest File First
        else if (strcmp(schedulingAlgo, "SFF") == 0)
        {
            if (new_request->size == -1)
                continue;

            buffer[fillptr] = new_request;
            fillptr++;

            //printbuff();
            if (fillptr > 1)
            {
                qsort(buffer, fillptr, sizeof(*buffer), cmp_func);
            }
            //printbuff();
        }

        sem_post(&semaphore);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}