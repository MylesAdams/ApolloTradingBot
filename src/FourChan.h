#ifndef FOURCHAN_H
#define FOURCHAN_H

#include "Bot.h"

namespace Apollo
{
    namespace Bot
    {
        class FourChan : public Bot
        {
        private:
            //fields
            const std::string RESOURCE_FILE_ = "../resources/fourchan.txt";
            const utility::string_t BASE_URL_ = U("https://a.4cdn.org");

            //methods
            virtual void saveSettings() override;
        protected:
            //fields
            unsigned long long int highest_post_seen_;
            unsigned long long int highest_timestamp_seen_;
            std::string search_pattern_;

            //methods
            virtual std::string requestResponse(const ScraperTarget& target) override;
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
        public:
			/*
			these are made available for testing
			*/
			std::string getsearch_pattern_() {
				return search_pattern_;
			}
			std::vector<Apollo::Comment> getcleanComments(std::vector<Comment>& comments) {
				return cleanComments(comments);
			}

			std::vector<Apollo::Comment> JSONparse(const rapidjson::Document& document) {
				return parseJSON(document);
			}

            //ctor
            /*
            @author: Gavin Frazar
            Parameters: none - default constructor
            Return values: none - default constructor
            pre-conditions: none
            post-conditions: FourChan object constructed, targetting the /biz/ board of 4chan
            Exceptions: None. This method never throws.
            */
            FourChan();

            /*
            @author: Gavin Frazar
            Parameters: none - destructor
            Return values: none - destructor
            pre-conditions: none
            post-conditions: Creates or modifies the fourchan.txt resource file in ApolloTradingBot/resources/fourchan.txt to hold two integers separated by a space
            Exceptions: None. This method never throws.
            */
            virtual ~FourChan();

            //methods
            /*
            @author: Gavin Frazar
            Parameters: query - string containing the query a user would like to search the 4chan/biz/ catalog for.
            Return values: void
            pre-conditions: none
            post-conditions: FourChan object's internal search_pattern_ member will be set to the query specified.
            Exceptions: None. This method never throws.
            */
            virtual void setSearchQuery(const std::string& query) override;
        };
    }// end of Bot namespace
}// end of Apollo namespace
#endif
