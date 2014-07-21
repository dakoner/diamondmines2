#ifndef MOTIONFILTER_H
#define MOTIONFILTER_H
#include <iostream>

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QEvent>
#include <QGraphicsView>
#include <QKeyEvent>

class MotionFilter : public QObject
 {
    Q_OBJECT

public:
    explicit MotionFilter(QGraphicsView* view, QGraphicsPolygonItem* pi): _view(view), _pi(pi) { }

 protected:
     bool eventFilter(QObject *obj, QEvent *event) {
         if (event->type() == QEvent::KeyPress) {
             QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
             switch (keyEvent->key()) {
                    case Qt::Key_Left:
                         moveLeft();
                         return true; break;
                    case Qt::Key_Right:
                         moveRight();
                         return true; break;
                    case Qt::Key_Up:
                         moveUp();
                         return true; break;
                   case Qt::Key_Down:
                         moveDown();
                         return true; break;
             }
             return true;
         } else {
             // standard event processing
             return QObject::eventFilter(obj, event);
         }
     }

private:

     void moveLeft(void) {
         // ensure not off left edge
         QPointF p = _pi->pos();
         p.setX(p.x() - 1);
         _pi->setPos(p);
         _view->centerOn(_pi);
     }
     void moveRight(void) {
         // ensure not off right edge
         QPointF p = _pi->pos();
         p.setX(p.x() + 1);
         _pi->setPos(p);
         _view->centerOn(_pi);
     }

     void moveUp(void) {
         QPointF p = _pi->pos();
         p.setY(p.y() - 0.5);
         _pi->setPos(p);
     }
     void moveDown(void) {
         QPointF p = _pi->pos();
         p.setY(p.y() + 0.5);
         _pi->setPos(p);
     }

     QGraphicsView* _view;
     QGraphicsPolygonItem* _pi;
};


#endif // MOTIONFILTER_H
