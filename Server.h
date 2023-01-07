#pragma once
#include "ThreadPool.h"
#include <Winsock2.h>
#include <assert.h>
#include <iostream>
#include <string>

/* Server--Ö÷Ïß³Ì */
class CSockServer
{
public:
    CSockServer(std::string ip, u_short port, int count);
    ~CSockServer()
    {
        closesocket(this->lisfd_);
    }
    CSockServer(const CSockServer&) = delete;
    CSockServer& operator=(const CSockServer) = delete;
    void ServerRun();
private:
    std::string ip_;
    u_short port_;
    struct sockaddr_in serv_addr;
    SOCKET lisfd_;
    int count_;
    int size_;
};
