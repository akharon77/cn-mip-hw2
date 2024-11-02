#include "utils.hpp"

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

int inputMessage (char* buf, size_t bufSize) {
    assert (buf != nullptr);

    int i = 0;

    for (i = 0; i < bufSize; ++i) {
        char c = getchar();

        if (c == '\n' || c == EOF) {
            buf[i] = '\0';
            return i;
        }

        buf[i] = c;
    }

    return i;
}
