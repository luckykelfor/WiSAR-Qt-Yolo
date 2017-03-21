#include<candidateview.h>



CandidateGraphicsView::CandidateGraphicsView(QWidget *parent,QRectF posRect) :
   QGraphicsView(parent)
{

   scale_iter = 0;
   //setBackgroundBrush(QPixmap("../YoloDisplay/test.png"));//其实就是设置场景的背景
   scene = new QGraphicsScene(this);

   scene->setSceneRect(posRect);//(0,0,800,800);
   QGraphicsRectItem item(posRect);//(0,0,800,800);
   item.setBrush(Qt::red);
   scene->addItem(&item);
   setScene(scene);
   setDragMode(QGraphicsView::ScrollHandDrag);//手型拖动
  // setGeometry()
}

CandidateGraphicsView::~CandidateGraphicsView()
{
    if(scene)
        delete scene;
    scene = NULL;
}
void CandidateGraphicsView::wheelEvent(QWheelEvent*event)  //滚轮事件
{
   // qDebug("DDD");
   // if(scene->s)



   if(event->delta() < 0 && scale_iter >0)  //如果鼠标滚轮远离使用者，则delta()返回正值
   {
       scale_iter -- ;
       scale(0.80,0.80);  //视图缩放
   }
   else if(event->delta()> 0 && scale_iter>=0)
   {
       scale_iter ++ ;
       scale(1.25,1.25);
   }
}


void CandidateGraphicsView::mousePressEvent(QMouseEvent*event)
{
      QGraphicsView::mousePressEvent(event);
}
