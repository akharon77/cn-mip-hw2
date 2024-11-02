#include "udp.hpp"

int sendMessage (int sock, char *buf, int size, struct sockaddr_in *sockAddr)
{
    assert (buf != nullptr);

    int sizeSent = 0;
    while (sizeSent != size) {
        int res = sendto (
                    sock,
                    buf + sizeSent,
                    size - sizeSent,
                    0,
                    (struct sockaddr*)sockAddr,
                    sizeof (struct sockaddr_in)
                  ); 
        if (res <= 0) {
            perror (NULL);
            return -1;
        }
        sizeSent += res;
    }
    return 0;
}

int receiveMessage (int sock, char *buf, size_t bufSize, struct sockaddr_in *sockAddr) {
    socklen_t len = sizeof(struct sockaddr_in);
    while (true) {
        int size  = recvfrom (
                      sock,
                      buf,
                      bufSize - 1,
                      MSG_WAITALL,
                      (struct sockaddr*)sockAddr,
                      &len
                    ); 
        if (size < 0) {
            perror (NULL);
            return -1;
        }
        if (size < bufSize - 1) {
            buf[size] = '\0';
            printf ("%s", buf);
            return 0;
        }
        else {
            buf[bufSize - 1] = '\0';
            printf ("%s", buf);
        }
    }
}

int communicateTo (int sock, char *buf, size_t bufSize, struct sockaddr_in *sockAddr) {
    assert (buf != nullptr);

    int res = 0;
    printf("> ");
    while (true)
    {
        int size = inputMessage (buf, bufSize);
        res = sendMessage (sock, buf, size, sockAddr);
        if (res < 0) {
            return -1;
        }
        if (size < bufSize) {
            return 0;
        }
    }
}

int communicateFrom (int sock, char *buf, size_t bufSize, struct sockaddr_in *sockAddr, bool fromFlag) {
    assert (buf != nullptr);

    printf ("< ");
    int status = receiveMessage (sock, buf, bufSize, sockAddr);
    if (fromFlag) {
        printf (" from [");
        printAddress (sockAddr);
        printf ("]");
    }
    printf ("\n");
}

