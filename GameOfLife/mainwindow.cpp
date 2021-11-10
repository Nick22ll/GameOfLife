#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTransform>
#include<QGraphicsItem>
#include<QGraphicsRectItem>
#include<QPainter>
#include<QTimer>


MainWindow::MainWindow(CellWorld* cw, TimerController* t, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cellWorld = cw;
    timer = t;

    timer->connect(timer ,&QTimer::timeout, this, &MainWindow::changeWorld);

    ui->setupUi(this);
    //Setup cell dimension
    cellDim = 10;

    //Setup Cell appearence
    cellPen = QPen(Qt::black, cellDim/10, Qt::SolidLine);
    cellBrush = QBrush(Qt::white);


    //Setup Grid in GraphicsScene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0, cellWorld->width()*cellDim, cellWorld->height()*cellDim);
    ui->graphicsView->setScene(scene);

    drawWorld();
    changeWorld();

    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::drawWorld(){
    for(int i=0; i<cellWorld->width();i++){
        for(int j=0;j<cellWorld->height();j++)
            scene->addRect(i*cellDim,j*cellDim,cellDim,cellDim, cellPen, cellBrush);
    }
}

void MainWindow::changeWorld(){
    for(const auto& iter : cellWorld->getChanged()){
        QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(scene->itemAt(iter.first*cellDim,iter.second*cellDim, ui->graphicsView->transform()));
        cellWorld->getState()[iter.first][iter.second].isAlive() ? rect->setBrush(QBrush(Qt::green)) : rect->setBrush(QBrush(Qt::white));
    }
}

void MainWindow::on_FrameSlider_valueChanged(int value)
{
    ui->FrameLCD->display(value);
}



void MainWindow::on_playPauseButton_clicked()
{
    timer->playPause();
    if(timer->isPaused())
        ui->playPauseButton->setText("Play");
    else
        ui->playPauseButton->setText("Pause");

}

