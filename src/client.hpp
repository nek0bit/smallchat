#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>

#include "backlog.hpp"

struct Client
{
    Client();
    ~Client();

    void handleInputs();
    void refreshScreen();
    void sendMessageBuffer();

    bool exit;
private:
    int winX, winY;
    int key;
    
    void updateBuffer();
    void drawBuffer();
    
    int bufferIndex;
    int at;
    std::string msgBuffer;

    Backlog log;
};
