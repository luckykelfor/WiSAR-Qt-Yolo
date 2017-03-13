#ifndef YOLOWORKTHREAD_H
#define YOLOWORKTHREAD_H



#include<QThread>
#include<QObject>
#include<QImage>
#include<opencv2/opencv.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include "arapaho.hpp"

using namespace cv;
using namespace std;
class YoloWorkThread :public QThread
{
    Q_OBJECT
public:
    YoloWorkThread();
    ~YoloWorkThread();
    int isStopped;
    //cv::Mat *currentFrame;
    //void* w;
   // void setWindow(void  *);

    ArapahoV2 * p ;//= new ArapahoV2();
    cv::VideoCapture *vcap ;//= new VideoCapture();


    bool setupArapahoV2();
    bool detectOnWebCam();
    bool setupWebCam();
    bool fileExists(const char *file);

signals:
    void frameProcessed( const QImage& frame);
protected:Mat currentFrame;
    void run();

};


#endif // YOLOWORKTHREAD_H
