//
// Created by Roderic Deichler on 2/19/18.
//

#ifndef APOLLOTRADINGBOT_OAUTHSESSION_H
#define APOLLOTRADINGBOT_OAUTHSESSION_H

#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"
#include "OauthListener.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

class OauthSession
{
public:
    OauthSession(utility::string_t name,
                          utility::string_t client_key,
                          utility::string_t client_secret,
                          utility::string_t auth_endpoint,
                          utility::string_t token_endpoint,
                          utility::string_t redirect_uri) :
            m_oauth2_config(client_key,
                            client_secret,
                            auth_endpoint,
                            token_endpoint,
                            redirect_uri),
            m_name(name)
            //m_listener(new oauth2_code_listener(redirect_uri, m_oauth2_config))
    {}

    //
// Utility method to open browser on Windows, OS X and Linux systems.
//
    static void open_browser(utility::string_t auth_uri)
    {
        #if defined(_WIN32) && !defined(__cplusplus_winrt)
            // NOTE: Windows desktop only.
            auto r = ShellExecuteA(NULL, "open", conversions::utf16_to_utf8(auth_uri).c_str(), NULL, NULL, SW_SHOWNORMAL);
        #elif defined(__APPLE__)
            // NOTE: OS X only.
        string_t browser_cmd(U("open \"") + auth_uri + U("\""));
        (void)system(browser_cmd.c_str());
        #else
            // NOTE: Linux/X11 only.
            string_t browser_cmd(U("xdg-open \"") + auth_uri + U("\""));
            (void)system(browser_cmd.c_str());
        #endif
    }

    void run()
    {
        if (is_enabled())
        {
            ucout << "Running " << m_name.c_str() << " session..." << std::endl;

            if (!m_oauth2_config.token().is_valid_access_token())
            {
                if (authorization_code_flow().get())
                {
                    m_http_config.set_oauth2(m_oauth2_config);
                }
                else
                {
                    ucout << "Authorization failed for " << m_name.c_str() << "." << std::endl;
                }
            }

            run_internal();
        }
        else
        {
            ucout << "Skipped " << m_name.c_str() << " session sample because app key or secret is empty. Please see instructions." << std::endl;
        }
    }

protected:
    virtual void run_internal() = 0;

    pplx::task<bool> authorization_code_flow()
    {
        open_browser_auth();
        return m_listener->listen_for_code();
    }

    http_client_config m_http_config;
    oauth2_config m_oauth2_config;

private:
    bool is_enabled() const
    {
        return !m_oauth2_config.client_key().empty() && !m_oauth2_config.client_secret().empty();
    }

    void open_browser_auth()
    {
        auto auth_uri(m_oauth2_config.build_authorization_uri(true));
        ucout << "Opening browser in URI:" << std::endl;
        ucout << auth_uri << std::endl;
        open_browser(auth_uri);
    }

    utility::string_t m_name;
    std::unique_ptr<oauth2_code_listener> m_listener;
};
#endif //APOLLOTRADINGBOT_OAUTHSESSION_H
