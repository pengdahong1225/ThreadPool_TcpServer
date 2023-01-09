#include "Server.h"

int main()
{
    CSockServer server("192.168.111.128",8888,100);
    server.ServerRun();
    return 0;
}