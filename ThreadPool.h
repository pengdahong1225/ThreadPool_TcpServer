#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>
#include <Winsock2.h>

class Task
{
public:
    Task(SOCKET sockfd, struct sockaddr_in clitAddr);
    void run();
private:
    SOCKET sockfd_;
    struct sockaddr_in clitAddr_;
};

/* 线程池 */
class ThreadPool
{
public:
    ThreadPool(int count);
    ~ThreadPool()
    {
        stop();
    }
    void start();
    void stop();

    void push_task(Task* task);
private:
    void work();
    std::vector<std::thread> threads;
    std::queue<Task*> Tasks;         //任务队列（临界区）
    std::mutex mtx_;
    std::atomic_bool running_flag;
    std::condition_variable cv_;
};

