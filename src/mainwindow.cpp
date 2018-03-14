#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Twitter.h"
#include "Watson.h"
#include "Comment.h"
#include "coin.h"
#include "TradeDialog.h"

#include <QTimer>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>

const int TIME_WEEK = 604800;
const int TIME_HOUR = 3600;
const int TEN_MIN = 600;
const int NUMBER_OF_PLOT = 61;
const int SENTIMENT_GRAPH = 0;
const int INSTANT_PRICE_GRAPH = 0;
const int AVG_PRICE_GRAPH = 1;
const double WATSON_LOWEST_VAL = .5;
const int NORMALIZE_VAL = 2;
utility::string_t WATSON_USERNAME = U("c032fda0-5c02-490d-8e00-ab00de2e5f40");
utility::string_t WATSON_PASSWORD = U("AfgP2LQIDCgB");

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupWidgets();

    twitter_bot = new Apollo::Bot::Twitter();
    reddit_bot = new Apollo::Bot::Reddit();
    fourchan_bot = new Apollo::Bot::FourChan();
    watson_analyzer = new Apollo::Watson(WATSON_USERNAME, WATSON_PASSWORD);
    gdax_accnt = new Apollo::Exchanges::GdaxAccnt();
    kucoin_accnt = new Apollo::Exchanges::KucoinAccnt();

    counter = 19;
    have_gdax_wallet = false;
    have_kucoin_wallet = false;
    max_price_seen_ = 0;
    min_price_seen_ = 10000000;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearData()
{
    qx_sent.clear();
    qy_pos.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(SENTIMENT_GRAPH)->setData(qx_sent, qy_pos);
    ui->trading_plot->graph(INSTANT_PRICE_GRAPH)->setData(qx_fin, qy_inst);
    ui->trading_plot->graph(AVG_PRICE_GRAPH)->setData(qx_fin, qy_avg);

    ui->plot->replot();
    ui->plot->update();
    ui->trading_plot->replot();
    ui->trading_plot->update();
}

void MainWindow::updatePlot()
{
    clearData();
    std::string filepath = "../resources/" + current_ticker_ + ".json";
    std::replace(filepath.begin(), filepath.end(), ' ', '_');
    std::ifstream json_file;
    json_file.open(filepath);

    if (json_file.is_open())
    {
        rapidjson::IStreamWrapper isw(json_file);
        rapidjson::Document doc;
        doc.ParseStream(isw);

        int upper = doc.Size();
        int lower;
        if (upper > 60)
           lower = upper - NUMBER_OF_PLOT;
        else
            lower = 0;

        for (int ndx = lower; ndx < upper; ndx++)
        {
            qx_sent.append(doc[ndx]["Time"].GetDouble());

            qy_pos.append(doc[ndx]["PosRating"].GetDouble());
//            std::cout << std::to_string(doc[ndx]["Time"].GetDouble()) << " | " << std::to_string(doc[ndx]["PosRating"].GetDouble()) << " | " <<  std::to_string(doc[ndx]["NegRating"].GetDouble()) << std::endl;
        }

        json_file.close();
        normalizeGraphs();
        ui->plot->xAxis->setRange(qx_sent[qx_sent.size() - 1] - 6*TEN_MIN, qx_sent[qx_sent.size() - 1] + 10);
        plot();
    }
}

void MainWindow::updateData()
{
    trading_bot->updateAndTrade();
    qx_fin.push_back(trading_bot->current_time_);
    qy_avg.push_back(trading_bot->current_interval_average_);
    qy_inst.push_back(trading_bot->last_trading_price_);

    if (trading_bot->last_trading_price_ > max_price_seen_)
        max_price_seen_ = trading_bot->last_trading_price_;

    if (trading_bot->last_trading_price_ < min_price_seen_)
        min_price_seen_ = trading_bot->last_trading_price_;

//    int diff = max_price_seen_ - min_price_seen_;
    double low = (min_price_seen_ < trading_bot->current_interval_average_) ? min_price_seen_ : trading_bot->current_interval_average_;
    double high = (max_price_seen_ > trading_bot->current_interval_average_) ? max_price_seen_ : trading_bot->current_interval_average_;

    ui->trading_plot->yAxis->setRange(low * .995, high * 1.005);

    if (this->counter == 19)
    {
        std::string filename = "../resources/" + current_ticker_ + ".json";
        std::replace(filename.begin(), filename.end(), ' ', '_');

        std::cout << filename << std::endl;
        auto t_comments = twitter_bot->getData();
        std::cout << "Twitter Size: " << t_comments.size() << std::endl;
        auto f_comments = fourchan_bot->getData();
        std::cout << "4Chan Size: " << f_comments.size() << std::endl;
        reddit_bot->readComments();
//        std::vector<std::string> r_comments;
        auto r_comments = reddit_bot->getComments();
        std::cout << "Reddit Size: " << r_comments.size() << std::endl;

        utility::string_t tone_input = commentsToString(t_comments, f_comments, r_comments);
        watson_analyzer->toneToFile(tone_input, utility::conversions::to_string_t(filename));
        counter = -1;
        updatePlot();
    }
    else
        plot();

    this->counter++;
}

