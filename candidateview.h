#ifndef CANDIDATEVIEW_H
#define CANDIDATEVIEW_H
#include <QtGui>
#include<QGraphicsView>
#include<QGraphicsRectItem>
#include<QRectF>

class CandidateGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    CandidateGraphicsView(QWidget*,QRectF);
    ~CandidateGraphicsView();
    QGraphicsScene *scene;
//protected:
   void wheelEvent(QWheelEvent *event);
   void mousePressEvent(QMouseEvent *event);
   //void destroyed
private:
   int scale_iter;

};


#endif // CANDIDATEVIEW_H
