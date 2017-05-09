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
    //    myProcessedView->setGeometry(QRect(0, 52, DISPLAY_WIDTH+2, DISPLAY_HEIGHT+2));

    myRealTimeView->setGeometry(QRect(0, 52, DISPLAY_WIDTH+2, DISPLAY_HEIGHT+2));


    server = new QTcpServer();
    connect(ui->pushButton_listenConnection,&QPushButton::clicked,this,&MainWindow::onClickListenBtn);

    connect(this,&MainWindow::startrecording,dispThread,&RealTimeDisplayThread::startrecording);
    connect(this,&MainWindow::stoprecording,dispThread,&RealTimeDisplayThread::stoprecording);
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
void MainWindow::readClient()
{
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());

    QString str = socket->readAll();
    int desc =  ((int)(socket->socketDescriptor()));
    qDebug("Received Something from: %d",desc);

    if(QString::compare(str,QString("STARTRECORD")) == 0)//<<std::endl;
    {
        //TODO: Start Recording
        qDebug("Start Recording");
        emit startrecording();


    }
    else if(QString::compare(str,QString("STOPRECORD")) == 0)
    {
        //TODO: Stop Recording
        qDebug("Stop Recording");
        emit stoprecording();
    }
    else
    {
        //TODO:Coordinate Transfering.
        int lu_x = 0;
        int lu_y = 0;
        int rb_x = DISPLAY_WIDTH;
        int rb_y = DISPLAY_HEIGHT;


        if(str.length()<42)
            return ;
        lu_x = str.mid(0,3).toInt();
        lu_y = str.mid(3,3).toInt();

        rb_x = str.mid(18,3).toInt();
        rb_y = str.mid(21,3).toInt();





        QRect roi(QPoint(lu_x,lu_y),QPoint(rb_x,rb_y));
        emit sendROI(roi);
    }

    cout<<"Msg: "<< str.toStdString()<<endl;
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
void MainWindow::onClickListenBtn()
{
    server->listen(QHostAddress::Any, 9527);
    connect(server, &QTcpServer::newConnection, this, &MainWindow::acceptConnection);
}

void MainWindow::acceptConnection()
{
    qDebug("Incoming");
    QTcpSocket * clientCurrentConnection = server->nextPendingConnection();


    clientConnectionList.push_back(clientCurrentConnection);
    //        clientCurrentConnection->setSocketDescriptor();


    connect(clientCurrentConnection, &QTcpSocket::readyRead, this, &MainWindow::readClient);
    qDebug("Connet Receiving .");//clientConnectionList.size());
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
    //    myRealTimeView->setVisible(false);
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
