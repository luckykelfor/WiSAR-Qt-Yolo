#include "realtimedisplaythread.h"
extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;

Mat currentFrameCopy;
extern char VIDEO_FILE[];
RealTimeDisplayThread::RealTimeDisplayThread():roiRect(QRect(0,0,720,420))
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

    int count = 0;
    int current_W;
    int current_H;
    while(!isStopped && vcap->isOpened())
    {


        vcap->read(currentFrame);
        if(0 == count)
        {
            current_W = currentFrame.cols;
            current_H = currentFrame.rows;
            count = 1;
            cout<<current_W<<" "<<current_H<<endl;
            continue;
        }

        cv::Rect r((int)(current_W*roiRect.x()/(float)DISPLAY_WIDTH),(int)(current_H*roiRect.y()/(float)DISPLAY_HEIGHT),(int)(current_W*roiRect.width()/(float)DISPLAY_WIDTH),(int)(current_H*roiRect.height()/(float)DISPLAY_HEIGHT));
        //if(r.x>current_W)r.x = current_W;
        if(r.x<0)r.x = 0;
        if(r.y<0)r.y = 0;
        if(r.x+ r.width > current_W)r.width = current_W - r.x;
        if(r.y+ r.height > current_H)r.height = current_H - r.y;
        Mat ROI(currentFrame,r);//int()(roiRect.x(),roiRect.y(),roiRect.width(),roiRect.height()));
        ROI.copyTo(::currentFrameCopy);

        cvtColor(currentFrame, currentFrame, CV_BGR2RGB);
        QImage imageQ((unsigned char*)currentFrame.data,currentFrame.cols,currentFrame.rows,currentFrame.cols*3,QImage::Format_RGB888);
        emit transmitCurrentFrame(imageQ);
        msleep(35);
    }
}

bool RealTimeDisplayThread::setupWebcam(const char videoFilePath[])
{
    this->vcap = new VideoCapture();
    this->vcap->open(videoFilePath);
    return vcap->isOpened();
}
void RealTimeDisplayThread::onScalePosChanged(QRect roi)
{
    roiRect=roi;
}
