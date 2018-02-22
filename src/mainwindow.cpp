#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <QTimer>
#include "Twitter.h"

const int TIME_WEEK = 604800;

const int NUMBER_OF_PLOT = 15;

const int POS_GRAPH = 0;

const int NEG_GRAPH = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plot->addGraph();
    ui->plot->addGraph();
    ui->plot->graph(POS_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(NEG_GRAPH)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(POS_GRAPH)->setLineStyle(QCPGraph::lsLine);
    ui->plot->graph(NEG_GRAPH)->setLineStyle(QCPGraph::lsLine);
    ui->plot->graph(POS_GRAPH)->setName("Positive Sentiment");
    ui->plot->graph(NEG_GRAPH)->setName("Negative Sentiment");
    ui->plot->graph(NEG_GRAPH)->setPen(QPen(Qt::GlobalColor::red));

    ui->plot->yAxis->setRange(0,1);
    ui->plot->legend->setVisible(true);

    ui->plot->setLocale(QLocale(QLocale::English, QLocale::UnitedStates));
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("d MMM\nyyyy");
    dateTicker->setTickCount(7);

    ui->plot->xAxis->setTicker(dateTicker);
    ui->plot->xAxis->setLabel("Date");
    ui->plot->yAxis->setLabel("Sentiment Index");

    ui->GDAX->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->Binance->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->Kucoin->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->Tabs->setCurrentIndex(0);

//    currentItem = ui->listWidget->findItems("Bitcoin", Qt::MatchExactly)[0];

    twitterBot.addSearchQuery("Vechain", 500);
    auto comments = twitterBot.getData();

    for (auto& com : comments)
    {
        std::cout << com.content << std::endl;
    }


    timerStarted = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    currentItem = item;
    updatePlot();

    if (!timerStarted)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updatePlot()));
        timer->start(1000);
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
    std::string filepath = "../Resources/" + currentItem->text().toStdString() + ".json";
    std::ifstream json_file;
    json_file.open(filepath);
    if (json_file.is_open())
    {
        rapidjson::IStreamWrapper isw(json_file);
        rapidjson::Document doc;
        doc.ParseStream(isw);

        int upper = doc.Size();
        int lower = upper - NUMBER_OF_PLOT;
        for (int ndx = lower; ndx < upper; ndx++)
        {
            qx.append(std::stoi(doc[ndx]["Time"].GetString()));
            qy_pos.append(std::stod(doc[ndx]["PosRating"].GetString()));
            qy_neg.append(std::stod(doc[ndx]["NegRating"].GetString()));
        }

        ui->plot->xAxis->setRange(qx[0], qx[NUMBER_OF_PLOT - 1]);
        plot();
        json_file.close();
    }
}
