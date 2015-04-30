/* 
 *
 * File:   SERVER/Server.h
 * Author: Vitaly Borodin
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <queue>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <mutex>
#include <thread>
#include <future>
#include <pthread.h>

/*
 * 
 */

class MyServer
{

private:
	int VarInc;
	pthread_t thread_id;	

	std::queue<std::string> msg_to;

	std::mutex mtx;

	std::condition_variable cv;
	std::string msg_data;

public:
	
	MyServer();
	~MyServer();

	
	void error(const char *);
	void start(int, char**);

	void chef(int );
	void choosone(int );
	void waiter(int);
	void customer(int );
};

#endif // SERVER_H

