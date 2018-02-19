/*
4chan example of using curlpp and rapidjson
*/

#include "FourChan.h"
#include "Twitter.h"

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
    auto utc = utility::datetime::utc_timestamp();
    utility::string_t oauth_nonce;
    std::vector<unsigned char> seed;
    for (int i = 0; i < 32; ++i)
        seed.push_back(utc % (31 * (i % 7 + 1)));   //generates 32 bytes of pseudo random data
    oauth_nonce = utility::conversions::to_base64(seed);
    wchar_t t = 0xF8;      //example character in hex
    wchar_t e = t % 0x10;  //extract least significant digit
    t /= 0x10;  //shift right
    wchar_t f = t % 0x10;  //extract least significant digit
    f += 0x37;  // for hex letters, add 0x37 to get the utf8 capital-cased letter
    e += 0x30;  // for hex numbers, add 0x30 to get the utf8 number
    return 0;
}