/* 
 *
 * File:   SERVER/server.cpp
 * Author: Vitaly Borodin
 *
 */

#include <queue>
#include <time.h>
#include <sstream>
#include <unistd.h>
#include "MyServer.h"

using namespace std;

/*
 * 
 */

MyServer::MyServer()
{
}
MyServer::~MyServer()
{
}

void MyServer::start(int argc, char** argv)
{ 
	int sockfd = 0,
		newsockfd = 0,
		portno = 16383,
		pid = 0;
	printf("The SERVER!\n");
	string MSG_buck;

	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;

	if (argc < 2)
	{
		// cheking fot user input of the port
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");

	bzero( (char *) &serv_addr, sizeof(serv_addr) );
	portno = atoi(argv[1]);
	
	serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
             error("ERROR on binding");
	
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
    
	// 
	// Start listening 
	while (true)
	{
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) 
	         error("ERROR on accept");
		// making new thrad for each connaction 
		// for claents.
		pid = fork();
		
		if (pid < 0)
			error("ERROR on fork");

		if (pid == 0)
		{
			close(sockfd);
			choosone( newsockfd );
			exit(0);
		}
		else
			close(newsockfd);
	}
	
    close(sockfd);
}
/*************************************************/

void MyServer::customer(int sock )
{
	// thrad for customer
	//
	printf("new customer here!\n");
	while(true)
	{
		int n;
		char buffer[256];
		bzero(buffer,256);

		// send to customer his id number.
		n = write(sock,"1023",5);
		if (n < 0) error("ERROR writing to socket");

		// reading the flight number from the customer.
		n = read(sock,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		printf("Flight: %s\n",buffer);

		// send respons thet I got message
		bzero(buffer,256);
	   	if (n < 0) error("ERROR writing to socket");
		else
		{			// chenge it to flit number is ###
			n = write(sock,"I got your flight number",18);
		   	if (n < 0) error("ERROR writing to socket");
		}
		
		// reading the order from customer.
		n = read(sock,buffer,255);
		if (n < 0) error("ERROR reading from socket");
		printf("Order: %s\n",buffer);

		// masege to chef not working.
		mtx.lock();
	//	MSG= "Mesage to Chef!!! from server in customer!";
		msg_to.push("Mesage to Chef!!! from server in customer!");
		mtx.unlock();
		
	
		listen(sock,5);
	}

	close(sock);
}
/***********************************************/
void MyServer::chef(int sock )
{
	cout << "new chef here!"<<endl;
	while(true)
	{
		int n;
		char buffer[256];
		string temmsg;

		bzero(buffer,256);

		// geting masege from chef not relevent !
		/* */
		//n = read(sock,buffer,255);
		//	if (n < 0) error("ERROR reading from socket");

		//printf("Message from Ghef: %s\n",buffer);

		// geting masege from the customer (not working)
		mtx.lock();
		if(!msg_to.empty())
		{
			temmsg = msg_to.front();
			msg_to.pop();
		}
		mtx.unlock();

		/* */
		// sending masege to chef from customer. or any other.
		n = write(sock,temmsg.c_str(),temmsg.length());
		   	if (n < 0) error("ERROR writing to socket");

		 listen(sock,5);
	}

	close(sock);
}
/***********************************************/
void MyServer::waiter(int sock)
{

	cout << "new waiter here!"<<endl;
	while(true)
	{
		int n;
		char buffer[256];
		string temmsg;

		bzero(buffer,256);

		mtx.lock();
		if(!msg_to.empty())
		{
			temmsg = msg_to.front();
			msg_to.pop();
		}
		mtx.unlock();

		/* */
		// sending masege to maiter from customer. or any other.
		n = write(sock,temmsg.c_str(),temmsg.length());
		   	if (n < 0) error("ERROR writing to socket");

		 listen(sock,5);
	}

	close(sock);
}

/***********************************************/
void MyServer::choosone(int sock)
{
	// the function what swithech controme betvine chef waiter and customers.
	// 
	int n;
	char buffer[256];
	bzero(buffer,256);

	n = read(sock,buffer,255);

	if (n < 0) error("ERROR reading from socket");

	else 
	if ( buffer[0] == 'C' && buffer[1] == 'h')
	{
		chef(sock);
	}
	else
	if ( buffer[0] == 'C' && buffer[1] == 'u')
	{
		customer(sock);
	}
	if ( buffer[0] == 'W' && buffer[1] == 'a')
	{
		waiter(sock);
	}
	
}

void MyServer::error(const char *msg)
{
	// just an error handaling
	perror(msg);
	exit(1);
}

