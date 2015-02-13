/* 
 * File:   Obdrw.cpp
 * Author: vitaly Borodin
 * 
 * Created on Tue Jan 27 11:44:42 2015
 */

#include "Obdrw.h"
#include <iostream>

using namespace std;

Obdrw::Obdrw()
{
    cout << "In constructor\n";
}

Obdrw::Obdrw(const Obdrw& orig)
{
    cout << "In copy constructor\n";
}

Obdrw::~Obdrw()
{
    cout << "In destructor\n";
}

void Obdrw::connect()
{

	cout << "Connecting\n";
}

string Obdrw::read()
{
	cout << "Reading\n";
	return "!";
}

void Obdrw::write(string wstr)
{
	cout <<"Writing: "<<wstr<<endl;
}
