#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "Twitter.h"
#include "watson.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void clearData();
    void plot();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void updatePlot();
    void updateData();

private:
    Ui::MainWindow *ui;
    QVector<double> qy_pos, qy_neg, qx;
    QListWidgetItem* currentItem;
    bool timerStarted;

    Apollo::Bot::Twitter* twitterBot;
    Apollo::Watson* watsonAnalyzer;

    void setupWidgets();
    void runBots();
    void normalizeGraphs();
    void normalizeData(QVector<double>& vec);

    // Move to other file later
    utility::string_t commentsToString(std::vector<Apollo::Comment> comments);

};

#endif // MAINWINDOW_H
