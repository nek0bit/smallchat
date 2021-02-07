#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <memory>
#include <ncurses.h>

struct Name
{
    std::string name;
    uint8_t color;
    bool enabled;
};

struct Message
{
    Name username;
    std::string msg;
};

struct Backlog
{
    Backlog(int width, int height);
    ~Backlog() = default;

    void addMessage(std::string msg);
    void addMessage(std::string name, std::string msg, uint8_t nameColor = 2);

    void drawBacklog();
    
    int width;
    int height;

    int scrollBack;
private:
    std::vector<Message> log;
};
