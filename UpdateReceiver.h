#ifndef UPDATERECEIVER_H
#define UPDATERECEIVER_H
#include <iostream>
#include <QObject>
#include <QGraphicsPolygonItem>
#include "box2dengine.h"
#include <QGraphicsView>


class UpdateReceiver : public QObject {
Q_OBJECT
public:
    UpdateReceiver(QtBox2DEngine* engine, QGraphicsView* view, QGraphicsPolygonItem* pi): _engine(engine), _view(view), _pi(pi) {
    }


public slots:
    void update() {
        QList<b2Body*> bodyList = _engine->bodyList();
        QList<b2Body*>::iterator it;
        for (it = bodyList.begin(); it != bodyList.end(); ++it) {
            if ((*it)->GetType() == b2BodyType(2)) {
                std::cout << (*it)->GetPosition().x << " " << (*it)->GetPosition().y << std::endl;
                _pi->setPos((*it)->GetPosition().x, (*it)->GetPosition().y);
                _view->centerOn(_pi->x(), _view->height()/2);

            }
        }
    }
private:
    QtBox2DEngine* _engine;
    QGraphicsView* _view;

    QGraphicsPolygonItem* _pi;

};

#endif // UPDATERECEIVER_H
