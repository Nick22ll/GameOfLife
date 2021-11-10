#ifndef TIMERCONTROLLER_H
#define TIMERCONTROLLER_H
#include"CellWorld.h"
#include <QTimer>

class TimerController : public QTimer
{
public:
    TimerController(CellWorld* cellWorld);
    void loop();
    void setSpeed(int ms);
    void playPause();
    bool isPaused();

private:
    bool pause = false;
    int currentInterval; //in milliseconds
};

#endif // TIMERCONTROLLER_H
