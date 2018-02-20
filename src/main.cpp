


/*
4chan example of using curlpp and rapidjson
*/

#include "FourChan.h"
#include "Reddit.h"
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
    //std::regex rgx("(\\bven\\b)|(\\bvechain\\b)");
    std::cout << "This is an example of using the FourChan bot.\n" << std::endl;
    Apollo::Bot::Reddit rBot;
    const auto& data = rBot.getData();

    //std::ofstream out("fourchanbot_test.txt");
    for (auto& comment : data)
            std::cout << "ID: " << comment.ID << "\nContent:\n" << comment.content << "\n==========================\n" << std::endl;
    //out.close();
    return 0;
}