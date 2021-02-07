#include "client.hpp"

Client::Client()
    : exit{false},
      winX{0},
      winY{0},
      key{0},
      bufferIndex{0},
      at{0},
      msgBuffer{},
      log{0, 0}
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
    init_pair(2, COLOR_CYAN, 0);
    init_pair(3, COLOR_YELLOW, 0);

    getmaxyx(stdscr, winY, winX);
    log.width = winX;
    log.height = winY;
    
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
        break;
    case 127: // BACKSPACE
        if (at != 0)
        {
            msgBuffer.erase(at-1, 1);
            at--;
        }
        drawBuffer();
        refreshScreen();
        break;
    case 1: // CTRL-A
        // Emacs binding to go to beginning of string
        at = 0;
        drawBuffer();
        refreshScreen();
        break;
    case 5: // CTRL-E
        // Emacs binding to go to end of string
        at = msgBuffer.size();
        drawBuffer();
        refreshScreen();
        break;
    case 10: // ENTER
        sendMessageBuffer();
        break;
    case KEY_LEFT:
        if (static_cast<size_t>(at) != 0) at--;
        drawBuffer();
        refreshScreen();
        break;
    case KEY_RIGHT:
        if (static_cast<size_t>(at) < msgBuffer.size()) at++;
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
    char key_s[] = {static_cast<char>(key)};
    msgBuffer.insert(at, key_s);
    at++;
}

void Client::drawBuffer()
{
    std::string nothing(winX, ' ');
    
    // Redraw buffer
    attron(COLOR_PAIR(1));
    // clrtoeol() replacement with colors
    mvprintw(winY-1, 0, nothing.c_str());
    mvprintw(winY-1, 0, msgBuffer.c_str());
    move(winY-1, at);
    attroff(COLOR_PAIR(1));
}

void Client::sendMessageBuffer()
{
    log.addMessage("Test", msgBuffer);
    msgBuffer = "";
    at = 0;
    log.drawBacklog();
    drawBuffer();
    refreshScreen();
}

void Client::refreshScreen()
{
    getmaxyx(stdscr, winY, winX);
    log.width = winX;
    log.height = winY;
    refresh();
}
