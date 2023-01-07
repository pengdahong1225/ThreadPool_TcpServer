#include "Server.h"

int main()
{
    CSockServer server("192.168.1.6", 8888, 100);
    server.ServerRun();
    return 0;
}
