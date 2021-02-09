#include <iostream>

#include "client.hpp"
#include "listener.hpp"

int main(int argc, char* argv[])
{
    Client client{};
    Listener server{"localhost", 1028, client};

    server.startThread();
    
    while (!client.exit)
    {
        client.handleInputs();
    }
    
    return 0;
}
