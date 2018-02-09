
#include <string>
#include <sstream>
#include <iostream>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


int main()
{
    std::string redditURL = "https://www.reddit.com/r/vechain/";
    try
    {
        curlpp::Cleanup myCleanup;

        curlpp::options::Url myUrl(redditURL + "about.json");
        curlpp::Easy myRequest;
        myRequest.setOpt(myUrl);


        myRequest.perform();


        std::stringstream jsonOutput;
        curlpp::options::WriteStream ws(&jsonOutput);
        rapidjson::Document doc;

        myRequest.setOpt(ws);
        myRequest.perform();

        doc.Parse(jsonOutput.str().c_str());

        jsonOutput << doc["subscribers"].GetInt();

        }

    catch(curlpp::RuntimeError &re)
    {
        std::cout << re.what() << std::endl;
    }

    catch(curlpp::LogicError &le)
    {
        std::cout << le.what() << std::endl;
    }

    return 0;
}