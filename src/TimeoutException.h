//
// Created by Myles Adams on 3/10/18.
//

#ifndef APOLLOTRADINGBOT_TIMEOUTEXCEPTION_H
#define APOLLOTRADINGBOT_TIMEOUTEXCEPTION_H

#include <stdexcept>
#include <string>
namespace Apollo
{
    namespace Bot
    {
        class TimeoutException : public std::runtime_error
        {
        public:
            TimeoutException()
                    : runtime_error("Request has timed out.") {}
            TimeoutException(const std::string& msg)
                    : runtime_error(msg)
            {}
        };
    }
}

#endif //APOLLOTRADINGBOT_TIMEOUTEXCEPTION_H
