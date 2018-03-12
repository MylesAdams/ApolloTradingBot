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
    l->hide();
    w->show();
    releaseKeyboard();

}

void ApolloSplashScreen::keyPressEvent(QKeyEvent * k)
{
    std::cout << "WHAT" << std::endl;
    hide();
    l->hide();
    w->show();
    releaseKeyboard();
    QWidget::keyPressEvent(k);
}

void ApolloSplashScreen::setDummyWindowPtr(QLabel *l)
{
    this->l = l;
}
