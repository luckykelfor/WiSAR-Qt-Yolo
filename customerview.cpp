#include<customerview.h>



CustomerGraphicsView::CustomerGraphicsView(QWidget *parent,QRectF posRect) :
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

CustomerGraphicsView::~CustomerGraphicsView()
{
    if(scene)
        delete scene;
    scene = NULL;
}
void CustomerGraphicsView::wheelEvent(QWheelEvent*event)  //滚轮事件
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

   QPolygonF res =  mapToScene(viewport()->rect());
   QRect roiRect(QPoint((int)(res[0].x()),(int)(res[0].y())),QPoint((int)(res[2].x()),(int)(res[2].y())));
   emit setROI(roiRect);
 //  qDebug("[%f] [%f] [%f] [%f]",res[0].x(),res[0].y(),res[3].x(),res[3].y());//top,bottom);
  // qDebug("[%f] [%f] [%f] [%f]",res[1].x(),res[1].y(),res[2].x(),res[2].y());//top,bottom);
}


void CustomerGraphicsView::mousePressEvent(QMouseEvent*event)
{
      QGraphicsView::mousePressEvent(event);
}
void CustomerGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QPolygonF res =  mapToScene(viewport()->rect());
    QRect roiRect(QPoint((int)(res[0].x()),(int)(res[0].y())),QPoint((int)(res[2].x()),(int)(res[2].y())));
    emit setROI(roiRect);
    event->accept();
}
