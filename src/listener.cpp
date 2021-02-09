#include "listener.hpp"

Listener::Listener(std::string host, int port, Client& client)
    : client{client},
      thr{}
{
    int check;
    addrinfo hints, *serverinfo, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    std::string portStr = std::to_string(port);

    if ((check = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &serverinfo)) != 0)
    {
        throw std::runtime_error(std::string("[client] Error: getaddrinfo:") + std::string(gai_strerror(check)));
    }

    // Loop through linked list of results
    for (res = serverinfo; res != NULL; res = res->ai_next)
    {
        if ((fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
        {
            continue;
        }

        if (connect(fd, res->ai_addr, res->ai_addrlen) == -1)
        {
            close(fd);
            continue;
        }

        break;
    }

    if (res == NULL)
    {
        throw std::runtime_error("[client] Failed to connect!!!");
    }

    freeaddrinfo(serverinfo);

    // Send client info
    std::string split = client.you + ";" + client.them;
    send(fd, split.c_str(), strlen(split.c_str()), 0);
}

Listener::~Listener()
{
    close(fd);
    thr.join();
}

void Listener::startThread()
{
    if (thr.joinable()) thr.join();
    thr = std::thread{&Listener::threadMethod, this};
}

void Listener::threadMethod()
{
    while (1)
    {
        int bs;
        char msg[1024];
        
        bs = recv(fd, msg, 1023, 0);
        
        msg[bs] = '\0';
        
        client.log.addMessage(client.them, &msg[2], 3);
        client.log.drawBacklog();
        client.drawBuffer();
        client.refreshScreen();
    }
}
