#ifndef BADSTATUSEXCEPTION_H
#define BADSTATUSEXCEPTION_H

#include <stdexcept>
#include <string>
namespace Apollo
{
    namespace Bot
    {
        class BadStatusException : public std::runtime_error
        {
        protected:
            std::string status;
        public:
            BadStatusException()
                : runtime_error("Status code from server was not OK") {}
            BadStatusException(const std::string& msg, const std::string& status)
                : runtime_error(msg)
            {
                this->status = status;
            }
            std::string status_code() const { return this->status; }
        };
    }
}
#endif