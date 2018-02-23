#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Twitter.h"
#include "watson.h"
#include "Comment.h"

#include <QTimer>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <algorithm>

const int TIME_WEEK = 604800;
const int TIME_HOUR = 3600;
const int TEN_MIN = 600;
const int NUMBER_OF_PLOT = 21;
const int POS_GRAPH = 0;
const int NEG_GRAPH = 1;
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
    watsonAnalyzer = new Apollo::Watson(WATSON_USERNAME, WATSON_PASSWORD);

    timerStarted = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    currentItem = item;
    std::cout << "Twitter query:\t" << currentItem->toolTip().toStdString();
    twitterBot->setSearchQuery(currentItem->toolTip().toStdString());
    updateData();
    updatePlot();
    if (!timerStarted)
    {

        QTimer *updateData = new QTimer(this);
        connect(updateData, SIGNAL(timeout()), this, SLOT(updateData()));
        updateData->start(30000);
        timerStarted = true;
    }
}

void MainWindow::clearData()
{
    qx.clear();
    qy_pos.clear();
    qy_neg.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(POS_GRAPH)->setData(qx, qy_pos);
    ui->plot->graph(NEG_GRAPH)->setData(qx, qy_neg);
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
        }

        json_file.close();
        normalizeGraphs();
        if (upper > 15)
            ui->plot->xAxis->setRange(qx[0], qx[NUMBER_OF_PLOT - 1]);
        else
        {
            if ((qx[qx.size() -1] - TEN_MIN) > qx[0])
                ui->plot->xAxis->setRange(qx[0], qx[qx.size() - 1]);
            else
                ui->plot->xAxis->setRange(qx[0], qx[0] + TEN_MIN);

        }
        plot();
    }
}

void MainWindow::updateData()
{
    std::string filename = "../resources/" + currentItem->text().toStdString() + ".json";
    std::replace(filename.begin(), filename.end(), ' ', '_');

    std::cout << filename << std::endl;
    auto comments = twitterBot->getData();

    std::cout << comments.size() << std::endl;

    watsonAnalyzer->toneToFile(commentsToString(comments), utility::conversions::to_string_t(filename));

    updatePlot();
}

void MainWindow::setupWidgets()
{
    ui->plot->addGraph();
    ui->plot->addGraph();

    ui->plot->graph(POS_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(NEG_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);

    ui->plot->graph(POS_GRAPH)->setLineStyle(QCPGraph::lsLine);
    ui->plot->graph(NEG_GRAPH)->setLineStyle(QCPGraph::lsLine);

    ui->plot->graph(POS_GRAPH)->setName("Twitter Positive Sentiment");
    ui->plot->graph(NEG_GRAPH)->setName("Twitter Negative Sentiment");

    ui->plot->graph(NEG_GRAPH)->setPen(QPen(Qt::GlobalColor::red));

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

void MainWindow::runBots()
{
    auto comments = twitterBot->getData();

}

void MainWindow::normalizeGraphs()
{
    normalizeData(qy_pos);
    normalizeData(qy_neg);
}

void MainWindow::normalizeData(QVector<double>& vec)
{
    for (auto& data : vec)
    {
        data = (data - WATSON_LOWEST_VAL) * NORMALIZE_VAL;
    }
}

utility::string_t MainWindow::commentsToString(std::vector<Apollo::Comment> comments)
{
    std::string str = "";

    for (auto& comment : comments)
        str.append(comment.content);

    return utility::conversions::to_string_t(str);
}
