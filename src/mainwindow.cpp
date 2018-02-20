#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

const int TIME_WEEK = 604800;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->plot->yAxis->setRange(-1,1);
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
    if (json_file.is_open())
    {
        rapidjson::IStreamWrapper isw(json_file);
        rapidjson::Document doc;
        doc.ParseStream(isw);
        rapidjson::Value &data = doc["entry"];
        for (int i = 0; i < data.Size(); i++)
        {
            q_x.append(std::stoi(data[i]["time"].GetString()));
            q_y.append(std::stod(data[i]["index"].GetString()));
        }
        ui->plot->xAxis->setRange(q_x[0], q_x[q_x.size() - 1]);
        plot();
    }
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
