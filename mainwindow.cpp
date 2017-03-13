#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    workThread = new YoloWorkThread();
    dispThread = new RealTimeDisplayThread();


    connect(dispThread,&RealTimeDisplayThread::transmitCurrentFrame,this,&MainWindow::dispRealTimeView);
    connect(ui->pushButton_realTimeView,&QPushButton::clicked,this,&MainWindow::onButton_dispRealTimeView);
    connect(ui->pushButton_beginProc,&QPushButton::clicked,this,&MainWindow::onButton_dispProcessResults);
    connect(workThread,&YoloWorkThread::frameProcessed,this,&MainWindow::dispProcessResults);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(workThread)
        delete workThread;
    if(dispThread)
        delete dispThread;
}

void MainWindow::dispProcessResults(const QImage & frame)
{

    // printf("IN");
    // cv::Mat image = imread("./test.png");
    // cvtColor(image, image, CV_BGR2RGB);
    //QImage imageQ((unsigned char*)image.data,image.cols,image.rows,image.cols*3,QImage::Format_RGB888);

    // if(frame.data_ptr() == NULL)printf("NULL");
    QImage scaledFrame = frame.scaled(ui->label_processedFrame->size());
    ui->label_processedFrame->setPixmap(QPixmap::fromImage(scaledFrame));

    //            label.resize(label.pixmap()->size()););
    //  ui->label_processedFrame->resize(ui->label_processedFrame->pixmap()->size());//size());
    ui->label_processedFrame->show();
    ;//TODO
}

void MainWindow::onButton_dispProcessResults()
{


    if(workThread->isRunning())return;
    else
        workThread->start();

}
void MainWindow::onButton_dispRealTimeView()
{
    if(dispThread->isRunning())return;
    else
        dispThread->start();
}   // pushB.show();
void MainWindow::dispRealTimeView(const QImage & frame)
{

    QImage scaledFrame = frame.scaled(ui->label_realTimeFrame->size());
    ui->label_realTimeFrame->setPixmap(QPixmap::fromImage(scaledFrame));

    //            label.resize(label.pixmap()->size()););



    // ui->label_realTimeFrame->resize(ui->label_realTimeFrame->pixmap()->size());//size());
    ui->label_realTimeFrame->show();
}
