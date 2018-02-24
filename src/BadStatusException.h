#ifndef BADSTATUSEXCEPTION_H
#define BADSTATUSEXCEPTION_H

#include <stdexcept>
namespace Apollo
{
    namespace Bot
    {
        class BadStatusException : public std::runtime_error
        {
        public:
            BadStatusException()
                : runtime_error("Status code from server was not OK") {}
            BadStatusException(const std::string& msg)
                : runtime_error(msg) {}
        };
    }
}
#endif