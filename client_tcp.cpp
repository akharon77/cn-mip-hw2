#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>

#define BUFFER_SIZE 2048
#define QUOTE(s) #s

int processArgs (int argc, char* argv[], in_addr_t* servAddr, in_port_t* servPort) {
    assert (argv != nullptr);
    assert (servAddr != nullptr);
    assert (servPort != nullptr);

    int status = 0;

    if (argc < 3) {
        printf ("Invalid arguments\n");
        return -1;
    }

    status = inet_pton (AF_INET, argv[1], servAddr);
    if (status <= 0) {
        perror ("Invalid server address");
        return -1;
    }

    *servPort = atoi (argv[2]);
    if (servPort == 0) {
        printf ("Invalid port\n");
        return -1;
    }

    return 0;
}

int inputMessage (char* buf) {
    assert (buf != nullptr);

    int i = 0;

    for (i = 0; i < BUFFER_SIZE; ++i) {
        char c = getchar();

        if (c == '\n' || c == EOF) {
            buf[i] = '\0';
            return i;
        }

        buf[i] = c;
    }

    return i;
}

int sendMessage (int sock, char *buf, int size)
{
    int sizeSent = 0;
    while (sizeSent != size) {
        int res = send(sock, buf + sizeSent, size - sizeSent, 0); 
        if (res == 0) {
            return -1;
        }
        sizeSent += res;
    }
    return 0;
}

int communicateToServer (int sock, char *buf) {
    int res = 0;
    printf("> ");
    while (true)
    {
        int size = inputMessage(buf);
        res = sendMessage(sock, buf, size);
        if (res < 0) {
            printf ("Something went wrong with sending message to server\n");
            return -1;
        }
        if (size < BUFFER_SIZE) {
            return 0;
        }
    }
}

int communicateFromServer (int sock, char *buf) {
    printf ("< ");
    while (true) {
        int size  = recv(sock, buf, BUFFER_SIZE - 1, 0); 
        if (size < -1) {
            printf ("Something went wrong with receiving message from server\n");
            return -1;
        }
        buf[BUFFER_SIZE - 1] = '\0';
        printf ("%s", buf);
        if (size < BUFFER_SIZE - 1) {
            printf ("\n");
            return 0;
        }
    }
}

int communicate (int sock, char* buf) {
    int res = 0;
    while (true) {
        char action = '\0';
        printf ("[(m)sg/(e)xit]? ");
        scanf("%c", &action);
        getchar();
        if (action == 'e') {
            return 0;
        }

        res = communicateToServer (sock, buf);
        if (res < 0) {
            return -1;
        }

        res = communicateFromServer (sock, buf);
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
    status = communicate(sock, buf);
    if (status < 0) {
        return -1;
    }

    free (buf);
    close (sock);

    return 0;
}