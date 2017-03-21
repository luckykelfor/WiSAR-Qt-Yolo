#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSize>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    workThread = new YoloWorkThread();
    dispThread = new RealTimeDisplayThread();

    myView  = new CandidateGraphicsView(this,QRectF(720,0,720,420));
    //this->keyPressEvent();
    myView->installEventFilter(this);

    //myView->resize(QSize(725,425));
    myView->setGeometry(QRect(720, 52, 722, 422));
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
   // ui->label_processedFrame->setPixmap(QPixmap::fromImage(scaledFrame));
   // ui->label_processedFrame->show();
    myView->setBackgroundBrush(QPixmap::fromImage(scaledFrame));

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

    QImage scaledFrame = frame.scaled(QSize(720,420));
    ui->label_realTimeFrame->setPixmap(QPixmap::fromImage(scaledFrame));
    ui->label_realTimeFrame->show();
    //myView->setBackgroundBrush(QPixmap::fromImage(scaledFrame));

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
