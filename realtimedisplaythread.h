#ifndef REALTIMEDISPLAYTHREAD_H
#define REALTIMEDISPLAYTHREAD_H
#include<QThread>
#include<QImage>
#include<opencv2/opencv.hpp>
#include<opencv2/highgui.hpp>
#include<time.h>
#include<QMutex>
using namespace std;
using namespace cv;
class RealTimeDisplayThread:public QThread
{
    Q_OBJECT
public:
    RealTimeDisplayThread();
    ~RealTimeDisplayThread();

    bool isStopped;
//    Mat currentFrame;
    VideoCapture * vcap;

    bool setupWebcam(const char videoFilePath[]);
//    bool setupWebcam(const int camID);

    QRect roiRect;
    bool recording;
    void startrecording();
    void stoprecording();
public slots:
    void onScalePosChanged(QRect);
    signals:

    void transmitCurrentFrame(const QImage &);
protected:
   void run();

private:
   int recordflag;


};

#endif // REALTIMEDISPLAYTHREAD_H
