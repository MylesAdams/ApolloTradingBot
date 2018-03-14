#ifndef TRADEDIALOG_H
#define TRADEDIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class TradeDialog;
}

class TradeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TradeDialog(QWidget *parent = 0);
    ~TradeDialog();
    void setMainPtr(MainWindow* w);

private slots:
    void on_ok_button_clicked();

    void on_cancel_button_clicked();

private:
    Ui::TradeDialog *ui;
    MainWindow* main;
};

#endif // TRADEDIALOG_H
