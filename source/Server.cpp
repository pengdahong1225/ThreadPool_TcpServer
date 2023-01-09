#include "Server.h"

CSockServer::CSockServer(std::string ip,u_short port,int count):ip_(ip),port_(port),
    count_(count)
{
    this->listenfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	assert(this->listenfd != -1);
    memset(&this->serAddr,0,sizeof(this->serAddr));
    this->serAddr.sin_family = AF_INET;
    this->serAddr.sin_addr.s_addr = inet_addr(this->ip_.c_str());
    this->serAddr.sin_port = htons(this->port_);
    int retb = bind(listenfd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    assert(retb!=-1);
    int retl = listen(this->listenfd,this->count_);
    assert(retl != -1);
    this->size_ = 0;
}
CSockServer::~CSockServer()
{
    shutdown(listenfd,SHUT_RDWR);
}
void CSockServer::ServerRun()
{
    /* 启动线程池 */
    ThreadPool pool(10);
    pool.start();

    /* 主线程接收客户端连接,新连接加入任务队列 */
    struct sockaddr_in clitAddr;
    memset(&clitAddr, 0, sizeof(clitAddr));
    socklen_t addrlen = sizeof(clitAddr);
    while(true)
    {
        if (size_ >= count_)
		{
			std::cout << "server has reach peak!" << std::endl;
			//...
			getchar();
		}
		int connfd = accept(this->listenfd, (struct sockaddr*)&clitAddr, &addrlen);
		pool.push_task(new Task(connfd, clitAddr));
		this->size_++;
    }
}