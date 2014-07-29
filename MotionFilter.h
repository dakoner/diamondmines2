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
    MotionFilter(QGraphicsView* view, QtBox2DEngine* engine, b2Body *ship_body, QGraphicsPolygonItem* pi):
        _view(view), _engine(engine), _ship_body(ship_body), _pi(pi) { }

protected:
    bool eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            switch (keyEvent->key()) {
            case Qt::Key_Left:
                _ship_body->ApplyForce(b2Vec2(-1., 0), _ship_body->GetWorldCenter(), true);
                break;
            case Qt::Key_Right:
                _ship_body->ApplyForce(b2Vec2(1., 0), _ship_body->GetWorldCenter(), true);
                break;
            case Qt::Key_Up:
                _ship_body->ApplyForce(b2Vec2(0, 1.), _ship_body->GetWorldCenter(), true);
                break;
            case Qt::Key_Down:
                _ship_body->ApplyForce(b2Vec2(0, -1.), _ship_body->GetWorldCenter(), true);
                break;
            }
        } else {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
        return true;
    }

private:
    QGraphicsView* _view;
    QtBox2DEngine* _engine;
    b2Body* _ship_body;
    QGraphicsPolygonItem* _pi;
    std::set<QGraphicsItem*> _stalagtites_set;
    std::set<QGraphicsItem*> _stalagmites_set;
};

#endif // MOTIONFILTER_H
