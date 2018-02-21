/*
4chan example of using curlpp and rapidjson
*/

#include "FourChan.h"
#include "Twitter.h"

#include <iostream>
#include <sstream>
#include <cctype>



#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using Apollo::Comment;

int main()
{
    Apollo::Bot::Twitter t;
    t.getData();
    return 0;
}