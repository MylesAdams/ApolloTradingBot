#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addPoint(double x, double y);
    void clearData();
    void plot();


private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void updatePlot();

private:
    Ui::MainWindow *ui;

    QVector<double> q_x, q_y;

    QListWidgetItem* currentItem;

    bool timerStarted;

};

#endif // MAINWINDOW_H
