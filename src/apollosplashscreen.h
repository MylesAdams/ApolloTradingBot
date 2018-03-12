#ifndef APOLLOSPLASHSCREEN_H
#define APOLLOSPLASHSCREEN_H

#include <QSplashScreen>
#include "mainwindow.h"
#include <QWidget>
#include <QLabel>

class ApolloSplashScreen : public QSplashScreen
{
public:
    void setMainWindowPtr(MainWindow* w);

    void setDummyWindowPtr(QLabel* l);

    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *) Q_DECL_OVERRIDE;

private:
    MainWindow* w;
    QLabel* l;
};

#endif // APOLLOSPLASHSCREEN_H
