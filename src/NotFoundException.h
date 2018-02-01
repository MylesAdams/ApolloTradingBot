//Written by ANDREW LAUX
//CS M20 Prof. Martin Chetlen : TopicD
//File: NotFoundException.h

//Preprocessor Directives.
#ifndef NotFoundException_h
#define NotFoundException_h
#include <cassert>
#include <stdexcept>
#include <string>

//Namespace
using std::logic_error;
using std::string;

//Exception object: NotFoundException
class NotFoundException : public logic_error
{
public:
	NotFoundException(const string& message = "") :
		logic_error("!Not Found Exception Violated: \n" + message)
	{}
};

#endif