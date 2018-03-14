#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "Twitter.h"
#include "FourChan.h"
#include "Watson.h"
#include "Reddit.h"
#include "gdax_accnt.h"
#include "kucoin_accnt.h"
#include "TestExchange.h"
#include "TradingBot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void startBots(std::string ticker, std::string subreddit);
    void setupTimer();


private slots:
    void updateData();
    void on_add_update_gdax_clicked();

    void on_add_update_ku_clicked();

    void on_remove_gdax_clicked();

    void on_remove_ku_clicked();

    void on_start_session_button_clicked();

private:
    Ui::MainWindow *ui;
    QVector<double> qy_pos, qx_sent, qx_fin, qy_avg, qy_inst;
    QListWidgetItem* currentItem;
    int counter;
    bool have_gdax_wallet;
    bool have_kucoin_wallet;
    QTimer* bot_timer;
    std::string current_ticker_;
    double max_price_seen_;
    double min_price_seen_;

    Apollo::Bot::Twitter* twitter_bot;
    Apollo::Bot::Reddit* reddit_bot;
    Apollo::Bot::FourChan* fourchan_bot;
    Apollo::Watson* watson_analyzer;
    Apollo::Exchanges::GdaxAccnt* gdax_accnt;
    Apollo::Exchanges::KucoinAccnt* kucoin_accnt;
    Apollo::Exchanges::TestExchange* test_exchange;
    Apollo::TradingBot* trading_bot;

    void clearData();
    void plot();
    void setupWidgets();
    void normalizeGraphs();
    void normalizeData(QVector<double>& vec);
    void updatePlot();

    // Move to other file later
    utility::string_t commentsToString(std::vector<Apollo::Comment> t_comments, std::vector<Apollo::Comment> f_comments, std::vector<utility::string_t> r_comments);
};

#endif // MAINWINDOW_H
