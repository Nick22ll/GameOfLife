#include "mainwindow.h"
#include "CellWorld.h"
#include <QApplication>
#include"timercontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CellWorld cellWorld;
    cellWorld.loadFromFile("InitialConfiguration/GOF(60x40).txt", &cellWorld);

    TimerController timer(&cellWorld);

    MainWindow w(&cellWorld, &timer);
    timer.loop();

    return a.exec();
}
