#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTransform>
#include<QGraphicsItem>
#include<QGraphicsRectItem>
#include<QPainter>
#include<QTime>

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Setup cell dimension
    cellDim = 10;

    //Setup Grid in GraphicsScene
    QPen pen(Qt::black, cellDim/10, Qt::SolidLine);
    QBrush brush(Qt::white);
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0, cellWorld.width()*cellDim, cellWorld.height()*cellDim);
    ui->graphicsView->setScene(scene);

    for(int i=0; i<cellWorld.width();i++){
        for(int j=0;j<cellWorld.height();j++)
            scene->addRect(i*cellDim,j*cellDim,cellDim,cellDim, pen, brush);
    }

    for(int i=0; i<1000; i++){
        QPainter painter;
        QTransform trans(1,0,0,0,1,0,0,0,1);
        for(const auto& iter : cellWorld.getChanged()){
            QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(scene->itemAt(iter.first*cellDim,iter.second*cellDim, trans));
            cellWorld.getState()[iter.first][iter.second].isAlive() ? rect->setBrush(QBrush(Qt::green)) : rect->setBrush(QBrush(Qt::white));
        }
        cellWorld.update();

    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_FrameSlider_valueChanged(int value)
{
    ui->FrameLCD->display(value);
}
