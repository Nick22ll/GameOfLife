#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>


MainWindow::MainWindow(CellWorld* cw, TimerController* t, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1280,720);
    ui->setupUi(this);


    //Setup WorldViewer
    ui->worldViewer->setCellWorld(cw);

    //Setup Timer
    timer = t;
    timer->connect(timer, &QTimer::timeout, ui->worldViewer, &WorldViewer::changeWorld);
    timer->connect(timer, &QTimer::timeout, this, &MainWindow::updateStatistics);
    //Setup playPause button
    ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));

    show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatistics(){
    ui->currentPopulationNumber->setText(QString::number(ui->worldViewer->aliveCells()));
}

void MainWindow::on_FrameSlider_valueChanged(int value)
{
    ui->FrameLCD->display(value);

}

void MainWindow::on_playPauseButton_clicked()
{
    timer->playPause();
    if(timer->isPaused()){
        ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));
    }
    else
        ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/pause.png"));
}


void MainWindow::on_FrameSlider_sliderReleased()
{
    timer->setSpeed(1/ui->FrameLCD->value()*1000);
}


void MainWindow::on_resetButton_clicked()
{
    timer->setPause();
    if(timer->isPaused())
        ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));
    ui->worldViewer->resetWorld();

}


void MainWindow::on_modifyCheckBox_toggled(bool checked)
{
    ui->worldViewer->setModifyFlag(checked);
}


