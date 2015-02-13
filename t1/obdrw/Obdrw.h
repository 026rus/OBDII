/* 
 * File:   Obdrw.h
 * Author: Vitaly Borodin
 *
 * Created on on Tue Jan 27 11:44:42 2015
 */

#ifndef OBDRW_H
#define	OBDRW_H
#include <iostream>

class Obdrw 
{
	private:


	public:
		// Constructors
		Obdrw();
    	Obdrw(const Obdrw& orig);
    	~Obdrw();

		// Methods
		//
		void connect();
		std::string read();
		void write ( std::string);
};

#endif	/* OBDRW_H */

