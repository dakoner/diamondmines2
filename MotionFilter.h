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
#include "box2dengine.h"

class MotionFilter : public QObject
 {
    Q_OBJECT

public:
    MotionFilter(QGraphicsView* view, QtBox2DEngine* engine, QGraphicsPolygonItem* pi, const std::set<QGraphicsItem*> &stalagtites_set, const std::set<QGraphicsItem*> &stalagmites_set):
        _view(view), _engine(engine), _pi(pi), _stalagtites_set(stalagtites_set), _stalagmites_set(stalagmites_set) { }

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

         QList<b2Body*> bodyList = _engine->bodyList();
         QList<b2Body*>::iterator it;
         for (it = bodyList.begin(); it != bodyList.end(); ++it) {
             if ((*it)->GetType() == b2BodyType(2)) {
                 std::cout << (*it)->GetPosition().x << " " << (*it)->GetPosition().y << std::endl;
             }
         }

     }

     int collide() {
        QList<QGraphicsItem* > ci = _view->scene()->collidingItems(_pi);
        for(int i = 0; i < ci.size(); ++i) {
            if (_stalagtites_set.find(ci[i]) != _stalagtites_set.end()) {
                std::cout << "Hit a stalagtite." << std::endl;
            }
            if (_stalagmites_set.find(ci[i]) != _stalagmites_set.end()) {
                std::cout << "Hit a stalagmite." << std::endl;
            }
        }
        return ci.size();
    }

private:
//


     void moveLeft(void) {
         // ensure not off left edge
         QPointF p = _pi->pos();
         if (!collide()) {
         //if (p2.x() > 0) {
             p.setX(p.x() - 10);
         } else {

             std::cout << "at left edge" << std::endl;
         }
         _pi->setPos(p);

         _view->centerOn(_pi->x(), _view->height()/2);

         log();
     }
     void moveRight(void) {
         // ensure not off right edge
         QPointF p = _pi->pos();
         if (!collide()) {
         //if (p2.x() < _view->viewport()->width() - 20) {
             p.setX(p.x() + 10);
         } else {
             std::cout << "at right edge" << std::endl;
         }
         _pi->setPos(p);

         _view->centerOn(_pi->x(), _view->height()/2);
         log();
     }

     // The top and bottom edges
     void moveUp(void) {
         QPointF p = _pi->pos();
         if (!collide()) {
         //if (p2.y() > 0) {
            p.setY(p.y() - 10);
         } else {
             std::cout << "at top edge" << std::endl;
         }
         _pi->setPos(p);

         _view->centerOn(_pi->x(), _view->height()/2);
         log();
     }
     void moveDown(void) {
         QPointF p = _pi->pos();
         if (!collide()) {
         //if (p2.y() < _view->viewport()->height() - 20) {
            p.setY(p.y() + 10);
         } else {
             std::cout << "at bottom edge" << std::endl;
         }
         _pi->setPos(p);
         _view->centerOn(_pi->x(), _view->height()/2);
         log();
     }

     QGraphicsView* _view;
     QtBox2DEngine* _engine;
     QGraphicsPolygonItem* _pi;
     std::set<QGraphicsItem*> _stalagtites_set;
     std::set<QGraphicsItem*> _stalagmites_set;
};


#endif // MOTIONFILTER_H
