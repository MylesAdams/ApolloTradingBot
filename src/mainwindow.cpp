#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <QTimer>

const int TIME_WEEK = 604800;

const int NUMBER_OF_PLOT = 15;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot->yAxis->setRange(-1,1);

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

    currentItem = ui->listWidget->findItems("Bitcoin", Qt::MatchExactly)[0];

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
        rapidjson::Value &data = doc["entry"];
//        std::cout << "Test5" << std::endl;
        int upper = data.Size();
//        std::cout << upper << std::endl;
        int lower = upper - NUMBER_OF_PLOT;
//        std::cout << lower << std::endl;
        for (int ndx = lower; ndx < upper; ndx++)
        {
//            std::cout << std::stoi(data[ndx]["time"].GetString()) << std::endl;
            q_x.append(std::stoi(data[ndx]["time"].GetString()));
            q_y.append(std::stod(data[ndx]["index"].GetString()));
        }
//        int x = q_x[lower];
//        int y = q_x[upper-1];
//        std::cout << x << "," << y << std::endl;
        ui->plot->xAxis->setRange(q_x[0], q_x[14]);
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

void MainWindow::addPoint(double x, double y)
{
    q_x.append(x);
    q_y.append(y);
}

void MainWindow::clearData()
{
    q_x.clear();
    q_y.clear();
}

void MainWindow::plot()
{
    ui->plot->graph(0)->setData(q_x, q_y);
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
        rapidjson::Value &data = doc["entry"];
        int upper = data.Size();
//        std::cout << upper << std::endl;
        int lower = upper - NUMBER_OF_PLOT;
//        std::cout << lower << std::endl;
        for (int ndx = lower; ndx < upper; ndx++)
        {
//            std::cout << ndx << std::endl;
//            std::cout << std::stoi(data[ndx]["time"].GetString()) << std::endl;
            q_x.append(std::stoi(data[ndx]["time"].GetString()));
            q_y.append(std::stod(data[ndx]["index"].GetString()));
        }
//        std::copy(q_x.begin(), q_x.end(), std::ostream_iterator<char>(std::cout, " "));
//        int x = q_x[lower];
//        int y = q_x[upper-1];
//        std::cout << x << "," << y << std::endl;
        ui->plot->xAxis->setRange(q_x[0], q_x[NUMBER_OF_PLOT - 1]);
        plot();
    }
    json_file.close();
}
