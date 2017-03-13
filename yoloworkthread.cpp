#include<yoloworkthread.h>
extern char INPUT_DATA_FILE[];
extern char INPUT_CFG_FILE[];
extern char INPUT_WEIGHTS_FILE[];
extern char VIDEO_FILE[];

YoloWorkThread::YoloWorkThread()
{
    p = NULL;
    vcap = NULL;
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
    Mat image;
    //QLabel label("Detection Demo",this);


    box* boxes = 0;
    int width = 1280;
    int height = 720;
    int i = 0;


    while(vcap->isOpened()) //OR ingterruption.
    {
        vcap->read(image);

        //image = imread(INPUT_IMAGE_FILE, IMREAD_COLOR);
        cout<<i<<endl;

        if( image.empty() )
        {

            printf("Could not capture the image\n");
            break;
        }
        if(i == 0)
        {
           // vcap->read(image);
            width = image.cols;
            height = image.rows;
            i++;
        }
        else
        {

            i++;
            //cout<<image.channels()<<endl;
            // imshow("T",image);

             cvtColor(image, image, CV_BGR2RGB);

//            // waitKey();
            QImage imageQ((unsigned char*)image.data,width,height,width*3,QImage::Format_RGB888);



            // Process the image
            printf("Image data = %p, w = %d, h = %d\n", image.data, image.size().width, image.size().height);
            arapahoImage.bgr = image.data;
            arapahoImage.w = image.size().width;
            arapahoImage.h = image.size().height;
            arapahoImage.channels = 3;
            // Using expectedW/H, can optimise scaling using HW in platforms where available

            int numObjects = 0;
            cout<<"OK"<<endl;
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
//                for(int i = 0;i<numObjects;i++)
//                {
//                    cv::rectangle(image);
//                }

            }

//            //label.setPixmap(QPixmap::fromImage(imageQ));
//           // label.resize(label.pixmap()->size());
//           // this->resize(label.size());

//           // label.show();
//           // this->show();
//            //waitKey(1);
           if(boxes)
           {
               delete[] boxes;
               boxes = NULL;//Almost forget!
           }

           emit frameProcessed(imageQ);
        }


    }


clean_exit:
    printf("Exiting...\n");
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
