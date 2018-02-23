//
// Created by Roderic Deichler on 2/19/18.
//

#ifndef APOLLOTRADINGBOT_OAUTHLISTENER_H
#define APOLLOTRADINGBOT_OAUTHLISTENER_H

#include "cpprest/http_listener.h"
#include "cpprest/http_client.h"

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace web::http::oauth2::experimental;
using namespace web::http::experimental::listener;

class oauth2_code_listener {
public:
    oauth2_code_listener(
            uri listen_uri,
            oauth2_config &config) :
            m_listener(new http_listener(listen_uri)),
            m_config(config) {
        m_listener->support([this](http::http_request request) -> void {
            if (request.request_uri().path() == U("/") && request.request_uri().query() != U("")) {
                m_resplock.lock();

                m_config.token_from_redirected_uri(request.request_uri()).then(
                        [this, request](pplx::task<void> token_task) -> void {
                            try {
                                token_task.wait();
                                m_tce.set(true);
                            }
                            catch (const oauth2_exception &e) {
                                ucout << "Error: " << e.what() << std::endl;
                                m_tce.set(false);
                            }
                        });

                request.reply(status_codes::OK, U("Ok."));

                m_resplock.unlock();
            } else {
                request.reply(status_codes::NotFound, U("Not found."));
            }
        });

        m_listener->open().wait();
    }

    ~oauth2_code_listener() {
        m_listener->close().wait();
    }

    pplx::task<bool> listen_for_code() {
        return pplx::create_task(m_tce);
    }

private:
    std::unique_ptr<http_listener> m_listener;
    pplx::task_completion_event<bool> m_tce;
    oauth2_config &m_config;
    std::mutex m_resplock;
};

#endif //APOLLOTRADINGBOT_OAUTHLISTENER_H
