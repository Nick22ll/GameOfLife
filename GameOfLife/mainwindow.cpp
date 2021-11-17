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
    ui->resetButton->setIcon(QIcon(":/Icons/Icons/reset.png"));

    //Setup Statistics
    ui->currentPopulationNumber->setText(QString::number(ui->worldViewer->aliveCells()));

    show();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatistics(){
    ui->currentPopulationNumber->setText(QString::number(ui->worldViewer->aliveCells()));
    ui->generationNumberLabel->setText(QString::number(ui->generationNumberLabel->text().toInt()+1));
    ui->overallPopulationNumber->setText(QString::number(ui->worldViewer->overallDeads()));
}

void MainWindow::on_FrameSlider_valueChanged(int value)
{
    ui->FrameLCD->display(value);
    timer->setSpeed(1/ui->FrameLCD->value()*1000);
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


void MainWindow::on_resetButton_clicked()
{
    timer->setPause();
    if(timer->isPaused()){
        ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));
        ui->worldViewer->resetWorld();
        ui->currentPopulationNumber->setText(QString::number(ui->worldViewer->aliveCells()));
        ui->generationNumberLabel->setText(QString::number(0));
    }

}


void MainWindow::on_modifyCheckBox_toggled(bool checked)
{
    ui->worldViewer->setModifyFlag(checked);
}



void MainWindow::on_agingBox_toggled(bool checked)
{
    ui->worldViewer->setAgingFlag(checked);
    if(checked)
        timer->connect(timer,&QTimer::timeout, ui->worldViewer, &WorldViewer::agingWorld);
    else
        timer->disconnect(timer,&QTimer::timeout, ui->worldViewer, &WorldViewer::agingWorld);

}


