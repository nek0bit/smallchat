#include "client.hpp"

Client::Client()
    : exit{false},
      winX{0},
      winY{0},
      key{0}
{
    // Init ncurses
    initscr();
    // Disable echo on keypress
    noecho();
    // Enable keypad buttons like F1 or Arrow keys
    keypad(stdscr, TRUE);
    // Enable colors
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    getmaxyx(stdscr, winY, winX);
    
    drawBuffer();
    refreshScreen();
}

Client::~Client()
{
    // Cleanup ncurses
    endwin();
}

void Client::handleInputs()
{
    
    key = getch();
    
    switch(key)
    {
    case KEY_F(8): // F8
        exit = true;
    case 127: // BACKSPACE
        if (msgBuffer.size() != 0) msgBuffer.pop_back();
        drawBuffer();
        refreshScreen();
        break;
    default:
        updateBuffer();
        drawBuffer();
        refreshScreen();
        break;
    }
}

void Client::updateBuffer()
{
    msgBuffer += static_cast<char>(key);    
}

void Client::drawBuffer()
{
    std::string nothing(winX, ' ');
    
    // Redraw buffer
    attron(COLOR_PAIR(1));
    mvprintw(winY-1, 0, nothing.c_str());
    mvprintw(winY-1, 0, msgBuffer.c_str());
    move(winY-1, msgBuffer.size() > static_cast<size_t>(winX) ? winX-1 : msgBuffer.size());
    attroff(COLOR_PAIR(1));
}

void Client::refreshScreen()
{
    getmaxyx(stdscr, winY, winX);
    refresh();
}
