#include "ThreadPool.h"

Task::Task(int sockfd, struct sockaddr_in clitAddr):sockfd_(sockfd),clitAddr_(clitAddr){}

void Task::run()
{
	std::cout << "connfd = " << this->sockfd_ << std::endl;
    /* recv and send */
    char rbuf[1024];
	char sbuf[1024];
    while (true)
    {
        memset(rbuf, 0, sizeof(rbuf));
		memset(sbuf, 0, sizeof(sbuf));
		int len = recv(this->sockfd_, rbuf, sizeof(rbuf), 0);
        if (len < 0)
			continue;
		else if (len == 0)
		{
			shutdown(this->sockfd_, SHUT_RDWR);
			/*日志*/
			//...
			std::cout << "break link" << std::endl;
			break;
		}
		else
		{
			std::cout << "from " << inet_ntoa(this->clitAddr_.sin_addr) << ":";
			std::cout << rbuf << std::endl;
			sprintf(sbuf, "ok\r\nHahaha,I want to say Messi is Goat!\r\n");
			send(this->sockfd_, sbuf, sizeof(sbuf), 0);
		}
    }
}

ThreadPool::ThreadPool(int count)
{
    running_flag = false;
    for(int i=0;i<count;i++)
        threads.emplace_back(&ThreadPool::work,this);
}

ThreadPool::~ThreadPool()
{
    stop();
}

void ThreadPool::start()
{
    running_flag = true;
}

void ThreadPool::stop()
{
    /* 日志 */
	//...
    running_flag = false;
    std::cout << "stop!" << std::endl;
    cv_.notify_all();
    for(auto& th : threads)
    {
        if(th.joinable())
            th.join();
    }
}

void ThreadPool::push_task(Task* task)
{
    std::unique_lock<std::mutex> ulock(mtx_);
    this->tasks.push(task);
    ulock.unlock();
    cv_.notify_one();
}

void ThreadPool::work()
{
    std::cout<<std::this_thread::get_id()<<std::endl;
    while(!running_flag);   //wait pool start (difference with Windows)
    while(running_flag)
    {
        Task* task = nullptr;
        //get task from task's queue
        {
            std::unique_lock<std::mutex> ulock(mtx_);
            while(this->tasks.empty())
                cv_.wait(ulock);
            if(running_flag)
            {
                task = this->tasks.front();
                tasks.pop();
            }
        }
        if(task!=nullptr)
            task->run();
    }
	return; //exit this thread
}