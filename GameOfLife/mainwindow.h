#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CellWorld.h"
#include<QGraphicsScene>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_FrameSlider_valueChanged(int value);

private:
    int cellDim;
    Ui::MainWindow *ui;
    CellWorld cellWorld;
    QGraphicsScene *scene = nullptr;

};
#endif // MAINWINDOW_H