void MainWindow::setupWidgets()
{
    ui->plot->addGraph();
    ui->trading_plot->addGraph();
    ui->trading_plot->addGraph();

    ui->plot->graph(SENTIMENT_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->trading_plot->graph(INSTANT_PRICE_GRAPH)->setScatterStyle(QCPScatterStyle::ssNone);
    ui->trading_plot->graph(AVG_PRICE_GRAPH)->setScatterStyle(QCPScatterStyle::ssNone);

    ui->plot->graph(SENTIMENT_GRAPH)->setLineStyle(QCPGraph::lsLine);
    ui->trading_plot->graph(INSTANT_PRICE_GRAPH)->setLineStyle(QCPGraph::lsLine);
    ui->trading_plot->graph(AVG_PRICE_GRAPH)->setLineStyle(QCPGraph::lsLine);

    ui->plot->yAxis->setRange(0,1);

    int current_time = utility::datetime::utc_timestamp();
    ui->trading_plot->xAxis->setRange(current_time, current_time +  TIME_HOUR);

    ui->plot->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    ui->trading_plot->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d MMM\nhh:mm");
    dateTicker->setTickCount(7);

    ui->plot->xAxis->setTicker(dateTicker);
    ui->trading_plot->xAxis->setTicker(dateTicker);

    QPen pen_inst = ui->trading_plot->graph(INSTANT_PRICE_GRAPH)->pen();
    QPen pen_avg = ui->trading_plot->graph(AVG_PRICE_GRAPH)->pen();
    QPen pen_sent = ui->plot->graph(SENTIMENT_GRAPH)->pen();

    pen_inst.setWidth(2);
    pen_avg.setWidth(2);
    pen_sent.setWidth(2);
    pen_avg.setColor(Qt::GlobalColor::red);

    ui->trading_plot->graph(INSTANT_PRICE_GRAPH)->setPen(pen_inst);
    ui->trading_plot->graph(AVG_PRICE_GRAPH)->setPen(pen_avg);
    ui->plot->graph(SENTIMENT_GRAPH)->setPen(pen_sent);


    ui->plot->xAxis->setLabel("Date");
    ui->trading_plot->xAxis->setLabel("Date");

    ui->plot->yAxis->setLabel("Sentiment Index");
    ui->trading_plot->yAxis->setLabel("Price (BTC)");

    ui->trading_plot->graph(INSTANT_PRICE_GRAPH)->setName("Last Traded Price");
    ui->trading_plot->graph(AVG_PRICE_GRAPH)->setName("12 Hour Average Price");

    ui->trading_plot->legend->setVisible(true);
}

void MainWindow::normalizeGraphs()
{
    normalizeData(qy_pos);
}

void MainWindow::normalizeData(QVector<double>& vec)
{
    for (auto& data : vec)
    {
        data = (data - WATSON_LOWEST_VAL) * NORMALIZE_VAL;
    }
}

utility::string_t MainWindow::commentsToString(std::vector<Apollo::Comment> t_comments, std::vector<Apollo::Comment> f_comments, std::vector<utility::string_t> r_comments)
{
    std::string str = "";

    for (auto& comment : t_comments)
        str.append(comment.content);

    for (auto& comment : f_comments)
        str.append(comment.content);

    for (auto& comment : r_comments)
        str.append(utility::conversions::to_utf8string(comment));

    return utility::conversions::to_string_t(str);
}

void MainWindow::on_add_update_gdax_clicked()
{
    this->gdax_accnt->setCredentials(utility::conversions::to_string_t(this->ui->key_gdax->text().toStdString()), utility::conversions::to_string_t(this->ui->secret_gdax->text().toStdString()), utility::conversions::to_string_t(this->ui->password_gdax->text().toStdString()));
    this->gdax_accnt->connect();

    QPalette palette;
    if (this->gdax_accnt->isConnected())
    {
        this->ui->add_update_gdax->setText("Update");
        this->ui->is_connected_gdax->setText("Connected");
        palette.setColor(QPalette::WindowText, Qt::green);

        this->ui->add_update_gdax->update();
        this->ui->add_update_gdax->repaint();

        this->gdax_accnt->update();

        std::vector<Apollo::Exchanges::Coin>& wallet_coins = this->gdax_accnt->coins_vec;
        std::sort(wallet_coins.begin(), wallet_coins.end(), std::greater<Apollo::Exchanges::Coin>());

        if (!have_gdax_wallet)
        {
            this->ui->wallet_gdax->setColumnCount(2);
            this->ui->wallet_gdax->setRowCount(wallet_coins.size());

            this->ui->wallet_gdax->setColumnWidth(0, 120);
            this->ui->wallet_gdax->setHorizontalHeaderItem(0, new QTableWidgetItem("Coin"));
            this->ui->wallet_gdax->setHorizontalHeaderItem(1, new QTableWidgetItem("Amount"));

            for (int i = 0; i < wallet_coins.size(); i++)
            {
                this->ui->wallet_gdax->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(utility::conversions::to_utf8string(wallet_coins[i].coin_id))));
                this->ui->wallet_gdax->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(wallet_coins[i].amount))));
            }
            this->have_gdax_wallet = true;
        }
        else
        {
            for (int i = 0; i < wallet_coins.size(); i++)
            {
                this->ui->wallet_gdax->item(i,1)->setText(QString::fromStdString(std::to_string(wallet_coins[i].amount)));
            }
        }

    }
    else
    {
        this->ui->add_update_gdax->setText("Add");
        this->ui->is_connected_gdax->setText("Not Connected");
        palette.setColor(QPalette::WindowText, Qt::red);
    }
    this->ui->is_connected_gdax->setPalette(palette);

    this->ui->key_gdax->setText("");
    this->ui->secret_gdax->setText("");
    this->ui->password_gdax->setText("");
}

