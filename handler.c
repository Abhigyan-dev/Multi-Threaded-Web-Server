#include "handler.h"

char *parseString(char *buffer)
{
    char method[MAX_INPUT], Path[MAX_INPUT], version[MAX_INPUT];

    //Parse string to break the request as: "GET" , "(path/to/file)" , "(HTTP version)"
    sscanf(buffer, "%s %s %s\n", method, Path, version);

    char *pth = Path;
    return pth;
}

int getFileSize(int client_socket)
{
    char buffer[BUFSIZE];
    char actualpath[PATH_MAX + 1];

    //read the client's file from message - and get the size.

    //We take a peek at the data present on the socket.
    //So the data is returned but not consumed, so that a subsequent receive operation sees the same data.
    //This is done because we only require the request file size.
    recv(client_socket, buffer, sizeof(buffer), MSG_PEEK);

    char *buff = parseString(buffer);

    for (int i = 0; i < strlen(buff); i++)
    {
        actualpath[i] = buff[i];
    }
    actualpath[strlen(buff)] = '\0';

    FILE *fp = fopen(actualpath, "r");

    // checking if the file exist or not
    if (fp == NULL)
    {
        printf("File Not Found!\n");
        return -1;
    }

    fseek(fp, 0L, SEEK_END); //seek the end of file

    long int res = ftell(fp); // calculating the size of the file
    fclose(fp);               // closing the file

    return res;
}

void *handle_connection(int client_socket, double arrival_time, double dispatch_time, int pid)
{
    char buffer[BUFSIZE];
    size_t bytes_read;
    int msgsize = 0;
    char actualpath[PATH_MAX + 1];

    //read the client's message -- the name of the file to read
    while ((bytes_read = read(client_socket, buffer + msgsize, sizeof(buffer) - msgsize - 1)) > 0)
    {
        msgsize += bytes_read;
        if (msgsize > BUFSIZE - 1 || buffer[msgsize - 1] == '\n')
            break;
    }

    buffer[msgsize - 1] = 0; //null terminate the message and remove the \n

    // printf("REQUEST: %s\n", buffer);
    // fflush(stdout);

    char *buff = parseString(buffer);
    // printf("PATH: %s\n", buff);
    // fflush(stdout);

    //validity check
    if (realpath(buff, actualpath) == NULL)
    {
        printf("ERROR(bad path): %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    //read file and send its contents to client
    FILE *fp = fopen(actualpath, "r");
    if (fp == NULL)
    {
        printf("ERROR(open): %s\n", buffer);
        close(client_socket);
        return NULL;
    }

    //Now we will read file contents and send them to client

    //This is an insecure program,
    //and while using in the real world we would probably limit the client's access to only  certain files.

    msgsize = 0;
    while ((bytes_read = fread(buffer, 1, BUFSIZE, fp)) > 0)
    {
        //printf("sending %zu bytes\n", bytes_read);
        msgsize += bytes_read;
        write(client_socket, buffer, bytes_read);
    }
    //printf("sent %d bytes\n", msgsize);

    close(client_socket);
    fclose(fp);
    //printf("closing connection\n\n");

    struct timeval completion;
    gettimeofday(&completion, NULL);

    double completion_time = (double)completion.tv_usec / 1000000 + (double)completion.tv_sec;

    double turn_around_time = completion_time - arrival_time;
    double response_time = dispatch_time - arrival_time;

    // printf("%d %d %f %f %f %f %f\n",pid,msgsize,arrival_time,dispatch_time,completion_time,turn_around_time,response_time);s
    // fflush(stdout);

    return NULL;
}