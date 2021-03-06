#include "backlog.hpp"

Backlog::Backlog(int width, int height)
    : width{width},
      height{height},
      scrollBack{0}
{}

void Backlog::addMessage(std::string msg)
{
    addMessage("", msg);
}

void Backlog::addMessage(std::string name, std::string msg, uint8_t nameColor)
{
    updateLock.lock();
    try
    {
        log.push_back(Message{{name, nameColor, name == "" ? false : true}, msg});
    }
    catch(const std::exception& err)
    {
        updateLock.unlock();
    }
    updateLock.unlock();
}

void Backlog::drawBacklog()
{
    drawLock.lock();
    // Clear backlog
    for (uint16_t i = 0; i < height; ++i)
    {
        move(i, 0);
        clrtoeol();
        move(0, 0);
    }


    const int offset = log.size() > static_cast<size_t>(height-1) ? log.size()-height+1 : 0;
    // Draw messages
    for (uint16_t i = 0; i < (log.size() > static_cast<size_t>(height-1) ? height-1 : log.size()); ++i)
    {
        move(i, 0);
        if (log[i+offset].username.enabled)
        {
            attron(COLOR_PAIR(log[i+offset].username.color));
            printw("<%s> ", log[i+offset].username.name.c_str());
            attroff(COLOR_PAIR(log[i+offset].username.color));
        }
        printw(log[i+offset].msg.c_str());
    }
    drawLock.unlock();
}
