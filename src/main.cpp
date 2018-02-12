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

int main()
{
    std::vector<std::string> complete_urls;
    std::vector<std::string> incomplete_urls;
    complete_urls.push_back("https://a.4cdn.org/biz/catalog.json");
    incomplete_urls.push_back("https://a.4cdn.org/biz");    //may or may not be necessary depending on the targeted website. This is needed in FourChan's case.
    Apollo::Bot::FourChan fc(complete_urls, incomplete_urls);
    auto data = fc.getData();
    return 0;
}