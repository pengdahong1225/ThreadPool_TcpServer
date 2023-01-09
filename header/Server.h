#pragma once
#include "ThreadPool.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>

/* Server--主线程 */
class CSockServer
{
public:
    CSockServer(std::string ip,u_short port,int count);
    ~CSockServer();
    CSockServer(const CSockServer&) = delete;
    CSockServer& operator=(const CSockServer&) = delete;
    void ServerRun();
private:
    std::string ip_;
    u_short port_;
    struct sockaddr_in serAddr;
    int listenfd;
    int count_;
    int size_;
};