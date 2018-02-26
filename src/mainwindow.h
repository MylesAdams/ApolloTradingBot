#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "Twitter.h"
#include "FourChan.h"
#include "Watson.h"
#include "Reddit.h"

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
    QVector<double> qy_pos, qy_neg, /*f_qy_pos, f_qy_neg, r_qy_pos, r_qy_neg,*/ qx;
    QListWidgetItem* currentItem;
    bool timerStarted;

    Apollo::Bot::Twitter* twitterBot;
    Apollo::Bot::Reddit* redditBot;
    Apollo::Bot::FourChan* fourchanBot;
    Apollo::Watson* watsonAnalyzer;


    void clearData();
    void plot();
    void setupWidgets();
    void normalizeGraphs();
    void normalizeData(QVector<double>& vec);
    void updatePlot();

    // Move to other file later
    utility::string_t commentsToString(std::vector<Apollo::Comment> t_comments, std::vector<Apollo::Comment> f_comments, std::vector<utility::string_t> r_comments);
};

#endif // MAINWINDOW_H
