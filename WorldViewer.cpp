#include "WorldViewer.h"
#include<iostream>
WorldViewer::WorldViewer(QWidget* parent)
    : QGraphicsView(parent)
{

    //Setup Widget Properties
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::WheelFocus);

    //Setup cell dimension
    cellDim = 10;

    //Setup Cell appearence
    deadCellPen = QPen(Qt::black, 0, Qt::SolidLine);
    deadCellBrush = QBrush(Qt::black);
    liveCellPen = QPen(Qt::green, 0, Qt::SolidLine);
    liveCellBrush = QBrush(Qt::green);

    show();
}

int WorldViewer::aliveCells(){
    return cellWorld->alivePopulation();
}

int WorldViewer::overallDeads(){
    return cellWorld->overallDeads();
}

int WorldViewer::updateTime(){
    return cellWorld->updateTime();
}

void WorldViewer::setCellWorld(CellWorld* cw){
    cellWorld = cw;

    //Setup Grid in GraphicsScene
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect(0,0,cw->width()*cellDim, cw->height()*cellDim);
    setScene(scene);
    drawAliveWorld();
}

void WorldViewer::setModifyFlag(bool b){
    enabledModify = b;
    if(enabledModify){
        liveCellPen = QPen(QColor(25, 25, 25, 255), 0, Qt::SolidLine);
        deadCellPen = QPen(QColor(25, 25, 25, 255), 0, Qt::SolidLine);
    }
    else{
        liveCellPen = QPen(Qt::green, 0, Qt::SolidLine);
        deadCellPen = QPen(Qt::black, 0, Qt::SolidLine);
    }
    drawAliveWorld();
}

void WorldViewer::setAgingFlag(bool b){
    agingFlag=b;
    if(!b)
        antiAgeWorld();
}

void WorldViewer::drawAliveWorld(){
    drawWorldGrid();
    for(const auto& iter : cellWorld->getAliveCells()){
        QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(scene()->itemAt(iter.first*cellDim,iter.second*cellDim, transform()));
        rect->setBrush(liveCellBrush);
        rect->setPen(liveCellPen);
    }
}

void WorldViewer::drawWorldGrid(){
    scene()->clear();
    for(int i=0; i<cellWorld->width();i++){
        for(int j=0;j<cellWorld->height();j++)
            scene()->addRect(i*cellDim,j*cellDim,cellDim,cellDim, deadCellPen, deadCellBrush);
    }
}

//Sets the CellWorld cells brushes using color palette of aging
void WorldViewer::agingWorld(){
    for(const auto& iter : cellWorld->getAliveCells()){
        QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(scene()->itemAt(iter.first*cellDim,iter.second*cellDim, transform()));
        int age = cellWorld->getState()[iter.first][iter.second].getAge();
        if(age <= 100){
            rect->setBrush(QBrush(QColor(0,255-(age*2),0)));
            rect->setPen(QPen(QColor(0,255-(age*2),0)));
        }
        else{
            rect->setBrush(QBrush(Qt::red));
            rect->setPen(QPen(Qt::red));
        }
    }
}

//Sets the CellWorld cells brushes using the live cell color
void WorldViewer::antiAgeWorld(){
    for(const auto& iter : cellWorld->getAliveCells()){
        QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(scene()->itemAt(iter.first*cellDim,iter.second*cellDim, transform()));
        rect->setBrush(liveCellBrush);
        rect->setPen(liveCellPen);
    }
}

//Generates an empty CellWorld and draws it
void WorldViewer::emptyWorld(int rows, int columns){
    cellWorld->initialize(rows, columns);
    scene()->clear();
    scene()->setSceneRect(0,0,cellWorld->width()*cellDim, cellWorld->height()*cellDim);
    stretchToBorder();
    drawWorldGrid();
}

//Generates an new randomly populated CellWorld and draws it
void WorldViewer::randomWorld(int rows, int columns){
    cellWorld->initialize(rows, columns, randomAlivePopulation(rows, columns));
    scene()->clear();
    scene()->setSceneRect(0,0,cellWorld->width()*cellDim, cellWorld->height()*cellDim);
    stretchToBorder();
    drawAliveWorld();
}

//Resets the CellWorld to the initial configuration maintaing the same world size
void WorldViewer::resetWorld(){
    QMessageBox msgBox(QMessageBox::Icon::Warning , "World Reset", "You are resetting the world!\nAre you sure?", QMessageBox::Yes | QMessageBox::No,  this);
    msgBox.setDefaultButton(QMessageBox::No);
    int ret = msgBox.exec();
    switch(ret){
        case QMessageBox::Yes:
            cellWorld->reset();;
            drawAliveWorld();
            break;
       case QMessageBox::No:
            break;
       default:
            break;
    }
}

void WorldViewer::stretchToBorder(){
    resetTransform();
    scale(width()/sceneRect().width(), width()/sceneRect().width());
}

//Saves the CellWorld current state into a txt file
void WorldViewer::saveWorld(string filename){
    cellWorld->saveToFile(cellWorld, filename);
}

//Loads a CellWorld from a txt file and draws it
void WorldViewer::loadWorld(string filename){
    cellWorld->loadFromFile(filename, cellWorld);
    scene()->clear();
    scene()->setSceneRect(0,0,cellWorld->width()*cellDim, cellWorld->height()*cellDim);
    stretchToBorder();
    drawAliveWorld();
}



/* *****  EVENT SECTION  ***** */

void WorldViewer::mouseMoveEvent(QMouseEvent *event){
    if (pan && !enabledModify)
        {
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->position().x() - panStart.x()));
            verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->position().y() - panStart.y()));
            panStart = event->position();
            event->accept();
            return;
        }
        event->ignore();
}

void WorldViewer::mousePressEvent(QMouseEvent *event){

    if (event->button() == Qt::LeftButton && !enabledModify)
       {
           pan = true;
           panStart = event->position();
           setCursor(Qt::ClosedHandCursor);
           event->accept();
           return;
       }
    else if(event->button() == Qt::LeftButton && enabledModify){

        int xPos = event->position().x();
        int yPos = event->position().y();
        QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(itemAt(xPos, yPos));
        QPointF point = mapToScene(xPos, yPos);
        cellWorld->modify(int(point.x()/cellDim), int(point.y()/cellDim));
        cellWorld->isAlive(int(point.x()/cellDim),int(point.y()/cellDim)) ? rect->setBrush(liveCellBrush) : rect->setBrush(deadCellBrush);
        event->accept();
        return;
    }
    event->ignore();
}


void WorldViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !enabledModify)
    {
        pan = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}


void WorldViewer::wheelEvent(QWheelEvent *event){
    static const float scaleFactor = 1.1;

    if (event->modifiers() & Qt::ControlModifier)
        {
            if (event->angleDelta().y() > 0 && currentScale<=maxScale)
            {
                scale(scaleFactor, scaleFactor);
                        currentScale *= scaleFactor;
            }
            else if(currentScale > minScale)
            {
                scale(1 / scaleFactor, 1 / scaleFactor);
                        currentScale /= scaleFactor;
            }
            centerOn(mapToScene(event->position().x(),event->position().y()));

        }
        else
        {
            QGraphicsView::wheelEvent(event);
        }
}

void WorldViewer::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    stretchToBorder();
}


void WorldViewer::enterEvent(QEnterEvent *event){
    if(enabledModify){
        setCursor(Qt::PointingHandCursor);
        event->accept();
        return;
    }
    event->ignore();
}

void WorldViewer::leaveEvent(QEvent *event){
    if(enabledModify){
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    event->ignore();
}

