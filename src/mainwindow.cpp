#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Twitter.h"
#include "Watson.h"
#include "Comment.h"
#include "GDAXPrice.h"

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

    twitterBot = new Apollo::Bot::Twitter();
    redditBot = new Apollo::Bot::Reddit();
    fourchanBot = new Apollo::Bot::FourChan();
    watsonAnalyzer = new Apollo::Watson(WATSON_USERNAME, WATSON_PASSWORD);

    timerStarted = false;
    counter = 5;
    currentPrice = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    currentItem = item;
    std::cout << "Query:\t" << currentItem->toolTip().toStdString() << std::endl;

    twitterBot->setSearchQuery(currentItem->toolTip().toStdString());
    std::cout << "Search query set for Twitter" << std::endl;
    fourchanBot->setSearchQuery(currentItem->toolTip().toStdString());
    std::cout << "Search query set for 4Chan" << std::endl;
    redditBot->setSubreddit(utility::conversions::to_string_t(currentItem->statusTip().toStdString()));
    std::cout << "Search query set for Reddit" << std::endl;

    updateData();
    updatePlot();
    if (!timerStarted)
    {

        QTimer *updateData = new QTimer(this);
        connect(updateData, SIGNAL(timeout()), this, SLOT(updateData()));
        updateData->start(6000);
        timerStarted = true;
    }
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
    std::cout << this->counter << std::endl;
    std::stringstream price;
    double priceDouble = Apollo::getAskingPriceGdax(utility::conversions::to_string_t(currentItem->toolTip().toStdString()));
//    currentPrice.setf(std::fixed, std::floatField);
    price.precision(2);
    price << std::fixed;
    price << "$" << priceDouble;

    if (priceDouble > this->currentPrice)
        ui->priceText->setTextColor(Qt::GlobalColor::green);
    else if (priceDouble < this->currentPrice)
        ui->priceText->setTextColor(Qt::GlobalColor::red);
    this->currentPrice = priceDouble;

    ui->priceText->setText(QString::fromStdString(price.str()));
    std::cout << ui->priceText->toPlainText().toStdString() << std::endl;

    if (this->counter == 5)
    {
        std::string filename = "../resources/" + currentItem->text().toStdString() + ".json";
        std::replace(filename.begin(), filename.end(), ' ', '_');

        std::cout << filename << std::endl;
        auto t_comments = twitterBot->getData();
        std::cout << "Twitter Size: " << t_comments.size() << std::endl;
        auto f_comments = fourchanBot->getData();
        std::cout << "4Chan Size: " << f_comments.size() << std::endl;
        redditBot->readComments();
        auto r_comments = redditBot->getComments();
        std::cout << "Reddit Size: " << r_comments.size() << std::endl;

        utility::string_t tone_input = commentsToString(t_comments, f_comments, r_comments);
        watsonAnalyzer->toneToFile(tone_input, utility::conversions::to_string_t(filename));
        counter = 0;
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

    ui->GDAX->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->Binance->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->Kucoin->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->Tabs->setCurrentIndex(0);
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

utility::string_t MainWindow::commentsToString(std::vector<Apollo::Comment> t_comments, std::vector<Apollo::Comment> f_comments, std::vector<std::string> r_comments)
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
