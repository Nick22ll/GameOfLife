#include "controller.h"


void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


Controller::Controller(MainWindow* w, CellWorld* c_w)
{
    window = w;
    cellWorld = c_w;
    window->show();
}

void Controller::run(){
    for(int i=0; i<100; i++){
        cellWorld->update();
        window->changeWorld();
        delay();
    }
}

