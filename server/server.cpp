#include "server.hpp"

Server::Server(int port)
    : fd{},
      exit{false},
      openThreads{},
      connections{}
{
    std::string s_port{std::to_string(port)};
    
    addrinfo opt, *res;

    int one = 1;

    // Make sure it's empty
    memset(&opt, 0, sizeof(opt));
    // Set options
    opt.ai_family = AF_UNSPEC; // IPv4 or IPv6
    opt.ai_socktype = SOCK_STREAM; // TCP
    opt.ai_flags = AI_PASSIVE; // Fill host info for me
    
    if (getaddrinfo(NULL, s_port.c_str(), &opt, &res) != 0)
    {
        throw std::runtime_error("Failed to getaddrinfo!");
    }

    if ((fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
    {
        throw std::runtime_error("Failed to open socket");
    }

    // Allow reuse of file descriptor
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));

    if (bind(fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        throw std::runtime_error("Failed to bind socket");
    }

    freeaddrinfo(res);
}

Server::~Server()
{
    exit = true;
    // Wait for threads to finish
    for (auto& thread: openThreads)
    {
        thread.join();
    }

    close(fd);
}

// THREAD FUNCTION
int Server::connectionHandle(int clientFd)
{
    char fullbuf[40];
    std::string fullbufStr, sendtoStr, namebufStr;

    int err;

    // Recieve username as first request
    try
    {
        if ((err = recv(clientFd, fullbuf, 20, 0)) != -1)
        {
            fullbufStr = fullbuf;
            size_t find = fullbufStr.find(";");
            namebufStr = fullbufStr.substr(0, find);
            sendtoStr = fullbufStr.substr(find+1, fullbufStr.size());
        
            parseName(namebufStr);
            parseName(sendtoStr);
        
            connections.insert({namebufStr, {clientFd, namebufStr}});
        }
        else
        {
            close(fd);
            return -1;
        }
    }
    catch(const std::out_of_range& err) {}

    while (true)
    {
        char msgBuffer[1024];
        int bs;
        bs = recv(clientFd, msgBuffer, 1023, 0);
        msgBuffer[bs] = '\0';
        
        std::string msgBufferStr = msgBuffer;

        if (msgBufferStr[0] == 'q' && msgBufferStr[1] == ';')
            break;
        
        parseMsg(msgBufferStr);
        sendMsgTo(sendtoStr, msgBufferStr);
    }
    
    close(clientFd);
    return 0;
}

void Server::startServer()
{
    listen(fd, 10);

    // TODO exit safely!!!
    while (true)
    {
        int currFd = accept(fd, (sockaddr*)&addr, &addrSize);
        openThreads.push_back(std::thread(&Server::connectionHandle, this, currFd));
    }
}

void Server::parseName(std::string& name)
{
    name.erase(std::remove(name.begin(), name.end(), '\n'), name.end());
}

void Server::parseMsg(std::string& message)
{

}

void Server::sendMsgTo(std::string to, std::string msg)
{
    try
    {
        auto found = connections.at(to);
        msg = "m;" + msg;
        send(found.id, msg.c_str(), strlen(msg.c_str()), 0);
    }
    catch(const std::out_of_range& err) {}
}
