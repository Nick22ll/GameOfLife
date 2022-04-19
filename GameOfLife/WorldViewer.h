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
#include<QMessageBox>

/*
 * This class represents the viewer in the MVC pattern which also mainly involves CelllWorld(the model) and TimerController(the controller).
 * The class is a widget that allows the display of a cellWorld. It can be used as a pure viewer but it is also equipped with functions that allow it to control the CellWorld(the model)
 * so that it can act as an intermediary between a possible user interface and the CellWorld(so basically it could be a Controller-Viewer that is controlled by a third part, like this case).
*/

class WorldViewer : public QGraphicsView
{
public:
    //Initializers
    WorldViewer(QWidget* parent = nullptr);


    //Drawing Methods
    void drawWorldGrid();
    void drawAliveWorld();
    void agingWorld();
    void antiAgeWorld();  //Restores the aspect of aged cell to a new birth cell

    //Getters
    int aliveCells();
    int overallDeads();
    int updateTime();

    //Change Viewing Methods
    void stretchToBorder();

    //Setters
    void setCellWorld(CellWorld* cw);
    void emptyWorld(int rows, int columns);
    void randomWorld(int rows, int columns);
    void resetWorld();
    void setModifyFlag(bool b);
    void setAgingFlag(bool b);

    //Save/Load Methods
    void saveWorld(string filename);
    void loadWorld(string filename);

private:
    //Events Handler
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void enterEvent(QEnterEvent* event);
    virtual void leaveEvent(QEvent* event);

    virtual void resizeEvent(QResizeEvent *event);

    //Events Variables
    bool pan;
    QPointF panStart;
    bool agingFlag = true;
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
