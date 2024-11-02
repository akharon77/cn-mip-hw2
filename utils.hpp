#pragma once

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

#define QUOTE(s) #s

int processArgs (int argc, char* argv[], in_addr_t* servAddr, in_port_t* servPort);
int inputMessage (char* buf, size_t bufSize);
void printAddress (const sockaddr_in* sockaddr);