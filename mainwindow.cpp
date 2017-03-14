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

    QImage scaledFrame = frame.scaled(ui->label_processedFrame->size());
    ui->label_processedFrame->setPixmap(QPixmap::fromImage(scaledFrame));
    ui->label_processedFrame->show();

}

void MainWindow::onButton_dispProcessResults()
{

     workThread->isStopped = (0xFF ^ workThread->isStopped) & 0x01;

     if(0x01 == workThread->isStopped)
         ui->pushButton_beginProc->setText("Begin Proc");
     else
         ui->pushButton_beginProc->setText("Pause");
    if(workThread->isRunning())return;
    else
        workThread->start();\



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
    ui->label_realTimeFrame->show();
}
