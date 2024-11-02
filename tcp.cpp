#include "tcp.hpp"

int sendMessage (int sock, char *buf, int size)
{
    assert (buf != nullptr);

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

int receiveMessage (int sock, char *buf, size_t bufSize) {
    while (true) {
        int size  = recv(sock, buf, bufSize - 1, 0); 
        if (size < -1) {
            printf ("Something went wrong with receiving message from server\n");
            return -1;
        }
        buf[bufSize - 1] = '\0';
        printf ("%s", buf);
        if (size < bufSize - 1) {
            printf ("\n");
            return 0;
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

int communicate (int sock, char* buf, size_t bufSize) {
    assert (buf != nullptr);

    int res = 0;
    while (true) {
        char action = '\0';
        printf ("[(m)sg/(e)xit]? ");
        scanf("%c", &action);
        getchar();
        if (action == 'e') {
            return 0;
        }

        res = communicateTo (sock, buf, bufSize);
        if (res < 0) {
            return -1;
        }

        res = communicateFrom (sock, buf, bufSize);
        if (res < 0) {
            return -1;
        }
    }
}
