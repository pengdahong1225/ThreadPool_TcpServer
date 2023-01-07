#include "Server.h"
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

CSockServer::CSockServer(std::string ip, u_short port, int count) :ip_(ip), port_(port), count_(count)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	this->lisfd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	/* ����־ */
	//...
	assert(this->lisfd_ != INVALID_SOCKET);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(this->ip_.c_str());
	serv_addr.sin_port = htons(this->port_);
	int retb = bind(this->lisfd_, (struct sockaddr*)&this->serv_addr, sizeof(this->serv_addr));
	/* ����־ */
	//...
	assert(retb != -1);
	int retl = listen(this->lisfd_, count);
	/* ����־ */
	//...
	assert(retl != -1);
	size_ = 0;
}

void CSockServer::ServerRun()
{
	/* �����̳߳� */
	ThreadPool pool(10);
	pool.start();

	/* ���߳̽��տͻ�������,�����Ӽ���������� */
	struct sockaddr_in clitAddr;
	memset(&clitAddr, 0, sizeof(clitAddr));
	int addrlen = sizeof(clitAddr);
	while (true)
	{
		if (size_ >= count_)
		{
			std::cout << "server has reach peak!" << std::endl;
			//...
			getchar();
		}
		int connfd = accept(this->lisfd_, (struct sockaddr*)&clitAddr, &addrlen);
		pool.push_task(new Task(connfd, clitAddr));
		this->size_++;
	}
}
