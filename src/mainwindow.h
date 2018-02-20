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

    void on_addButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;

    QVector<double> q_x, q_y;
};

#endif // MAINWINDOW_H
