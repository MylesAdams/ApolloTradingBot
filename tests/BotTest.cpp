#include <Bot.h>

#define CATCH_CONFIG_MAIN 
#include "catch.hpp"

namespace Apollo
{
	namespace Bot
	{
		class BotTester : public Bot {
		public:
			std::vector<Comment> temp;
			uint64_t temp2;
			virtual void saveSettings() override{}
			virtual std::string requestResponse(const ScraperTarget& target) override { return "foo"; }
			virtual std::vector<Comment> parseJSON(const rapidjson::Document& document) override { return temp; }   // implementation is specific to derived class as the DOM varies from site to site.
			virtual std::vector<Comment> cleanComments(std::vector<Comment>& comments) override { return temp; }
			virtual void setSearchQuery(const std::string& query) override { }
			virtual std::vector<Comment> getData() override { return temp; }

			virtual uint64_t getTime() override { return temp2; }

			bool compare(std::string a, std::string b) {
				return compareBigNumbers(a, b);
			}

			std::string trimWord(const std::string& str) {
				return trim(str);
			}
			BotTester() {}

			utility::string_t stripBase(const utility::string_t& s) {
				return stripBase64(s);
			}

			virtual ~BotTester() {}
		};
	}
}

TEST_CASE("Bot tests") {
	Apollo::Bot::BotTester bt;
	
	SECTION("compare") {
		REQUIRE_FALSE(bt.compare("1", "2"));
		REQUIRE(bt.compare("2", "1"));	
	}

	SECTION("trimWord") {
		REQUIRE(bt.trimWord("nothing to trim") == "nothing to trim");
		REQUIRE(bt.trimWord(" start to trim") == "start to trim");
		REQUIRE(bt.trimWord("end to trim ") == "end to trim");
		REQUIRE(bt.trimWord(" both sides to trim ") == "both sides to trim");
	}

	SECTION("stripBase64") {
		REQUIRE(utility::conversions::to_utf8string(bt.stripBase(U("@ # $ % ^ &a* -+= |d \\ { }[] :a;\"m' <r>u,() ` ~!l.e?s/"))) == "adamrules");
	}
}