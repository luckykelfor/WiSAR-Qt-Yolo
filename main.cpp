#include "mainwindow.h"
#include <QApplication>

#include<QPushButton>

#include<customerview.h>
using namespace std;
using namespace cv;

int WEBCAM_ID = 0;
char VIDEO_FILE[] = "../YoloDisplay/DJI_0038.MOV";
char INPUT_DATA_FILE[]    = "../YoloDisplay/cfg/voc.data";//coco.data";
char INPUT_CFG_FILE[]     = "../YoloDisplay/cfg/yolo-voc.cfg";//yolo.cfg
char INPUT_WEIGHTS_FILE[] = "../YoloDisplay/yolo-voc_7000.weights";
char INPUT_IMAGE_FILE[]   = "../YoloDisplay/test.png";
int WEBCAM_WIDTH = 1280;
int WEBCAM_HEIGHT = 720;
int DISPLAY_WIDTH = 720;
int DISPLAY_HEIGHT=440;

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


    //CandidateGraphicsView myView(&w);

    w.show();


    return a.exec();
}


