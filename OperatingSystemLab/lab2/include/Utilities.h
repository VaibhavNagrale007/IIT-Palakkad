#ifndef UTILITIES_H
#define UTILITIES_H
 
#include <iostream>
#include <stdio.h>
#include <ctime>

 
namespace Util
{
    // Get current date/time, format is YYYY-MM-DD.HH:mm:ss
    const std::string CurrentDateTime()
    {
        time_t curr_time;
	    tm * curr_tm;
        char tr[100];
	
	    time(&curr_time);
	    curr_tm = localtime(&curr_time);

        strftime(tr, 50, "%F.%T", curr_tm);
        return tr;
    }
}
#endif
