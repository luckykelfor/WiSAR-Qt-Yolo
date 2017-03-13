#-------------------------------------------------
#
# Project created by QtCreator 2017-03-13T05:26:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = YoloDisplay
TEMPLATE = app

DEFINES += OPENCV GPU CUDNN

SOURCES += main.cpp\
        mainwindow.cpp \
        arapaho/arapaho.cpp \
        yoloworkthread.cpp \
    realtimedisplaythread.cpp



INCLUDEPATH += /usr/local/include \
            /usr/local/cuda/include \
           ./include \
          ./arapaho



LIBS += \
    -L/usr/local/lib  \
    -L/home/amax/fkm/YoloDisplay/lib -ldarknet-cpp-shared \
    -L/usr/local/cuda/lib64 -lcuda -lcudart -lcublas -lcurand
LIBS += `pkg-config --libs opencv`

HEADERS  += mainwindow.h \
            arapaho/arapaho.hpp \
    yoloworkThread.h \
    yoloworkthread.h \
    realtimedisplaythread.h


FORMS    += mainwindow.ui
