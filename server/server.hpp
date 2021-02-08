#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <exception>
#include <unordered_map>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

struct Connection
{
    int id;
    std::string username;
};

struct Server
{
    Server(int port);
    ~Server();

    void startServer();
    
private:
    int fd;
    bool exit;
    sockaddr_storage addr;
    socklen_t addrSize;
    int connectionHandle(int fd);
    
    void parseName(std::string& name);
    void parseMsg(std::string& message);
    void sendMsgTo(std::string to, std::string msg);

    std::vector<std::thread> openThreads;
    std::unordered_map<std::string, Connection> connections;
};
