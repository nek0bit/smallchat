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
    char namebuf[20] = "";
    char sendto[20] = "";
    char msgBuffer[2048] = "";
    std::string sendtoStr, namebufStr, msgBufferStr;

    // Recieve username as first request
    if (recv(clientFd, namebuf, sizeof(namebuf), 0) != -1)
    {
        namebufStr = namebuf;
        parseName(namebufStr);
        
        connections.insert({namebufStr, {clientFd, namebufStr}});
    } else {
        std::cerr << "[thread] Couldn't recieve" << std::endl;
        close(clientFd);
        return -1;
    }

    if (recv(clientFd, sendto, sizeof(sendto), 0) != -1)
    {
        sendtoStr = sendto;
        parseName(sendtoStr);        
    } else {
        std::cerr << "[thread] Couldn't recieve" << std::endl;
        close(clientFd);
        return -1;
    }

    while (true)
    {
        recv(clientFd, msgBuffer, sizeof(msgBuffer), 0);
        msgBufferStr = msgBuffer;

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
        send(found.id, msg.c_str(), sizeof(msg.c_str()), 0);
    }
    catch(const std::out_of_range& err) {}
}
