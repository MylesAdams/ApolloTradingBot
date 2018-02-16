<<<<<<< HEAD

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

=======
/*
4chan example of using curlpp and rapidjson
*/

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
    auto& data = fc.getData();

    std::ofstream out("fourchanbot_test.txt");
    for (auto& comment : data)
        if (std::regex_search(comment.content, rgx))
            out << "ID: " << comment.ID << "\nContent:\n" << comment.content << "\n==========================\n" << std::endl;
    out.close();
>>>>>>> c3efdc27c8bd067b953be16a22ad7576f4dc7a77
    return 0;
}