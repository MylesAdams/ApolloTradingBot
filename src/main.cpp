#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QLabel>
#include "ApolloSplashScreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create a dummy window so that we get keypresses on OSX
    QLabel v(0, Qt::FramelessWindowHint);
    v.setMinimumSize(QSize(0, 0));
    v.move(0,0);
    v.show();

    ApolloSplashScreen splash;

    MainWindow w;
    splash.setMainWindowPtr(&w);
    splash.setDummyWindowPtr(&v);
    splash.setPixmap(QPixmap("../resources/splash.png"));
    splash.show();
    splash.grabKeyboard();

    return a.exec();
}
