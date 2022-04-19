QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    CellWorld.cpp \
    CustomInputDialog.cpp \
    WorldViewer.cpp \
    cell.cpp \
    graphiccell.cpp \
    main.cpp \
    mainwindow.cpp \
    timercontroller.cpp


HEADERS += \
    CellWorld.h \
    CustomInputDialog.h \
    WorldViewer.h \
    cell.h \
    timercontroller.h \
    mainwindow.h \
    graphiccell.h\

FORMS += mainwindow.ui

RESOURCES += resources.qrc

