#include "timercontroller.h"

TimerController::TimerController(CellWorld* cellWorld)
{
    pause = true;
    currentInterval = 200;
    connect(this, &QTimer::timeout,this, &TimerController::loop);
    connect(this, &QTimer::timeout, cellWorld, &CellWorld::update);
    setSingleShot(true);
}

void TimerController::setSpeed(int ms){
    currentInterval = ms;
}

void TimerController::loop(){
    if(!pause && isSingleShot() && currentInterval>0 )
        start(currentInterval);
}

void TimerController::playPause(){
    stop();
    pause = !pause;
    if(!pause)
        start(currentInterval);
}

bool TimerController::isPaused(){
    return pause;
}
