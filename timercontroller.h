#ifndef TIMERCONTROLLER_H
#define TIMERCONTROLLER_H
#include"CellWorld.h"
#include <QTimer>

/*
 *   This class represents the controller in the MVC pattern which also mainly involves CellWorld(the model) and WorldViewer(the viewer).
 *   The class is responsible for controlling the update of the model and its subsequent display by the viewer.
 *   TimerControlled is obviously controlled by the user interface.
 *
 *
*/

class TimerController : public QTimer
{
public:
    TimerController(CellWorld* cellWorld);
    void loop();
    void setSpeed(int ms);
    void playPause();
    void setPause();
    bool isPaused();

private:
    bool pause = false;
    int currentInterval; //in milliseconds
};

#endif // TIMERCONTROLLER_H
