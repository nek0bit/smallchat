#include <iostream>

#include "client.hpp"

int main(int argc, char* argv[])
{
    Client client{};

    while (!client.exit)
    {
        client.handleInputs();
    }
    
    return 0;
}
