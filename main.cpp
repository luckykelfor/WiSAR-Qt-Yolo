#include "mainwindow.h"
#include <QApplication>

#include<QPushButton>


using namespace std;
using namespace cv;

int WEBCAM_ID = 0;
char VIDEO_FILE[] = "../YoloDisplay/dd.mp4";
char INPUT_DATA_FILE[]    = "../YoloDisplay/cfg/coco.data";
char INPUT_CFG_FILE[]     = "../YoloDisplay/cfg/yolo.cfg";
char INPUT_WEIGHTS_FILE[] = "../YoloDisplay/tiny-yolo-voc.weights";
char INPUT_IMAGE_FILE[]   = "../YoloDisplay/input.jpg";
int WEBCAM_WIDTH = 1280;
int WEBCAM_HEIGHT = 720;

bool needProcess = false;
//
// Some utility functions
//



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Mat image = imread("test.png");
    if(image.empty())
    {
        cout<<"Read Image Error!\n";
        return -1;
    }
    cvtColor(image,image,CV_BGR2RGB);
    cout<<image.channels()<<endl;

    // QPushButton button("Begin Detection");

    //   QObject::connect(&button,&QPushButton::clicked,&w,&MainWindow::onButtonBeginDetection);
    //    QImage *imageQ = new QImage((unsigned char*)image.data,image.cols,image.rows,image.cols*3,QImage::Format_RGB888);
    //    QLabel *label = new QLabel("Test Image",&w);


    //button.show();
    w.show();


    return a.exec();
}


