#pragma once

#include "utils.hpp"

int sendMessage (int sock, char *buf, int size);
int receiveMessage (int sock, char *buf, size_t bufSize);

int communicateTo (int sock, char *buf, size_t bufSize);
int communicateFrom (int sock, char *buf, size_t bufSize);
int communicate (int sock, char* buf, size_t bufSize);
