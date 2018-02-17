/*
4chan example of using curlpp and rapidjson
*/

#include <boost/python.hpp>

#include "FourChan.h"

#include <iostream>
#include <sstream>
#include <cctype>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using Apollo::Comment;

int main()
{
    std::regex rgx("(\\bven\\b)|(\\bvechain\\b)");
    std::cout << "This is an example of using the FourChan bot.\n" << std::endl;
    Apollo::Bot::FourChan fc;
    const auto& data = fc.getData();

    std::ofstream out("fourchanbot_test.txt");
    for (auto& comment : data)
        if (std::regex_search(comment.content, rgx))
            out << "ID: " << comment.ID << "\nContent:\n" << comment.content << "\n==========================\n" << std::endl;
    out.close();
    return 0;
}