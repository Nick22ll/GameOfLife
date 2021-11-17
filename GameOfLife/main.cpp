#include "mainwindow.h"
#include "CellWorld.h"
#include "controller.h"
#include <QApplication>
#include"timercontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CellWorld cellWorld(100,100);

    TimerController timer(&cellWorld);

    MainWindow w(&cellWorld, &timer);
    timer.loop();

    return a.exec();
}
