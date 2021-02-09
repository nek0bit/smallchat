#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "client.hpp"

struct Listener
{
    Listener(std::string host, int port, Client& client);
    ~Listener();

    void startThread();
    int fd;
private:
    Client& client;
    void threadMethod();
    std::thread thr;
    int sockfd;
};
