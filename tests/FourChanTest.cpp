#include <FourChan.h>
#include <Bot.h>

#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include <string>

using Apollo::Bot::FourChan;
using namespace web::http;
using utility::string_t;




TEST_CASE("ForChan tests") {
	FourChan fc;

	SECTION("gets assigned, and converts to lowercase") {
		fc.setSearchQuery(" adam Is SUpER CooL yo 345 Q3lk45J2q345jl-  ");
		std::string pattern = fc.getsearch_pattern_();
		REQUIRE(pattern == " adam is super cool yo 345 q3lk45j2q345jl-  ");
	}

	SECTION("gets rid of whitespace") {
		std::vector<Apollo::Comment> comments;
		comments.push_back(Apollo::Comment("adam", "69"));
		comments.push_back(Apollo::Comment(" the meaning of life    ", "42"));
		comments.push_back(Apollo::Comment("highway though sb ", "101"));
		comments.push_back(Apollo::Comment(" waiting in line at the dmv is like being in", "666"));
		comments = fc.getcleanComments(comments);

		REQUIRE(comments[0].content == "adam");
		REQUIRE(comments[1].content == "the meaning of life");
		REQUIRE(comments[2].content == "highway though sb");
		REQUIRE(comments[3].content == "waiting in line at the dmv is like being in");
	}

	SECTION("parse JSON") {

		// example of a json from 4chan
		std::string json = "[{\"page\": 1,\"threads\" : [	{	\"no\": 904256,	\"last_modified\" : 1443114714},  { \"no\": 4884770,  \"last_modified\" : 1512740123  },  {  \"no\": 8251047,  \"last_modified\" : 1520710289  }	]},  { \"page\": 2,  \"threads\" : [{	\"no\": 8251932,	\"last_modified\" : 1520710287	},	  {	  \"no\": 8249041,  \"last_modified\" : 1520710284  },	  {		  \"no\": 8251212,		  \"last_modified\" : 1520710283	  },	  {		  \"no\": 8251581,		  \"last_modified\" : 1520710133	  }	  ]  },	{		\"page\": 11,		\"threads\" : [					{						\"no\": 8249849,							\"last_modified\" : 1520706108					},	  {		  \"no\": 8249585,		  \"last_modified\" : 1520706084	  }		]	}]";
		
		rapidjson::Document document;
		document.Parse(json);
		std::vector<Apollo::Comment> testParse =fc.JSONparse(document);
		std::string placeHolder = "placeHolder";
		
		// no new posts
		if(testParse.size() == 0)
			REQUIRE(placeHolder  == "placeHolder");

		// new posts
		else
		REQUIRE(testParse[0].content != "placeHolder");
	}
}

