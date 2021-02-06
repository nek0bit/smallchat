#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ncurses.h>

struct Client
{
    Client();
    ~Client();

    void handleInputs();
    void refreshScreen();

    bool exit;
private:
    int winX, winY;
    int key;
    
    void updateBuffer();
    void drawBuffer();
    
    int bufferIndex;
    std::string msgBuffer;

    // Index at which to start reading backlog, can be controlled
    int readIndex;
    std::vector<std::string> msgBacklog;
};
