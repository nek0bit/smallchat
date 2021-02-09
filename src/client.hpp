#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <string.h>
#include <ncurses.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "backlog.hpp"

struct Client
{
    Client(std::string you, std::string them);
    ~Client();

    void handleInputs();
    void refreshScreen();
    void sendMessageBuffer();

    
    void updateBuffer();
    void drawBuffer();

    bool exit;
    Backlog log;
    
    const std::string you;
    const std::string them;
    int fd;
private:
    int winX, winY;
    int key;
    
    
    int bufferIndex;
    int at;
    std::string msgBuffer;

};
