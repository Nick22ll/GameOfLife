#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CellWorld.h"
#include<QGraphicsScene>
#include"timercontroller.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CellWorld* cw, TimerController* t, QWidget *parent = nullptr);
    void drawWorld();
    void changeWorld();
    ~MainWindow();

private slots:
    void on_FrameSlider_valueChanged(int value);

    void on_playButton_clicked();

    void on_playPauseButton_clicked();

private:
    //Timer variables
    TimerController* timer;

    //Cell Variables
    int cellDim;
    QBrush cellBrush;
    QPen cellPen;

    //CellWorld Variables
    CellWorld* cellWorld;

    //GUI Variables
    Ui::MainWindow *ui;
    QGraphicsScene *scene = nullptr;

};
#endif // MAINWINDOW_H
