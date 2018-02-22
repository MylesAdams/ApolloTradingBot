#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "Twitter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

//    void addPoint(double x, double y);
    void clearData();
    void plot();


private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void updatePlot();

private:
    Ui::MainWindow *ui;

    QVector<double> qy_pos, qy_neg, qx;

    QListWidgetItem* currentItem;

    bool timerStarted;

    Apollo::Bot::Twitter twitterBot;

};

#endif // MAINWINDOW_H
