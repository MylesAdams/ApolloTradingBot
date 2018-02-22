//
// Created by Roderic Deichler on 2/20/18.
//

#if defined(_WIN32) && !defined(__cplusplus_winrt)
// Extra includes for Windows desktop.
#include <windows.h>
#include <Shellapi.h>
#endif

#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include "RedditOauth.h"
#include <iostream>
#include <sys/stat.h>
#include <string>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;



inline bool exist(const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}



#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
#else
int main(int argc, char *argv[])
#endif
{
    RedditOauth reddit(U("VeChain"));


    reddit.readSubscriberCount();

    ucout << std::endl;
    ucout << std::endl;

    reddit.readComments();

    return 0;
}