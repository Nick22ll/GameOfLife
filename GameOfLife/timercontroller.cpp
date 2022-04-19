#include "timercontroller.h"

TimerController::TimerController(CellWorld* cellWorld)
{
    pause = true;
    currentInterval = 1000;
    //set the loop call of the timer
    connect(this, &QTimer::timeout,this, &TimerController::loop);
    //set the update call on the linked CellWorld
    connect(this, &QTimer::timeout, cellWorld, &CellWorld::update);
    setSingleShot(true);
}

//Setter function of timer interval
void TimerController::setSpeed(int ms){
    currentInterval = ms;
}

void TimerController::loop(){
    if(!pause && isSingleShot() && currentInterval>0 )
        start(currentInterval);
}

//This function switch from pause to play and vice versa
void TimerController::playPause(){
    stop();
    pause = !pause;
    if(!pause)
        start(currentInterval);
}

void TimerController::setPause(){
    stop();
    pause = true;
}

bool TimerController::isPaused(){
    return pause;
}
