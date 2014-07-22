#ifndef MOTIONFILTER_H
#define MOTIONFILTER_H
#include <iostream>

#include <QGraphicsPolygonItem>
#include <QObject>
#include <QEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QScrollBar>
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

     void log() {
         QPointF p = _pi->pos();
         std::cout << _view->horizontalScrollBar()->minimum() << " " << _view->horizontalScrollBar()->value() << " " << _view->horizontalScrollBar()->maximum() << std::endl;
         std::cout << _view->viewport()->width() << " " << _view->viewport()->height() << std::endl;
         std::cout << p.x() << " " << p.y() << std::endl;
         QPointF p2 = _view->mapFromScene(p);
         std::cout << p2.x() << " " << p2.y() << std::endl;

     }

     void collide() {
        QList<QGraphicsItem* > ci = _view->scene()->collidingItems(_pi);
        for(int i = 0; i < ci.size(); ++i) {
            if (_stalagtites_set.find(ci[i]) != _stalagtites_set.end()) {
                std::cout << "Hit a stalagtite." << std::endl;
            }
            if (_stalagmites_set.find(ci[i]) != _stalagmites_set.end()) {
                std::cout << "Hit a stalagmite." << std::endl;
            }
        }

    }

private:

     void moveLeft(void) {
         // ensure not off left edge
         QPointF p = _pi->pos();
         QPointF p2 = _view->mapFromScene(p);
         if (p2.x() > 0) {
            p.setX(p.x() - 10);
            _pi->setPos(p);
            _view->centerOn(_pi->x(), _view->height()/2);
         } else {
             std::cout << "at left edge" << std::endl;
         }
         log();
         collide();
     }
     void moveRight(void) {
         // ensure not off right edge
         QPointF p = _pi->pos();
         QPointF p2 = _view->mapFromScene(p);
         if (p2.x() < _view->viewport()->width() - 20) {
             p.setX(p.x() + 10);
            _pi->setPos(p);
            _view->centerOn(_pi->x(), _view->height()/2);
         } else {
             std::cout << "at right edge" << std::endl;
         }
         log();
         collide();
     }

     void moveUp(void) {
         QPointF p = _pi->pos();
         p.setY(p.y() - 0.5);
         _pi->setPos(p);
         _view->centerOn(_pi->x(), _view->height()/2);
         log();
         collide();
     }
     void moveDown(void) {
         QPointF p = _pi->pos();
         p.setY(p.y() + 0.5);
         _pi->setPos(p);
         _view->centerOn(_pi->x(), _view->height()/2);
         log();
         collide();
     }

     QGraphicsView* _view;
     QGraphicsPolygonItem* _pi;
     std::set<QGraphicsItem*> _stalagtites_set;
     std::set<QGraphicsItem*> _stalagmites_set;
};


#endif // MOTIONFILTER_H
