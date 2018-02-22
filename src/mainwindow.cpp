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

    timerStarted = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    clearData();
    std::string filepath = "../Resources/" + item->text().toStdString() + ".json";
    std::ifstream json_file;
    json_file.open(filepath);
//    std::cout << "Test1" << std::endl;
    if (json_file.is_open())
    {
        rapidjson::IStreamWrapper isw(json_file);
//        std::cout << "Test2" << std::endl;
        rapidjson::Document doc;
//        std::cout << "Test3" << std::endl;
        doc.ParseStream(isw);
//        std::cout << "Test4" << std::endl;
//        rapidjson::Value &data = doc;
//        std::cout << "Test5" << std::endl;
        int upper = doc.Size();
//        std::cout << upper << std::endl;
        int lower = upper - NUMBER_OF_PLOT;
//        std::cout << lower << std::endl;
        for (int ndx = lower; ndx < upper; ndx++)
        {
            qx.append(std::stoi(doc[ndx]["Time"].GetString()));
            qy_pos.append(std::stod(doc[ndx]["PosRating"].GetString()));
            qy_neg.append(std::stod(doc[ndx]["NegRating"].GetString()));
        }
        ui->plot->xAxis->setRange(qx[0], qx[NUMBER_OF_PLOT - 1]);
        plot();

    }
    json_file.close();

    if (!timerStarted)
    {
        QTimer *timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(updatePlot()));
        timer->start(1000);
        timerStarted = true;
    }

    currentItem = item;
}

//void MainWindow::addPoint(double x, double y)
//{
//    qx.append(x);
//    q_y.append(y);
//}

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
//        rapidjson::Value &data = doc;
        int upper = doc.Size();
//        std::cout << upper << std::endl;
        int lower = upper - NUMBER_OF_PLOT;
//        std::cout << lower << std::endl;
        for (int ndx = lower; ndx < upper; ndx++)
        {
//            std::cout << ndx << std::endl;
//            std::cout << std::stoi(data[ndx]["time"].GetString()) << std::endl;
            qx.append(std::stoi(doc[ndx]["Time"].GetString()));
            qy_pos.append(std::stod(doc[ndx]["PosRating"].GetString()));
            qy_neg.append(std::stod(doc[ndx]["NegRating"].GetString()));
        }
//        std::copy(q_x.begin(), q_x.end(), std::ostream_iterator<char>(std::cout, " "));
//        int x = q_x[lower];
//        int y = q_x[upper-1];
//        std::cout << x << "," << y << std::endl;
        ui->plot->xAxis->setRange(qx[0], qx[NUMBER_OF_PLOT - 1]);
        plot();
    }
    json_file.close();
}
