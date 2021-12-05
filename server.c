#include "server.h"
#include "helper.h"

int create_server(int port_num)
{
    int server_socket, opt;
    struct sockaddr_in servaddr;
    struct sockaddr_in clientaddr;
    int bindCheck, listenCheck;

    //initialisation:
    opt = 1;

    //Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0); //using IPv4 domain, taking input as stream and protocol is TCP
    //If socket could not be created successfully
    if (server_socket < 0)
    {
        error("Error while creating the socket!");
    }

    //Forcefully attaching socket to the port, else it takes some time to re-attach the port next time we try to run the server.
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //Define servaddr
    bzero(&servaddr, sizeof(servaddr)); //zero out any previous values
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port_num);

    //Bind the socket address to the listen_fd socket descriptor
    bindCheck = bind(server_socket, (struct sockaddr *)&servaddr, sizeof(servaddr));
    //Check if error occured while binding
    if (bindCheck < 0)
    {
        error("Bind error");
    }

    //Start listening on the socket
    listenCheck = listen(server_socket, SERVER_BCKLG);
    //Check if any occurred while listening
    if (listenCheck < 0)
    {
        error("listen error");
    }

    return server_socket;
}