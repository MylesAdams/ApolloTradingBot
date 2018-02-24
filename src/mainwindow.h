#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "Twitter.h"
#include "FourChan.h"
#include "watson.h"
#include "RedditOauth.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void updateData();

private:
    Ui::MainWindow *ui;
    QVector<double> qy_pos, qy_neg, qx;
    QListWidgetItem* currentItem;
    bool timerStarted;

    Apollo::Bot::Twitter* twitterBot;
    RedditOauth* redditBot;
    Apollo::Watson* watsonAnalyzer;


    void clearData();
    void plot();
    void setupWidgets();
    void runBots();
    void normalizeGraphs();
    void normalizeData(QVector<double>& vec);
    void updatePlot();

    // Move to other file later
    utility::string_t commentsToString(std::vector<Apollo::Comment> comments);

};

#endif // MAINWINDOW_H
