#ifndef TWITTER_H
#define TWITTER_H


#include "twitcurl.h"
#include "oauthlib.h"
//#include "prettywriter.h" // for stringify JSON


#include <fstream>
#include <regex>
#include <string>

#include "Bot.h"




namespace Apollo
{
	namespace Bot
	{
		class Twitter : public Bot
		{

		private:
			std::string consumerKey;
			std::string consumerKeySecret;
			std::string myOAuthAccessTokenKey;
			std::string myOAuthAccessTokenSecret;
			

		protected:

			std::stringstream requestResponse(const std::string& target_url) override;
			//methods
			std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
			std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;

		public:
			twitCurl twitterObj;
			Twitter();
			virtual ~Twitter();
		};
	}
}
#endif