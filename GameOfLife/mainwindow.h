#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include"CustomInputDialog.h"
#include<QMainWindow>
#include<QTransform>
#include<QGraphicsItem>
#include<QPainter>
#include<QTimer>
#include<QtGui>
#include<QFileDialog>
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
    void resetStatistics();
    ~MainWindow();

private slots:
    void on_FrameSlider_valueChanged(int value);
    void on_playPauseButton_clicked();
    void on_resetButton_clicked();

    void on_modifyCheckBox_toggled(bool checked);
    void on_agingBox_toggled(bool checked);

    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_actionEnableAging_triggered();

    void on_actionDisableAging_triggered();

    void on_actionDisableModify_triggered();

    void on_actionEnableModify_triggered();

    void on_actionEmpty_World_triggered();

    void on_actionRandom_World_triggered();

    void on_actionLoad_World_triggered();

    void on_actionSave_World_triggered();

    void on_actionFull_View_triggered();

    void on_actionZoom_In_triggered();

    void on_actionZom_Out_triggered();

private:
    //Timer variables
    TimerController* timer;

    //GUI Variables
    Ui::MainWindow *ui;


};
#endif // MAINWINDOW_H
