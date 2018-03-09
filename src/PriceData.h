#ifndef PRICEDATA_H
#define PRICEDATA_H

#include "Bot.h"
#include "PriceDataTarget.h"
#include "Comment.h"

namespace Apollo
{
    namespace Bot
    {
        class PriceData : public Bot
        {
        public:
            PriceData();
            virtual ~PriceData();

        private:
            const utility::string_t RESOUCE_URL_ = U("https://min-api.cryptocompare.com");
            const utility::string_t DEFAULT_INTERVAL_REQUEST_PATH_ = U("/data/histominute");
            const utility::string_t DEFAULT_TICKER_FROM_ = U("BTC");
            const utility::string_t DEFAULT_TICKER_TO_ = U("USD");
            const int DEFAULT_TIME_IN_HOURS_ = 12;
            const int TIME_WEEK_ = 604800;


            Apollo::Bot::PriceDataTarget price_data_target_;

        public:
            std::string full_request_path_interval_;
            std::string full_request_path_instant_;
            virtual void saveSettings() override;
            virtual std::string requestResponse(const ScraperTarget& target) override;
            virtual std::vector<Apollo::Comment> parseJSON(const rapidjson::Document& document) override;
            virtual std::vector<Apollo::Comment> cleanComments(std::vector<Comment>& comments) override;
            virtual uint64_t getTime() override;
            virtual std::vector<Apollo::Comment> getData() override;
            virtual void setSearchQuery(const std::string & query) override;
            void updateInstantPriceRequestPath(std::string ticker);
            void setTickerFrom(utility::string_t ticker_from, bool update_uri);
            void setTickerTo(utility::string_t ticker_to, bool update_uri);
            void setAveragePeriod(int time_in_hours, bool update_uri);
            void setupRequest(utility::string_t ticker_from, utility::string_t ticker_to, int time_in_hours);
            void updateFullRequestPathInterval();
            std::string requestIntervalPriceData();
            std::string requestLastPrice();
            double getLastPrice();
            double getIntervalAverage();


        };


    }

}

#endif //PRICEDATA_H
