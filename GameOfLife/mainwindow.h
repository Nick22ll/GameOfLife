#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QTransform>
#include<QGraphicsItem>
#include<QPainter>
#include<QTimer>
#include<QtGui>

#include"timercontroller.h"
#include"WorldViewer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CellWorld* cw, TimerController* t, QWidget *parent = nullptr);
    void updateStatistics();
    ~MainWindow();

private slots:
    void on_FrameSlider_valueChanged(int value);
    void on_playPauseButton_clicked();
    void on_resetButton_clicked();

    void on_modifyCheckBox_toggled(bool checked);
    void on_agingBox_toggled(bool checked);

private:
    //Timer variables
    TimerController* timer;

    //GUI Variables
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
