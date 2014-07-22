#ifndef MOTIONFILTER_H
#define MOTIONFILTER_H
#include <iostream>

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <set>

class MotionFilter : public QObject
 {
    Q_OBJECT

public:
    MotionFilter(QGraphicsView* view, QGraphicsPolygonItem* pi, const std::set<QGraphicsItem*> &stalagtites_set, const std::set<QGraphicsItem*> &stalagmites_set):
        _view(view), _pi(pi), _stalagtites_set(stalagtites_set), _stalagmites_set(stalagmites_set) { }

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

     void collide() {
        QList<QGraphicsItem* > ci = _view->scene()->collidingItems(_pi);
        std::cout << ci.size() << std::endl;
        for(int i = 0; i < ci.size(); ++i) {
            if (_stalagtites_set.find(ci[i]) != _stalagtites_set.end()) {
                std::cout << "Hit a stalagtite." << std::endl;
            }
            if (_stalagmites_set.find(ci[i]) != _stalagmites_set.end()) {
                std::cout << "Hit a stalagmite." << std::endl;
            }
        }
        std::cout << std::endl;
    }

private:

     void moveLeft(void) {
         // ensure not off left edge
         QPointF p = _pi->pos();
         p.setX(p.x() - 1);
         _pi->setPos(p);
         //_view->centerOn(_pi->x(), _view->height()/2);
         collide();
     }
     void moveRight(void) {
         // ensure not off right edge
         QPointF p = _pi->pos();
         p.setX(p.x() + 1);
         _pi->setPos(p);
         //_view->centerOn(_pi->x(), _view->height()/2);
         collide();
     }

     void moveUp(void) {
         QPointF p = _pi->pos();
         p.setY(p.y() - 0.5);
         _pi->setPos(p);
         //_view->centerOn(_pi->x(), _view->height()/2);

         collide();
     }
     void moveDown(void) {
         QPointF p = _pi->pos();
         p.setY(p.y() + 0.5);
         _pi->setPos(p);
         //_view->centerOn(_pi->x(), _view->height()/2);
         collide();
     }

     QGraphicsView* _view;
     QGraphicsPolygonItem* _pi;
     std::set<QGraphicsItem*> _stalagtites_set;
     std::set<QGraphicsItem*> _stalagmites_set;
};


#endif // MOTIONFILTER_H
