#include "apollosplashscreen.h"
#include "mainwindow.h"
#include <QWidget>

void ApolloSplashScreen::setMainWindowPtr(MainWindow* w)
{
    this->w = w;
}

void ApolloSplashScreen::mousePressEvent(QMouseEvent *)
{
    hide();
    w->show();
}

void ApolloSplashScreen::keyPressEvent(QKeyEvent * k)
{
    hide();
    w->show();

    QWidget::keyPressEvent(k);
}
