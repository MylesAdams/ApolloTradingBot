#include "TradeDialog.h"
#include "ui_tradedialog.h"
#include <QTimer>

TradeDialog::TradeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TradeDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Setup Trading Session");
    this->setAttribute(Qt::WA_DeleteOnClose, true);
}

TradeDialog::~TradeDialog()
{
    delete ui;
}

void TradeDialog::setMainPtr(MainWindow* w)
{
    main = w;
}

void TradeDialog::on_ok_button_clicked()
{
    main->startBots(this->ui->ticker_sym->text().toStdString(), this->ui->subreddit->text().toStdString());
    main->setupTimer();
    this->close();
}

void TradeDialog::on_cancel_button_clicked()
{
    this->close();
}
