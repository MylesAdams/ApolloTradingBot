#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Twitter.h"
#include "Watson.h"
#include "Comment.h"
#include "coin.h"

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
const int NUMBER_OF_PLOT = 21;
const int TWITTER_POS_GRAPH = 0;
const int TWITTER_NEG_GRAPH = 1;
const int FOURCHAN_POS_GRAPH = 2;
const int FOURCHAN_NEG_GRAPH = 3;
const int REDDIT_POS_GRAPH = 4;
const int REDDIT_NEG_GRAPH = 5;
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

    timerStarted = false;
    counter = 199;
    have_gdax_wallet = false;
    have_kucoin_wallet = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clearData()
{
    qx.clear();
    qy_pos.clear();
    qy_neg.clear();
//    f_qy_pos.clear();
//    f_qy_neg.clear();
//    r_qy_pos.clear();
//    r_qy_neg.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(TWITTER_POS_GRAPH)->setData(qx, qy_pos);
    ui->plot->graph(TWITTER_NEG_GRAPH)->setData(qx, qy_neg);
//    ui->plot->graph(FOURCHAN_POS_GRAPH)->setData(qx, f_qy_pos);
//    ui->plot->graph(FOURCHAN_NEG_GRAPH)->setData(qx, f_qy_neg);
//    ui->plot->graph(REDDIT_POS_GRAPH)->setData(qx, r_qy_pos);
//    ui->plot->graph(REDDIT_NEG_GRAPH)->setData(qx, r_qy_neg);
    ui->plot->replot();
    ui->plot->update();
}

void MainWindow::updatePlot()
{
    clearData();
    std::string filepath = "../resources/" + currentItem->text().toStdString() + ".json";
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
        if (upper > 14)
           lower = upper - NUMBER_OF_PLOT;
        else
            lower = 0;

        for (int ndx = lower; ndx < upper; ndx++)
        {
            qx.append(doc[ndx]["Time"].GetDouble());

            qy_pos.append(doc[ndx]["PosRating"].GetDouble());
            qy_neg.append(doc[ndx]["NegRating"].GetDouble());
            std::cout << std::to_string(doc[ndx]["Time"].GetDouble()) << " | " << std::to_string(doc[ndx]["PosRating"].GetDouble()) << " | " <<  std::to_string(doc[ndx]["NegRating"].GetDouble()) << std::endl;
        }

        json_file.close();
        normalizeGraphs();
        ui->plot->xAxis->setRange(qx[qx.size() - 1] - TEN_MIN, qx[qx.size() - 1] + 10);
//        if (upper > 15)
//            ui->plot->xAxis->setRange(qx[0], qx[NUMBER_OF_PLOT - 1]);
//        else
//        {
////            if ((qx[qx.size() -1] - TEN_MIN) > qx[0])
////                ui->plot->xAxis->setRange(qx[0], qx[qx.size() - 1]);
////            else
////                ui->plot->xAxis->setRange(qx[0], qx[0] + TEN_MIN);

////            std::cout << qx[qx.size() - 1] << " | " << qx[qx.size() - 1] + TEN_MIN << std::endl;
//            ui->plot->xAxis->setRange(qx[qx.size() - 1], qx[qx.size() - 1] + TEN_MIN);
//        }
        plot();
    }
}

void MainWindow::updateData()
{

    if (this->counter == 199)
    {
        std::string filename = "../resources/" + currentItem->text().toStdString() + ".json";
        std::replace(filename.begin(), filename.end(), ' ', '_');

        std::cout << filename << std::endl;
        auto t_comments = twitter_bot->getData();
        std::cout << "Twitter Size: " << t_comments.size() << std::endl;
        auto f_comments = fourchan_bot->getData();
        std::cout << "4Chan Size: " << f_comments.size() << std::endl;
        reddit_bot->readComments();
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
    ui->plot->addGraph();
//    ui->plot->addGraph();
//    ui->plot->addGraph();
//    ui->plot->addGraph();
//    ui->plot->addGraph();

    ui->plot->graph(TWITTER_POS_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(TWITTER_NEG_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
//    ui->plot->graph(FOURCHAN_POS_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
//    ui->plot->graph(FOURCHAN_NEG_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
//    ui->plot->graph(REDDIT_POS_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
//    ui->plot->graph(REDDIT_NEG_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);

    ui->plot->graph(TWITTER_POS_GRAPH)->setLineStyle(QCPGraph::lsLine);
    ui->plot->graph(TWITTER_NEG_GRAPH)->setLineStyle(QCPGraph::lsLine);
//    ui->plot->graph(FOURCHAN_POS_GRAPH)->setLineStyle(QCPGraph::lsLine);
//    ui->plot->graph(FOURCHAN_NEG_GRAPH)->setLineStyle(QCPGraph::lsLine);
//    ui->plot->graph(REDDIT_POS_GRAPH)->setLineStyle(QCPGraph::lsLine);
//    ui->plot->graph(REDDIT_NEG_GRAPH)->setLineStyle(QCPGraph::lsLine);

    ui->plot->graph(TWITTER_POS_GRAPH)->setName("Positive Sentiment");
    ui->plot->graph(TWITTER_NEG_GRAPH)->setName("Negative Sentiment");
//    ui->plot->graph(FOURCHAN_POS_GRAPH)->setName("4Chan Positive Sentiment");
//    ui->plot->graph(FOURCHAN_NEG_GRAPH)->setName("4Chan Negative Sentiment");
//    ui->plot->graph(REDDIT_POS_GRAPH)->setName("Reddit Positive Sentiment");
//    ui->plot->graph(REDDIT_NEG_GRAPH)->setName("Reddit Negative Sentiment");

    ui->plot->graph(TWITTER_NEG_GRAPH)->setPen(QPen(Qt::GlobalColor::red));
//    ui->plot->graph(FOURCHAN_POS_GRAPH)->setPen(QPen(Qt::GlobalColor::green));
//    ui->plot->graph(FOURCHAN_NEG_GRAPH)->setPen(QPen(Qt::GlobalColor::magenta));
//    ui->plot->graph(REDDIT_POS_GRAPH)->setPen(QPen(Qt::GlobalColor::yellow));
//    ui->plot->graph(REDDIT_NEG_GRAPH)->setPen(QPen(Qt::GlobalColor::black));

    ui->plot->yAxis->setRange(0,1);
    ui->plot->legend->setVisible(true);

    ui->plot->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d MMM\nhh:mm");
    dateTicker->setTickCount(7);

    ui->plot->xAxis->setTicker(dateTicker);
    ui->plot->xAxis->setLabel("Date");
    ui->plot->yAxis->setLabel("Sentiment Index");
}

void MainWindow::normalizeGraphs()
{
    normalizeData(qy_pos);
    normalizeData(qy_neg);
//    normalizeData(f_qy_pos);
//    normalizeData(f_qy_neg);
//    normalizeData(r_qy_pos);
//    normalizeData(r_qy_neg);
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

//            this->ui->wallet_gdax->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
            this->ui->wallet_gdax->setColumnWidth(0, 120);
            this->ui->wallet_gdax->setHorizontalHeaderItem(0, new QTableWidgetItem("Coin"));
            this->ui->wallet_gdax->setHorizontalHeaderItem(0, new QTableWidgetItem("Amount"));

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

            this->ui->wallet_ku->setColumnWidth(0, 110);
            this->ui->wallet_ku->setColumnWidth(1, 110);
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
