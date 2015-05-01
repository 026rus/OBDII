/* 
 *
 * File:   SERVER/main.cpp
 * Author: Vitaly Borodin
 *
 */

#include "MyServer.h"

using namespace std;

/*
 * 
 */

int main(int argc, char** argv)
{ 
	MyServer s;
	s.start(argc, argv);
	return 0;
}
