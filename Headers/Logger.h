#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Logger
{
public:
	template<typename T>
	static void log(T s)
	{
		ofstream file;
		file.open("log.txt",ios::in | ios::app);
		file << s << '\n';
	}

};


#endif
