#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include<yoloworkthread.h>
#include<iostream>
#include<QLabel>
#include<QPushButton>
#include<QImage>


using namespace cv;
using namespace std;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

 //widgets:
  //  QPushButton pushButton_beginProc; //in ui.h
    void onButtonBeginDetection();

    void drawPic(const QImage&);
    YoloWorkThread *workThread;




    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
