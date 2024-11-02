#include "tcp.hpp"

#define BUFFER_SIZE 2048
#define QUOTE(s) #s

int main(int argc, char* argv[])
{
    in_addr_t servAddr = {};
    in_port_t servPort = {};

    int status = 0;

    status = processArgs(argc, argv, &servAddr, &servPort);
    if (status < 0) {
        return -1;
    }

    int sock = socket (AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servSockAddr = {};
    servSockAddr.sin_family = AF_INET;
    servSockAddr.sin_port = htons(servPort);
    servSockAddr.sin_addr.s_addr = servAddr;

    status = connect (
               sock,
               (struct sockaddr*)&servSockAddr, 
               sizeof (servSockAddr)
             );
    
    if (status < 0) {
        perror (NULL);
        return -1;
    }

    char* buf = (char*) calloc (BUFFER_SIZE, sizeof (char));
    status = communicate(sock, buf, BUFFER_SIZE);
    if (status < 0) {
        return -1;
    }

    free (buf);
    close (sock);

    return 0;
}