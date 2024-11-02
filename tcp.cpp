#include "tcp.hpp"

int sendMessage (int sock, char *buf, int size)
{
    assert (buf != nullptr);

    int sizeSent = 0;
    while (sizeSent != size) {
        int res = send(sock, buf + sizeSent, size - sizeSent, MSG_NOSIGNAL); 
        if (res <= 0) {
            return -1;
        }
        sizeSent += res;
    }
    return 0;
}

int receiveMessage (int sock, char *buf, size_t bufSize) {
    while (true) {
        int size  = recv(sock, buf, bufSize - 1, MSG_NOSIGNAL); 
        if (size < 0) {
            printf ("Something went wrong with receiving message from server\n");
            return -1;
        }
        if (size < bufSize - 1) {
            buf[size] = '\0';
            printf ("%s\n", buf);
            return 0;
        }
        else {
            buf[bufSize - 1] = '\0';
            printf ("%s", buf);
        }
    }
}

int communicateTo (int sock, char *buf, size_t bufSize) {
    assert (buf != nullptr);

    int res = 0;
    printf("> ");
    while (true)
    {
        int size = inputMessage(buf, bufSize);
        res = sendMessage(sock, buf, size);
        if (res < 0) {
            printf ("Something went wrong with sending message to server\n");
            return -1;
        }
        if (size < bufSize) {
            return 0;
        }
    }
}

int communicateFrom (int sock, char *buf, size_t bufSize) {
    assert (buf != nullptr);

    printf ("< ");
    int status = receiveMessage (sock, buf, bufSize);
}

