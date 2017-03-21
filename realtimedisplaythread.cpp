#include "realtimedisplaythread.h"

Mat currentFrameCopy;

extern char VIDEO_FILE[];
RealTimeDisplayThread::RealTimeDisplayThread()
{

    isStopped = false;
    vcap = NULL;
}
RealTimeDisplayThread::~RealTimeDisplayThread()
{
    if(vcap)delete vcap;
    vcap = NULL;

}
void RealTimeDisplayThread::run()
{

    if(!setupWebcam(VIDEO_FILE))
    {
        cout<<"Cannot open Camera.\n";
        return;
    }
    float FPS =  vcap->get(CV_CAP_PROP_FPS);
    Mat currentFrame;
    while(!isStopped && vcap->isOpened())
    {

        vcap->read(currentFrame);
        currentFrame.copyTo(::currentFrameCopy);
        cvtColor(currentFrame, currentFrame, CV_BGR2RGB);
        QImage imageQ((unsigned char*)currentFrame.data,currentFrame.cols,currentFrame.rows,currentFrame.cols*3,QImage::Format_RGB888);
        emit transmitCurrentFrame(imageQ);
        msleep(1000/FPS);
    }
}

bool RealTimeDisplayThread::setupWebcam(const char videoFilePath[])
{
    this->vcap = new VideoCapture();
    this->vcap->open(videoFilePath);
    return vcap->isOpened();
}
//bool RealTimeDisplayThread::setupWebcam(const int webcamID)
//{
//    this->vcap = new VideoCapture();
//    this->vcap->open(webcamID);
//    return vcap->isOpened();
//}
