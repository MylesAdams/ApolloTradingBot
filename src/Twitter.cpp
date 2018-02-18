#include "Twitter.h"
#include <algorithm>
twitCurl twitterObj;

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
extern "C" void __imp__set_output_format(void) {};


std::vector<Apollo::Comment> Apollo::Bot::Twitter::parseJSON(const rapidjson::Document & document)
{
	//getting comments
	
	//vector being returned
	std::vector<Apollo::Comment> comments;

	//pulling out "tweets" and "screen name" from json document
	for (int i = 0; i < document.Size(); ++i) 
		comments.push_back(Comment(document[i]["test"].GetString(), document[i]["id_str"].GetString()));
	
	return comments;


}

std::vector<Apollo::Comment> Apollo::Bot::Twitter::cleanComments(std::vector<Comment>& comments)
{
	//cleaning comments of unwanted characters
	std::regex rgx("[^a-zA-Z\.\?\!\' ]");

	for (auto& comment : comments)
	{
		//removing all non standard characters
		comment.content = std::regex_replace(comment.content, rgx, "");
		
		//remove begining and end whitewspace
		comment.content = this->trim(comment.content);

		//to lower ccase
		std::transform(comment.content.begin(), comment.content.end(), comment.content.begin(), ::tolower);
	}
	return comments;

}

std::stringstream requestResponse(const std::string& target_url)
{
	//building json

	std::stringstream ss;
	std::string replyMsg = "";
	
	if (twitterObj.timelineUserGet(false, false, 50, target_url, false))
	{
		twitterObj.getLastWebResponse(replyMsg);
		ss << replyMsg.c_str();
	}
	else
	{
		twitterObj.getLastCurlError(replyMsg);
		printf("\ntwitterClient:: twitCurl::timelineUserGet error:\n%s\n", replyMsg.c_str());
	}

	return ss;


}

Apollo::Bot::Twitter::Twitter()
{

	// which page will be getting fetched from twitter
	this->COMPLETE_URLS_.push_back("vechainofficial");

	// Saving keys
	consumerKey = "28JDbKeafaAMvHACv6Z9Y1Wlq";
	consumerKeySecret = "y2EISPTqHxSOqj7Lz7Rffg0QprOpzbqjS6e3udOL7rQMt0Izv5";
	myOAuthAccessTokenKey = "955967890301190144-MJuVJqzRCg4Jgr4wxFb0ZzWwAfQU20Y";
	myOAuthAccessTokenSecret = "7XB8lfEtdN7z1faK45nFvDItHMUWoyk4weVoNtoDD9GVz";

	// Seting keys to the twitter object
	twitterObj.getOAuth().setConsumerKey(consumerKey);
	twitterObj.getOAuth().setConsumerSecret(consumerKeySecret);
	twitterObj.getOAuth().setOAuthTokenKey(myOAuthAccessTokenKey);
	twitterObj.getOAuth().setOAuthTokenSecret(myOAuthAccessTokenSecret);

}

Apollo::Bot::Twitter::~Twitter()
{

}