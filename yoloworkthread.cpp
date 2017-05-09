#include<yoloworkthread.h>
extern char INPUT_DATA_FILE[];
extern char INPUT_CFG_FILE[];
extern char INPUT_WEIGHTS_FILE[];
extern char VIDEO_FILE[];
extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;
extern QMutex mutex;
extern Mat currentFrameCopy;
Mat frameInUse;
YoloWorkThread::YoloWorkThread():roiRect(QRect(0,0,DISPLAY_WIDTH,DISPLAY_HEIGHT))
{
    p = NULL;
    vcap = NULL;
    isPaused = 0x01;
    isStopped = false;
}
YoloWorkThread::~YoloWorkThread()
{
    if(p)delete p;
    if(vcap)delete vcap;
    p = NULL;
    vcap = NULL;
}
bool YoloWorkThread::fileExists(const char *file)
{
    struct stat st;
    if(!file) return false;
    int result = stat(file, &st);
    return (0 == result);
}
bool YoloWorkThread::setupArapahoV2()
{
    bool ret = false;
    int expectedW = 0, expectedH = 0;
    // Early exits
    if(!fileExists(INPUT_DATA_FILE) || !fileExists(INPUT_CFG_FILE) || !fileExists(INPUT_WEIGHTS_FILE))
    {
        printf("Setup failed as inputs files(.cfg OR .weights) do not exist or not readable!\n");
        return false;
    }
    // Create arapaho
    p = new ArapahoV2();
    if(!p) return false;

    // TODO - read from arapaho.cfg
    ArapahoV2Params ap;
    ap.datacfg = INPUT_DATA_FILE;
    ap.cfgfile = INPUT_CFG_FILE;
    ap.weightfile = INPUT_WEIGHTS_FILE;
    ap.nms = 0.4;
    ap.maxClasses = 2;

    // Always setup before detect
    ret = p->Setup(ap, expectedW, expectedH);
    if(false == ret)
    {
        printf("Setup failed!\n");

        if(p) delete p;
        p = NULL;
        return false;
    }

    return true;

}
bool YoloWorkThread::setupWebCam()
{
    vcap = new VideoCapture() ;
    if(!vcap)return false;
    vcap->set(CV_CAP_PROP_FRAME_WIDTH,1280);
    vcap->set(CV_CAP_PROP_FRAME_HEIGHT,720);
    vcap->open(VIDEO_FILE);
    if(!vcap->isOpened())
    {
        cout<<"Webcam setup failed.\n";
        return false;
    }
    return true;
}



bool YoloWorkThread::detectOnWebCam()
{
    // Setup image buffer here
    ArapahoV2ImageBuff arapahoImage;
    box* boxes = 0;
    int i = 0;


    while(!isStopped)
    {
        while(0x00 == this->isPaused)//(vcap->isOpened()) //OR ingterruption.
        {
            //        vcap->read(image);


            //        if( image.empty() )
            //        {

            //            printf("Could not capture the image\n");
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
            {



                mutex.lock();
                frameInUse = currentFrameCopy.clone();//DO NOT USE copyTO()function here!!!
                mutex.unlock();

                mutex.lock();







                cv::Rect roi(this->roiRect.x(),this->roiRect.y(),this->roiRect.width(),this->roiRect.height());
                mutex.unlock();
                Mat roi_(frameInUse, roi);
//                roi_tmp.copyTo(roi);



                               // Process the image
                printf("Image data = %p, w = %d, h = %d\n", roi_.data, roi_.cols, roi_.rows);
                arapahoImage.bgr = roi_.data;
                arapahoImage.w = roi_.cols;//.size().width;
                arapahoImage.h = roi_.rows;//.size().height;
                arapahoImage.channels = 3;
                // Using expectedW/H, can optimise scaling using HW in platforms where available

                int numObjects = 0;
                // Detect the objects in the image
                p->Detect(
                            arapahoImage,
                            0.24,
                            0.5,
                            numObjects);
                printf("Detected %d objects\n", numObjects);

                if(numObjects > 0)
                {
                    boxes = new box[numObjects];

                    if(!boxes)
                    {
                        //                    if(p) delete p;
                        //                    p = 0;
                        //                    return -1;
                        break;
                    }
                    p->GetBoxes(
                                boxes,
                                numObjects);
                    for(int i = 0; i<numObjects;i++)
                    {
                        // cv::Scalar()

                        int left  = (boxes[i].x-boxes[i].w/2)*roi_.cols;
                        int width = boxes[i].w*roi_.cols;
                        int top   = (boxes[i].y-boxes[i].h/2)*roi_.rows;
                        int height   = boxes[i].h*roi_.rows;
                        cv::rectangle(roi_,cv::Rect(left,top,width,height),Scalar(0,0,255),6);
                        cv::putText(roi_,"person",Point(left,top),0,1,Scalar(0,255,255),2);
                    }
                }

                if(boxes)
                {
                    delete[] boxes;
                    boxes = NULL;//Almost forget!
                }


                cvtColor(frameInUse, frameInUse, CV_BGR2RGB);

                QImage imageQ((unsigned char*)frameInUse.data,frameInUse.cols,frameInUse.rows,frameInUse.cols*3,QImage::Format_RGB888);
                emit frameProcessed(imageQ);
            }


        }
        msleep(100);
    }




    printf("Exiting Thread...\n");
    return true;
}

void YoloWorkThread::run()
{
    if(!setupArapahoV2())
    {
        cout<<"ArapahoV2 setup Failed.\n";
        return;
    }
    if(!setupWebCam())
    {
        cout<<"Webcam setup failed.\n";
        return;
    }
    detectOnWebCam();

}

void YoloWorkThread::onProposalROIReceived(QRect &roi)
{
    this->roiRect = roi;
}
