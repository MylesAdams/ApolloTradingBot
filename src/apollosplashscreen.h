#ifndef APOLLOSPLASHSCREEN_H
#define APOLLOSPLASHSCREEN_H

#include <QSplashScreen>
#include "mainwindow.h"

class ApolloSplashScreen : public QSplashScreen
{
public:
    void setMainWindowPtr(MainWindow* w);

    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *);

private:
    MainWindow* w;
};

#endif // APOLLOSPLASHSCREEN_H
