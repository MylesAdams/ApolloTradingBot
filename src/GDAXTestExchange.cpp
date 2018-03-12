//
// Created by Myles Adams on 3/8/18.
//

#include "GDAXTestExchange.h"

#include <string>

Apollo::Exchanges::GDAXTestExchange::GDAXTestExchange(std::string trading_currency) :
	amount_btc_(0.0), amount_cur_(0.0), trading_currency_ticker_(trading_currency)
{}

Apollo::Exchanges::GDAXTestExchange::GDAXTestExchange(std::string trading_currency, double amount_btc, double amount_cur) :
	amount_btc_(amount_btc), amount_cur_(amount_cur), trading_currency_ticker_(trading_currency)
{}


void Apollo::Exchanges::GDAXTestExchange::setCurrency(std::string trading_currency)
{
	this->trading_currency_ticker_ = trading_currency;
}


void Apollo::Exchanges::GDAXTestExchange::setAmountBTC(double amount)
{
	this->amount_btc_ = amount;
}

void Apollo::Exchanges::GDAXTestExchange::setAmountCurrency(double amount)
{
	this->amount_cur_ = amount;
}

// price is (amt btc / 1 cur)
void Apollo::Exchanges::GDAXTestExchange::buyCurrency(double amount_percent, double price)
{
	if (this->amount_btc_ <= 0.0)
		return;

	if (amount_percent <= 0 || amount_percent > 1)
		return;

	double amount_in_btc = this->amount_btc_ * amount_percent;
	double amount_in_cur = amount_in_btc / price;

	this->amount_btc_ -= amount_in_btc;
	this->amount_cur_ += amount_in_cur;

}

// price is (amt btc / 1 cur)
void Apollo::Exchanges::GDAXTestExchange::sellCurrency(double amount_percent, double price)
{
	if (this->amount_cur_ <= 0.0)
		return;

	if (amount_percent <= 0 || amount_percent > 1)
		return;

	double amount_in_cur = this->amount_cur_ * amount_percent;
	double amount_in_btc = amount_in_cur * price;

	this->amount_btc_ += amount_in_btc;
	this->amount_cur_ -= amount_in_cur;
}

bool Apollo::Exchanges::GDAXTestExchange::hasCredentials()
{
	return false;
}

void Apollo::Exchanges::GDAXTestExchange::update()
{
	return;
}

void Apollo::Exchanges::GDAXTestExchange::connect()
{
	return;
}

void Apollo::Exchanges::GDAXTestExchange::clearCredentials()
{
	return;
}

void Apollo::Exchanges::GDAXTestExchange::setCredentials(utility::string_t key, utility::string_t secret)
{
	return;
}

void Apollo::Exchanges::GDAXTestExchange::setCredentials(utility::string_t key, utility::string_t secret, utility::string_t passphrase)
{
	return;
}

bool Apollo::Exchanges::GDAXTestExchange::buyAtMarket(utility::string_t coin_id, utility::string_t amount)
{
    return false;
}

bool Apollo::Exchanges::GDAXTestExchange::buyAtLimit(utility::string_t coin_id, utility::string_t amount, utility::string_t price)
{
    return false;
}
