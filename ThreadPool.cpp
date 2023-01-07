#pragma warning(disable:4996)
#include "ThreadPool.h"

Task::Task(SOCKET sockfd, sockaddr_in clitAddr):sockfd_(sockfd),clitAddr_(clitAddr){}

void Task::run()
{
	std::cout << std::this_thread::get_id() << std::endl;
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
			shutdown(this->sockfd_, SD_BOTH);
			closesocket(this->sockfd_);
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
	//添加线程
	for (int i = 0; i < count; i++)
		threads.emplace_back(&ThreadPool::work, this);
}

void ThreadPool::start()
{
	running_flag = true;
}

void ThreadPool::stop()
{
	/* 日志 */
	//...
	std::cout << "stop!" << std::endl;
	running_flag = false;
	cv_.notify_all();
	for (auto& th : threads)
	{
		if (th.joinable())
			th.join();
	}
}

void ThreadPool::push_task(Task* task)
{
	std::unique_lock<std::mutex> ulock(this->mtx_);
	this->Tasks.push(task);
	ulock.unlock();
	//唤醒一个线程（任务队列不为空了，就可以唤醒一个在等待任务的线程）
	this->cv_.notify_one();
}

void ThreadPool::work()
{
	/* 让线程池启动，所有线程在此睡眠等任务，唤醒 */
	while (running_flag)
	{
		Task* task = nullptr;
		//取出任务
		{
			std::unique_lock<std::mutex> ulock(this->mtx_);
			/* 如果任务队列为空，线程睡眠等待，直到被其他线程notify */
			while (this->Tasks.empty())//被唤醒之后要继续判断队列是否为空，防止虚假唤醒
				cv_.wait(ulock);
			if (running_flag)
			{
				task = this->Tasks.front();
				this->Tasks.pop();
			}
		}
		if (task != nullptr)
			task->run();
	}
}