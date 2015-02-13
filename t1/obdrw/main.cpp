/* 
 * File:   main.cpp
 * Author: Vitaly
 *
 * Created on Tue Jan 27 11:44:42 2015
 *
 */

#include <iostream>
#include <cstdlib>
#include "Obdrw.h"

using namespace std;

/*
 * 
 */

int main(int argc, char** argv)
{
	string astr="Q";
	Obdrw a1;
	a1.connect();
	astr = a1.read();
	cout << astr << endl;
	a1.write("Vitaly");
	Obdrw a2(a1);
	return 0;
}
