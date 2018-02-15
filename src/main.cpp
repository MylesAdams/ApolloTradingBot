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
    std::vector<std::string> complete_urls;
    std::vector<std::string> incomplete_urls;
    complete_urls.push_back("https://a.4cdn.org/biz/threads.json");
    incomplete_urls.push_back("https://a.4cdn.org/biz");    //may or may not be necessary depending on the targeted website. This is needed in FourChan's case.
    Apollo::Bot::FourChan fc(complete_urls, incomplete_urls);
    auto& data = fc.getData();
    for (auto& comment : data)
        std::cout << "ID: " << comment.ID << "\nContent:\n" << comment.content << "\n==================================\n" << std::endl;
    return 0;
}