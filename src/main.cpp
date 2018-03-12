#include "mainwindow.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include "ApolloSplashScreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ApolloSplashScreen splash;
    MainWindow w;
    splash.setMainWindowPtr(&w);
    splash.setPixmap(QPixmap("../resources/splash.png"));
    splash.show();


//    QTimer::singleShot(3000, splash, SLOT(close()));
//    QTimer::singleShot(3000, &w, SLOT(show()));
    return a.exec();
}
