#include "tcp.hpp"

#define BUFFER_SIZE 2048
#define QUOTE(s) #s

int communicate (int sock, char* buf, size_t bufSize) {
    assert (buf != nullptr);

    int res = 0;
    while (true) {
        res = communicateFrom (sock, buf, bufSize);
        if (res < 0) {
            return -1;
        }

        char action = '\0';
        printf ("[(m)sg/(r)efuse]? ");
        scanf("%c", &action);
        getchar();
        if (action == 'r') {
            shutdown (sock, SHUT_RDWR);
            close (sock);
            return -2;
        }

        res = communicateTo (sock, buf, bufSize);
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

    int sock = socket (AF_INET, SOCK_STREAM, 0);

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
    struct sockaddr_in clientSockAddr = {};
    socklen_t len = 0;

    while (true) {
        listen (sock, 1);


        int clientSock = accept (
                           sock,
                           (struct sockaddr*)&clientSockAddr,
                           &len
                         );

        printf ("Connected with ");
        printAddress (&clientSockAddr);
        printf ("\n");

        status = communicate (clientSock, buf, BUFFER_SIZE);

        char action = '\0';
        printf ("[(w)ait, (e)xit]? ");
        scanf("%c", &action);
        getchar();
        if (action == 'e') {
            break;
        }
    }

    free (buf);
    close (sock);

    return 0;
}
