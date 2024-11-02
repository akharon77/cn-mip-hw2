#include "udp.hpp"

#define BUFFER_SIZE 2048
#define QUOTE(s) #s

int communicate (int sock, char* buf, size_t bufSize, struct sockaddr_in *sockAddr) {
    assert (buf != nullptr);

    int res = 0;
    while (true) {
        res = communicateFrom (sock, buf, bufSize, sockAddr, true);
        if (res < 0) {
            return -1;
        }

        char action = '\0';
        printf ("[(m)sg/(s)kip]? ");
        scanf("%c", &action);
        getchar();
        if (action == 's') {
            return 0;
        }

        res = communicateTo (sock, buf, bufSize, sockAddr);
        if (res < 0) {
            return -1;
        }
    }
}

int main(int argc, char* argv[])
{
    in_addr_t servAddr = {};
    in_port_t servPort = {};

    int status = 0;

    status = processArgs(argc, argv, &servAddr, &servPort);
    if (status < 0) {
        return -1;
    }

    int sock = socket (AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in servSockAddr = {};
    servSockAddr.sin_family = AF_INET;
    servSockAddr.sin_port = htons(servPort);
    servSockAddr.sin_addr.s_addr = servAddr;

    status = bind (
               sock,
               (struct sockaddr*)&servSockAddr, 
               sizeof (servSockAddr)
             );
    
    if (status < 0) {
        perror (NULL);
        return -1;
    }

    char* buf = (char*) calloc (BUFFER_SIZE, sizeof (char));
    struct sockaddr_in clientSockAddr = {.sin_addr = 2};
    socklen_t len = 0;

    while (true) {
        status = communicate (sock, buf, BUFFER_SIZE, &clientSockAddr);

        char action = '\0';
        printf ("exit: [(y)es/(n)o]? ");
        scanf("%c", &action);
        getchar();
        if (action == 'y') {
            break;
        }
    }

    free (buf);
    close (sock);

    return 0;
}
