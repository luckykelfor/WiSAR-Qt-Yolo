#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSize>
#include<QMessageBox>

extern int WEBCAM_WIDTH;
extern int WEBCAM_HEIGHT;
extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    workThread = new YoloWorkThread();
    dispThread = new RealTimeDisplayThread();

    myProcessedView  = new CustomerGraphicsView(this,QRectF(DISPLAY_WIDTH,0,DISPLAY_WIDTH,DISPLAY_HEIGHT));
    myRealTimeView  = new CustomerGraphicsView(this,QRectF(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT));


    //Need to adjust accordingly.
    myProcessedView->setGeometry(QRect(DISPLAY_WIDTH, 52, DISPLAY_WIDTH+2, DISPLAY_HEIGHT+2));
    myRealTimeView->setGeometry(QRect(0, 52, DISPLAY_WIDTH+2, DISPLAY_HEIGHT+2));

    connect(dispThread,&RealTimeDisplayThread::transmitCurrentFrame,this,&MainWindow::dispRealTimeView);
    connect(ui->pushButton_realTimeView,&QPushButton::clicked,this,&MainWindow::onButton_dispRealTimeView);
    connect(ui->pushButton_beginProc,&QPushButton::clicked,this,&MainWindow::onButton_dispProcessResults);
    connect(workThread,&YoloWorkThread::frameProcessed,this,&MainWindow::dispProcessResults);
    connect(myRealTimeView,&CustomerGraphicsView::setROI,dispThread,&RealTimeDisplayThread::onScalePosChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(workThread)
        delete workThread;
    if(dispThread)
        delete dispThread;
    if(myProcessedView)
        delete myProcessedView;
    if(myRealTimeView)
        delete myRealTimeView;
}

void MainWindow::dispProcessResults(const QImage & frame)
{

    QImage scaledFrame = frame.scaled(QSize(DISPLAY_WIDTH,DISPLAY_HEIGHT));
   // ui->label_processedFrame->setPixmap(QPixmap::fromImage(scaledFrame));
   // ui->label_processedFrame->show();
    myProcessedView->setBackgroundBrush(QPixmap::fromImage(scaledFrame));

}

void MainWindow::onButton_dispProcessResults()
{

     workThread->isPaused = (0xFF ^ workThread->isPaused) & 0x01;

     if(0x01 == workThread->isPaused)
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

    QImage scaledFrame = frame.scaled(QSize(DISPLAY_WIDTH,DISPLAY_HEIGHT));
    //ui->label_realTimeFrame->setPixmap(QPixmap::fromImage(scaledFrame));
   // ui->label_realTimeFrame->show();
    myRealTimeView->setBackgroundBrush(QPixmap::fromImage(scaledFrame));
    //myRealTimeView->scene->addPixmap(QPixmap::fromImage(scaledFrame));

}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (workThread->isRunning() || dispThread->isRunning()) {
        bool exit = QMessageBox::question(this,
                                      tr("Quit"),
                                      tr("Thread is still running. Are you sure to quit this application?"),
                                      QMessageBox::Yes | QMessageBox::No,
                                      QMessageBox::No) == QMessageBox::Yes;
        if (exit) {
            workThread->isPaused = 0x01;
            workThread->isStopped = true;
            workThread->quit();
            workThread->wait();
            dispThread->isStopped = true;
            dispThread->quit();
            dispThread->wait();
            qDebug("Exiting threads successfully.");
            event->accept();

        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}
