/*
4chan example of using curlpp and rapidjson
*/

#include "FourChan.h"


#include <iostream>
#include <sstream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using Apollo::Comment;

int main()
{
    std::cout << "This is an example of using the FourChan bot.\n" << std::endl;
    Apollo::Bot::FourChan fc;
    auto& data = fc.getData();
    for (auto& comment : data)
        std::cout << "ID: " << comment.ID << "\nContent:\n" << comment.content << "\n==================================\n" << std::endl;
    return 0;
}