#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>


MainWindow::MainWindow(CellWorld* cw, TimerController* t, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setMinimumSize(1280,720);
    ui->setupUi(this);
    setWindowIcon(QIcon(":/Icons/Icons/windowIcon.png"));

    //Setup Toolbar
    ui->actionDisableAging->setEnabled(false);
    ui->actionDisableModify->setEnabled(false);

    //Setup WorldViewer
    ui->worldViewer->setCellWorld(cw);

    //Setup Timer
    timer = t;
    timer->connect(timer, &QTimer::timeout, ui->worldViewer, &WorldViewer::drawAliveWorld);
    timer->connect(timer, &QTimer::timeout, this, &MainWindow::updateStatistics);

    //Setup playPause button
    ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));
    ui->resetButton->setIcon(QIcon(":/Icons/Icons/reset.png"));

    //Setup Statistics
    ui->currentPopulationNumber->setText(QString::number(ui->worldViewer->aliveCells()));
    ui->updateTimeNumber->setText(QString::number(ui->FrameLCD->value()));
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

    int uT = ui->worldViewer->updateTime();
    ui->updateTimeNumber->setText(QString::number(uT));
    if(uT > 0 && uT < 20)
        ui->FrameSlider->setMaximum(50);
    else if(uT >= 20 && uT < 50)
        ui->FrameSlider->setMaximum(25);
    else if(uT >= 50)
        ui->FrameSlider->setMaximum(10);
}

void MainWindow::resetStatistics(){
    ui->currentPopulationNumber->setText(QString::number(ui->worldViewer->aliveCells()));
    ui->generationNumberLabel->setText(QString::number(0));
    ui->overallPopulationNumber->setText(QString::number(0));
    ui->updateTimeNumber->setText(QString::number(0));
}


/* *****  EVENT SECTION  ***** */

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
        resetStatistics();
    }
}

void MainWindow::on_modifyCheckBox_toggled(bool checked)
{
    ui->worldViewer->setModifyFlag(checked);
}

void MainWindow::on_agingBox_toggled(bool checked)
{
    ui->worldViewer->setAgingFlag(checked);
    if(checked){
        timer->connect(timer,&QTimer::timeout, ui->worldViewer, &WorldViewer::agingWorld);
    }
    else
        timer->disconnect(timer,&QTimer::timeout, ui->worldViewer, &WorldViewer::agingWorld);

}

void MainWindow::on_saveButton_clicked()
{
    auto filename = QFileDialog::getSaveFileName( this, "Save GOF Configuration",
                                                  "InitialConfiguration",
                                                  tr("Text files (*.txt)"),
                                                  nullptr,
                                                  QFileDialog::Options(QFileDialog::List));
    ui->worldViewer->saveWorld(filename.toStdString());
}

void MainWindow::on_loadButton_clicked()
{
    timer->setPause();
    if(timer->isPaused())
        ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));
    QString filename = QFileDialog::getOpenFileName(this, tr("Load GOF Configuration"),
                                                    "InitialConfiguration/",
                                                    tr("Text files (*.txt)"),
                                                    nullptr,
                                                    QFileDialog::Options(QFileDialog::List));
    ui->worldViewer->loadWorld(filename.toStdString());
    resetStatistics();
}


void MainWindow::on_actionEnableAging_triggered()
{
    ui->agingBox->setChecked(true);
    ui->actionEnableAging->setEnabled(false);
    ui->actionDisableAging->setEnabled(true);
}


void MainWindow::on_actionDisableAging_triggered()
{
    ui->agingBox->setChecked(false);
    ui->actionEnableAging->setEnabled(true);
    ui->actionDisableAging->setEnabled(false);
}


void MainWindow::on_actionDisableModify_triggered()
{
    ui->modifyCheckBox->setChecked(false);
    ui->actionDisableModify->setEnabled(false);
    ui->actionEnableModify->setEnabled(true);
}

void MainWindow::on_actionEnableModify_triggered()
{
    ui->modifyCheckBox->setChecked(true);
    ui->actionEnableModify->setEnabled(false);
    ui->actionDisableModify->setEnabled(true);
}


void MainWindow::on_actionEmpty_World_triggered()
{
    bool ok;
    CustomInputDialog dialog(this);
    vector<int> dimensions = dialog.getValues(this, &ok);
    if(ok){
        timer->setPause();
        if(timer->isPaused())
            ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));
        ui->worldViewer->emptyWorld(dimensions[0], dimensions[1]);
        resetStatistics();
    }
}

void MainWindow::on_actionRandom_World_triggered()
{
    bool ok;
    CustomInputDialog dialog(this);
    vector<int> dimensions = dialog.getValues(this, &ok);
    if(ok){
        timer->setPause();
        ui->worldViewer->randomWorld(dimensions[0], dimensions[1]);
        ui->playPauseButton->setIcon(QIcon(":/Icons/Icons/play.png"));
        resetStatistics();
    }
}


void MainWindow::on_actionLoad_World_triggered()
{
    on_loadButton_clicked();
}


void MainWindow::on_actionSave_World_triggered()
{
    on_saveButton_clicked();
}


void MainWindow::on_actionFull_View_triggered()
{
    ui->worldViewer->stretchToBorder();
}


void MainWindow::on_actionZoom_In_triggered()
{
    ui->worldViewer->scale(1.1,1.1);
}


void MainWindow::on_actionZom_Out_triggered()
{
    ui->worldViewer->scale(1/1.1,1/1.1);
}

