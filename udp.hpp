#pragma once

#include "utils.hpp"

int sendMessage (int sock, char *buf, int size, struct sockaddr_in *sockAddr);
int receiveMessage (int sock, char *buf, size_t bufSize, struct sockaddr_in *sockAddr);

int communicateTo (int sock, char *buf, size_t bufSize, struct sockaddr_in *sockAddr);
int communicateFrom (int sock, char *buf, size_t bufSize, struct sockaddr_in *sockAddr);
