#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "mainwindow.h"
#include<QTime>
#include<QCoreApplication>

class Controller
{
public:
    Controller(MainWindow* w, CellWorld* c_w);
    void run();

private:
    MainWindow* window;
    CellWorld* cellWorld;

};

#endif // CONTROLLER_H
