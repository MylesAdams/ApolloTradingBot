#ifndef BADTARGETEXCEPTION_H
#define BADTARGETEXCEPTION_H

#include <stdexcept>
namespace Apollo
{
    namespace Bot
    {
        class BadTargetException : public std::runtime_error
        {
        public:
            BadTargetException()
                : runtime_error("Target url missing or invalid") {}
            BadTargetException(const std::string& msg)
                : runtime_error(msg) {}
        };
    }
}
#endif