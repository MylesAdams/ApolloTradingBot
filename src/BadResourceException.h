#ifndef BADRESOURCEEXCEPTION_H
#define BADRESOURCEEXCEPTION_H

#include <stdexcept>
namespace Apollo
{
    namespace Bot
    {
        class BadResourceException : public std::runtime_error
        {
        public:
            BadResourceException()
                : runtime_error("Resource file missing or incorrectly formatted") {}
            BadResourceException(const std::string& msg)
                : runtime_error(msg) {}
        };
    }
}
#endif