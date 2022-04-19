#ifndef GRAPHICCELL_H
#define GRAPHICCELL_H
#include<QPainter>
#include<QPen>
#include<QBrush>
#include <QRect>

class GraphicCell : public QRect
{
public:
    GraphicCell(int width, int height, QPainter* p);
    void paint();
private:
    QPainter* painter;
};

#endif // GRAPHICCELL_H
