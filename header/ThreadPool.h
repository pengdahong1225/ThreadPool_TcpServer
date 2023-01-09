#pragma once
#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

class Task
{
public:
    Task(int sockfd, struct sockaddr_in clitAddr);
    void run();
private:
    int sockfd_;
    struct sockaddr_in clitAddr_;
};

/* 线程池 */
class ThreadPool
{
public:
    ThreadPool(int count);
    ~ThreadPool();
    void start();
    void stop();
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    void push_task(Task* task);
private:
    void work();
    std::vector<std::thread> threads;
    std::queue<Task*> tasks;           //任务队列(临界区)
    std::mutex mtx_;
    std::atomic_bool running_flag;
    std::condition_variable cv_;
};