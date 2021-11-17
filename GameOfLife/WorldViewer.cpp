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

void WorldViewer::setCellWorld(CellWorld* cw){
    cellWorld = cw;

    //Setup Grid in GraphicsScene
    QGraphicsScene* scene = new QGraphicsScene();
    scene->setSceneRect(0,0,cw->width()*cellDim, cw->height()*cellDim);
    setScene(scene);
    drawWorldGrid();
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
    drawWorldGrid();
    drawAliveWorld();
}

void WorldViewer::setAgingFlag(bool b){
    agingFlag=b;
    if(!b)
        antiAgeWorld();
}

void WorldViewer::drawAliveWorld(){
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

void WorldViewer::changeWorld(){
    for(const auto& iter : cellWorld->getChanged()){
        QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(scene()->itemAt(iter.first*cellDim,iter.second*cellDim, transform()));
        if(cellWorld->getState()[iter.first][iter.second].isAlive()){           
            rect->setBrush(liveCellBrush);
            rect->setPen(liveCellPen);
        }
        else{
            rect->setBrush(deadCellBrush);
            rect->setPen(deadCellPen);
        }
    }
}

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

void WorldViewer::antiAgeWorld(){
    for(const auto& iter : cellWorld->getAliveCells()){
        QGraphicsRectItem* rect = qgraphicsitem_cast<QGraphicsRectItem *>(scene()->itemAt(iter.first*cellDim,iter.second*cellDim, transform()));
        rect->setBrush(liveCellBrush);
        rect->setPen(liveCellPen);
    }
}
void WorldViewer::resetWorld(){
    QMessageBox msgBox;
    msgBox.setText("You are resetting the world!");
    msgBox.setInformativeText("Are you sure?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();
    switch(ret){
        case QMessageBox::Yes:
            cellWorld->clear();
            drawWorldGrid();
            changeWorld();
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

