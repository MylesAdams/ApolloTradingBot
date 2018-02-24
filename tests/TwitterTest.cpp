#include "Twitter.h"

using Apollo::Bot::Twitter;
using namespace web::http;
using utility::string_t;

class TwitterTester : public Twitter
{
protected:
    virtual void saveSettings() override {}
    uint64_t getTime() override { return 1318622958; }
    string_t generateNonce(uint64_t utc) override { return U("kYjzVBB8Y0ZFabxSWbWovY3uYSQ2pTgmZeNu2VS4cg"); }
    void checkSig(const utility::string_t& sig) override { assert(sig == U("hCtSmYh+iHYCEqBWrE7C7hYmtUk=")); } //expected signature, per the Twitter API's example
public:
    virtual ~TwitterTester() {};
    TwitterTester()
    {
        Apollo::Bot::ScraperTarget test_target(U("https://api.twitter.com"), U("/1.1/statuses/update.json"), methods::POST);
        using Apollo::Bot::RequestParameter;
        test_target.request_parameters.push_back(RequestParameter(U("include_entities"), U("true")));
        test_target.request_parameters.push_back(RequestParameter(U("status"), U("Hello Ladies + Gentlemen, a signed OAuth request!")));
        this->consumer_key_ = "xvz1evFS4wEEPTGEFPHBog";
        this->consumer_secret_ = "kAcSOqF21Fu85e7zjz7ZN2U4ZRhfV3WpwPAoE3Z7kBw";
        this->oauth_access_token_key_ = "370773112-GmHxMAgYyLbNEtIKZeRNFsMKPR9EyMZeS9weJAEb";
        this->oauth_access_token_secret_ = "LswwdoUaIvS8ltyTt5jkRh4J50vUPVVHtR2YPi5kE";
        this->target_ = test_target;
    }
    void test()
    {
        try
        {
            this->getData();
        }
        catch (...)
        {
            return; //always throws bad status, since the nonce and timestamp are improper for a real request
        }
    }
};

int main()
{
    TwitterTester t;
    t.test();
    return 0;
}