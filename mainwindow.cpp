#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    workThread = new YoloWorkThread();

   // pushB.show();
    connect(ui->pushButton_beginProc,&QPushButton::clicked,this,&MainWindow::onButtonBeginDetection);
    connect(workThread,&YoloWorkThread::frameProcessed,this,&MainWindow::drawPic);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(workThread)
        delete workThread;
}

void MainWindow::drawPic(const QImage & frame)
{

    printf("IN");
   // cv::Mat image = imread("./test.png");
   // cvtColor(image, image, CV_BGR2RGB);
    //QImage imageQ((unsigned char*)image.data,image.cols,image.rows,image.cols*3,QImage::Format_RGB888);

   // if(frame.data_ptr() == NULL)printf("NULL");
    ui->label_frame->setPixmap(QPixmap::fromImage(frame));

            //            label.resize(label.pixmap()->size()););
    ui->label_frame->resize(ui->label_frame->pixmap()->size());//size());
    ui->label_frame->show();
    ;//TODO
}

//bool MainWindow::fileExists(const char *file)
//{
//    struct stat st;
//    if(!file) return false;
//    int result = stat(file, &st);
//    return (0 == result);
//}

////
//// Main test wrapper for arapaho
////
//bool MainWindow::setupArapahoV2(ArapahoV2 * p)
//{
//    bool ret = false;
//    int expectedW = 0, expectedH = 0;


//    // Early exits
//    if(!fileExists(INPUT_DATA_FILE) || !fileExists(INPUT_CFG_FILE) || !fileExists(INPUT_WEIGHTS_FILE))
//    {
//        printf("Setup failed as inputs files(.cfg OR .weights) do not exist or not readable!\n");
//        return false;
//    }

//    // Create arapaho
//    //ArapahoV2* p = new ArapahoV2();
//    //    if(!p)
//    //    {
//    //        return NULL;
//    //    }

//    // TODO - read from arapaho.cfg
//    ArapahoV2Params ap;
//    ap.datacfg = INPUT_DATA_FILE;
//    ap.cfgfile = INPUT_CFG_FILE;
//    ap.weightfile = INPUT_WEIGHTS_FILE;
//    ap.nms = 0.4;
//    ap.maxClasses = 2;

//    // Always setup before detect
//    ret = p->Setup(ap, expectedW, expectedH);
//    if(false == ret)
//    {
//        printf("Setup failed!\n");
//        //        if(p) delete p;
//        //        p = 0;
//        return false;
//    }

//    return true;
//}
//// Steps below this, can be performed in a loop

//// loop
//// {
////    setup arapahoImage;
////    p->Detect(arapahoImage);
////    p->GetBoxes;
//// }
////
//bool MainWindow::setupWebCam(VideoCapture *vcap)
//{

//}
//bool MainWindow::detectOnWebCam(ArapahoV2 *p,tion()
//{VideoCapture * vcap)
//{
//    // Setup image buffer here
//    ArapahoV2ImageBuff arapahoImage;
//    Mat image;
//    QLabel label("Detection Demo",this);


//    box* boxes = 0;
//    int width = 1280;
//    int height = 720;
//    int i = 0;
//    while(vcap->isOpened()) //OR ingterruption.
//    {
//        vcap->read(image);

//        //image = imread(INPUT_IMAGE_FILE, IMREAD_COLOR);


//        if( image.empty() )
//        {

//            printf("Could not capture the image\n");
//            //            if(p) delete p;
//            //            p = 0;
//            //            return -1;
//            break;
//        }
//        if(i == 0)
//        {
//           // vcap->read(image);
//            width = image.cols;
//            height = image.rows;
//            i++;
//        }
//        else
//        {

//            i++;
//           // cout<<image.channels()<<endl;
//             cvtColor(image, image, CV_BGR2RGB);
//            QImage imageQ((unsigned char*)image.data,width,height,width*3,QImage::Format_RGB888);


//           // cout<<"OK"<<endl;
//            // Process the image
//            printf("Image data = %p, w = %d, h = %d\n", image.data, image.size().width, image.size().height);
//            arapahoImage.bgr = image.data;
//            arapahoImage.w = image.size().width;
//            arapahoImage.h = image.size().height;
//            arapahoImage.channels = 3;
//            // Using expectedW/H, can optimise scaling using HW in platforms where available

//            int numObjects = 0;

//            // Detect the objects in the image
//            p->Detect(
//                        arapahoImage,
//                        0.24,
//                        0.5,
//                        numObjects);
//            printf("Detected %d objects\n", numObjects);

//            if(numObjects > 0)
//            {
//                boxes = new box[numObjects];

//                if(!boxes)
//                {
//                    //                    if(p) delete p;
//                    //                    p = 0;
//                    //                    return -1;
//                    break;
//                }
//                p->GetBoxes(
//                            boxes,
//                            numObjects);
////                for(int i = 0;i<numObjects;i++)
////                {
////                    cv::rectangle(image);
////                }

//            }

//            label.setPixmap(QPixmap::fromImage(imageQ));
//            label.resize(label.pixmap()->size());
//            this->resize(label.size());

//            label.show();
//            this->show();
//            waitKey(1);
//           if(boxes)
//           {
//               delete[] boxes;
//               boxes = NULL;//Almost forget!
//           }
//        }


//    }


//clean_exit:

//    // Clear up things before exiting

//    //    if(p) delete p;
//    printf("Exiting...\n");
//    return true;
//}



void MainWindow::onButtonBeginDetection()
{


//    ArapahoV2 * p = new ArapahoV2();
//    VideoCapture *vcap = new VideoCapture();
    if(workThread->isRunning())return;
    else
        workThread->start();

}
