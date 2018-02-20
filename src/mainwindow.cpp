#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssCircle);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{

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

void MainWindow::on_addButton_clicked()
{
    addPoint(ui->xSpinBox->value(), ui->ySpinBox->value());
    plot();
}

void MainWindow::on_clearButton_clicked()
{
    clearData();
}
