#include "client.h"

//custom error function
void error(char *str)
{
    printf("%s", str);
    exit(0);
}

int main(int argc, char **argv)
{
    int sockfd, n, sendbytes, ipTextToBinaryCheck, connectCheck, writeCheck;
    struct sockaddr_in servaddr;
    char sendline[MAX_INPUT], recvline[MAX_INPUT];
    char *ip_server = "127.0.0.1";

    //If the input is not correct/standard as specified.
    if (argc != 2)
    {
        error("Input Incorrect");
    }

    //Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //using IPv4 domain, input as stream , TCP protocol
    //If socket could not be created successfully
    if (sockfd < 0)
    {
        error("Error while creating the socket!");
    }

    //Define servaddr
    bzero(&servaddr, sizeof(servaddr)); //zero out any previous values
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERVER_PORT);

    // Convert IP addresses from text to binary form, and report error if any.
    ipTextToBinaryCheck = inet_pton(AF_INET, ip_server, &servaddr.sin_addr);
    //If error in converting IP address
    if (ipTextToBinaryCheck <= 0)
    {
        error("IP address could not be converted/parsed.\n");
    }

    //Connect to the server
    connectCheck = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    //If error in connection
    if (connectCheck < 0)
    {
        error("\nConnection Failed\n");
    }

    //Now we are connected to the server.

    //Define the request to be sent to the server
    char requestline[] = "GET ";
    strcat(requestline, argv[1]);
    strcat(requestline, " HTTP/1.1\r\n\r\n");

    printf("REQ:|%s|\n",requestline);

    strcpy(sendline, requestline);
    sendbytes = strlen(sendline);

    /* Another way to send request file to server is using send() 
	            
                send(sock , hello , strlen(hello) , 0 );*/

    //Send the request-file to the server
    writeCheck = write(sockfd, sendline, sendbytes);

    if (writeCheck != sendbytes)
    {
        error("Request-file Error!");
    }

    //Read the server's response
    memset(recvline, 0, MAX_INPUT);
    while ((n = read(sockfd, recvline, MAX_INPUT - 1)) > 0)
    {
        printf("%s", recvline);
        memset(recvline, 0, MAX_INPUT);
    }

    if (n < 0)
    {
        error("Read-file error");
    }
    return 0;
}
