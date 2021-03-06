#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "Record.h"

/* Utility functions, constants, and classes used by other modules */

// a simple class for error exceptions - msg points to a C-string error message
struct Error {
	Error(const char * in_msg = "") :
		msg(in_msg)
		{}
	const char * msg;
};
// define a function template named "swapem" that interchanges the values of two variables
// use in Ordered_list and String where convenient
template <typename T>
void swapem(T & a, T & b) {
    T temp(a);
    a = b;
    b = temp;
    return;
}
//Read title from stdin, throw Error if fail
void read_title_stdin(std::string & title);
//Read title from file, throw Error if fail
void read_title_file(std::string & title, std::istream & is);
//Read integer from stdin, throw Error if fail
void read_integer(int& id, std::istream & is);
//Trim String for whitespaces
void trim_string(std::string & str);
#endif
