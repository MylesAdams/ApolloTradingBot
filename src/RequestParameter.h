#ifndef REQUESTPARAMETER_H
#define REQUESTPARAMETER_H

#include "cpprest/details/basic_types.h"

namespace Apollo
{
    namespace Bot
    {
        struct RequestParameter //TODO -- make this into a class with a .h and .cpp file. Literally make it exactly the same but as a class.
        {
            utility::string_t key;
            utility::string_t value;
            RequestParameter(const utility::string_t& key, const utility::string_t& value)
                : key(key), value(value)
            {
            }
            bool operator < (const RequestParameter& str) const
            {
                return (key < str.key);
            }
        };
    }
}
#endif