#include <iostream>

#include "client.hpp"
#include "listener.hpp"

int main(int argc, char* argv[])
{
    Client client{argv[2], argv[3]};
    Listener server{argv[1], 1028, client};

    client.fd = server.fd;

    server.startThread();
    
    while (!client.exit)
    {
        client.handleInputs();
    }
    
    return 0;
}