void MainWindow::on_add_update_ku_clicked()
{
    this->kucoin_accnt->setCredentials(utility::conversions::to_string_t(this->ui->key_ku->text().toStdString()), utility::conversions::to_string_t(this->ui->secret_ku->text().toStdString()));

    this->kucoin_accnt->connect();

    QPalette palette;
    if (this->kucoin_accnt->isConnected())
    {
        this->ui->add_update_ku->setText("Update");
        this->ui->is_connected_ku->setText("Connected");
        palette.setColor(QPalette::WindowText, Qt::green);

        this->ui->add_update_ku->update();
        this->ui->add_update_ku->repaint();

        this->kucoin_accnt->update();

        std::vector<Apollo::Exchanges::Coin>& wallet_coins = this->kucoin_accnt->coins_vec;
        std::sort(wallet_coins.begin(), wallet_coins.end(), std::greater<Apollo::Exchanges::Coin>());

        if (!have_kucoin_wallet)
        {
            this->ui->wallet_ku->setColumnCount(2);
            this->ui->wallet_ku->setRowCount(wallet_coins.size());

            this->ui->wallet_ku->setColumnWidth(0, 118);
            this->ui->wallet_ku->setColumnWidth(1, 117);
            this->ui->wallet_ku->setHorizontalHeaderItem(0, new QTableWidgetItem("Coin"));
            this->ui->wallet_ku->setHorizontalHeaderItem(1, new QTableWidgetItem("Amount"));

//            this->ui->wallet_ku->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);

            for (int i = 0; i < wallet_coins.size(); i++)
            {
                this->ui->wallet_ku->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(utility::conversions::to_utf8string(wallet_coins[i].coin_id))));
                this->ui->wallet_ku->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(std::to_string(wallet_coins[i].amount))));
            }
        }
        else
        {
            for (int i = 0; i < wallet_coins.size(); i++)
            {
                this->ui->wallet_ku->item(i,1)->setText(QString::fromStdString(std::to_string(wallet_coins[i].amount)));
            }
        }

    }
    else
    {
        this->ui->add_update_ku->setText("Add");
        this->ui->is_connected_ku->setText("Not Connected");
        palette.setColor(QPalette::WindowText, Qt::red);
    }
    this->ui->is_connected_ku->setPalette(palette);

    this->ui->key_ku->setText("");
    this->ui->secret_ku->setText("");
}

void MainWindow::on_remove_gdax_clicked()
{
    this->gdax_accnt->clearCredentials();
    this->gdax_accnt->connect();

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);

    this->ui->add_update_gdax->setText("Add");
    this->ui->is_connected_gdax->setText("Not Connected");
    this->ui->is_connected_gdax->setPalette(palette);

    this->ui->key_gdax->setText("");
    this->ui->secret_gdax->setText("");
    this->ui->password_gdax->setText("");
}

void MainWindow::on_remove_ku_clicked()
{
    this->kucoin_accnt->clearCredentials();
    this->kucoin_accnt->connect();

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red);

    this->ui->add_update_ku->setText("Add");
    this->ui->is_connected_ku->setText("Not Connected");
    this->ui->is_connected_ku->setPalette(palette);

    this->ui->key_ku->setText("");
    this->ui->secret_ku->setText("");
}

void MainWindow::on_start_session_button_clicked()
{
    if (this->ui->start_session_button->text() == "Start Trading Session")
    {
        TradeDialog* dialog = new TradeDialog();
        dialog->setMainPtr(this);
        dialog->show();

        this->ui->start_session_button->setText("End Trading Session");
    }
    else
    {
        trading_bot->endSession();
        delete trading_bot;
        delete bot_timer;
        counter = 19;
        this->ui->start_session_button->setText("Start Trading Session");
    }
}

void MainWindow::startBots(std::string ticker, std::string subreddit)
{
    twitter_bot->setSearchQuery(ticker);
    reddit_bot->setSubreddit(utility::conversions::to_string_t(subreddit));
    fourchan_bot->setSearchQuery(ticker);
    current_ticker_ = ticker;

    trading_bot = new Apollo::TradingBot(ticker);
    trading_bot->startSession();
}

void MainWindow::setupTimer()
{
    bot_timer = new QTimer(this);
    connect(bot_timer, SIGNAL(timeout()), this, SLOT(updateData()));
    bot_timer->start(3000);
}
