#include "Server.h"

int main()
{
    CSockServer server("121.37.10.164", 8888, 100);
    server.ServerRun();
    return 0;
}
