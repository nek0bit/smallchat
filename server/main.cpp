#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "server.hpp"


int main()
{
    Server server{1028};
    
    server.startServer();
    
    return 0;
}
