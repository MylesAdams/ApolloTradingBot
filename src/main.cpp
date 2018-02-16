

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "twitcurl.h"
#include "oauthlib.h"
#include "prettywriter.h" // for stringify JSON

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;
using namespace rapidjson;

FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
extern "C" void __imp__set_output_format(void) {};



int main() {
	ofstream myfile;
	string tmpStr;
	string replyMsg;
	char tmpBuf[1024];
	twitCurl twitterObj;

	string consumerKey = "28JDbKeafaAMvHACv6Z9Y1Wlq";
	string consumerKeySecret = "y2EISPTqHxSOqj7Lz7Rffg0QprOpzbqjS6e3udOL7rQMt0Izv5";
	string myOAuthAccessTokenKey = "955967890301190144-MJuVJqzRCg4Jgr4wxFb0ZzWwAfQU20Y";
	string myOAuthAccessTokenSecret = "7XB8lfEtdN7z1faK45nFvDItHMUWoyk4weVoNtoDD9GVz";

	// Seting keys
	twitterObj.getOAuth().setConsumerKey(consumerKey);
	twitterObj.getOAuth().setConsumerSecret(consumerKeySecret);
	twitterObj.getOAuth().setOAuthTokenKey(myOAuthAccessTokenKey);
	twitterObj.getOAuth().setOAuthTokenSecret(myOAuthAccessTokenSecret);





	myfile.open("example.json");
	Document d;


	/* Get user timeline */
	replyMsg = "";
	printf("\nGetting user timeline\n");
	if (twitterObj.timelineUserGet(true, false, 1, "vechainofficial", false))
	{
		twitterObj.getLastWebResponse(replyMsg);
		//printf("\ntwitterClient:: twitCurl::timelineUserGet web response:\n%s\n", replyMsg.c_str());
		myfile << replyMsg.c_str();
		d.Parse(replyMsg.c_str());
	}
	else
	{
		twitterObj.getLastCurlError(replyMsg);
		printf("\ntwitterClient:: twitCurl::timelineUserGet error:\n%s\n", replyMsg.c_str());
	}
	myfile.close();
	//printf("text = %s\n", d["text"].GetString());
	//string comment = d["text"].GetString();

	//cout << comment << endl;

	system("pause");




	//// Post a new status message 
	//tmpStr = "Testing Twitter API 2.0";
	//if (twitterObj.statusUpdate(tmpStr))
	//{
	//	twitterObj.getLastWebResponse(replyMsg);
	//	printf("\ntwitterClient:: twitCurl::statusUpdate web response:\n%s\n", replyMsg.c_str());
	//}
	//else
	//{
	//	twitterObj.getLastCurlError(replyMsg);
	//	printf("\ntwitterClient:: twitCurl::statusUpdate error:\n%s\n", replyMsg.c_str());
	//}





	cout << "hi" << endl;
	system("pause");
	return 0;
}

