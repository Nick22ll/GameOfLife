#ifndef WORLDVIEWER_H
#define WORLDVIEWER_H
#include"CellWorld.h"
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsRectItem>
#include<QBrush>
#include<QPen>
#include<QWidget>
#include<QMouseEvent>
#include<QScrollBar>
#include<QSizePolicy>

class WorldViewer : public QGraphicsView
{
public:
    WorldViewer(QWidget* parent = nullptr);
    void setCellWorld(CellWorld* cw);
    void drawWorldGrid();
    int aliveCells();
    void changeWorld();
    void stretchToBorder();
    void resetWorld();
    void setModifyFlag(bool b);
private:
    //Events Handler
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void enterEvent(QEnterEvent* event);
    virtual void leaveEvent(QEvent* event);

    //virtual void keyPressEvent(QKeyEvent *event);
    //virtual void keyReleaseEvent(QKeyEvent *event);

    virtual void resizeEvent(QResizeEvent *event);

    //Events Variables
    bool pan;
    QPointF panStart;

    bool enabledModify = false;

    //Scale Controll Variables
    float scaleFactor = 1.1;
    float currentScale = 1;
    float maxScale = 5;
    float minScale = 1;

    //Cell Variables
    CellWorld* cellWorld;
    int cellDim;
    QBrush deadCellBrush, liveCellBrush;
    QPen deadCellPen, liveCellPen;

};

#endif // WORLDVIEWER_H
