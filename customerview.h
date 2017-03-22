#ifndef CANDIDATEVIEW_H
#define CANDIDATEVIEW_H
#include <QtGui>
#include<QGraphicsView>
#include<QGraphicsRectItem>
#include<QRectF>

class CustomerGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    CustomerGraphicsView(QWidget*,QRectF);
    ~CustomerGraphicsView();
    QGraphicsScene *scene;
//protected:
   void wheelEvent(QWheelEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   //void destroyed
signals:
   void setROI(QRect);
private:
   int scale_iter;


};


#endif // CANDIDATEVIEW_H
