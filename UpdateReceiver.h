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
    UpdateReceiver(QtBox2DEngine* engine, QGraphicsView* view, QGraphicsPolygonItem* pi, b2Body* ship_body): _engine(engine), _view(view), _pi(pi), _ship_body(ship_body) {
    }


public slots:
    void update() {


                _pi->setPos(_ship_body->GetPosition().x, -_ship_body->GetPosition().y);
                _view->centerOn(_pi->x(), _view->height()/2);
                b2Vec2 lv = _ship_body->GetLinearVelocity();
                _ship_body->SetLinearVelocity(b2Vec2(lv.x*.995, lv.y*.995));

            }
        //for (b2Contact* contact = world->GetContactList(); contact; contact = contact->GetNext())
         //     std::cout << "contact: " << contact->GetChildIndexA() << " " << contact->GetChildIndexB() << std::endl;
        //std::cout << std::endl;
private:
    QtBox2DEngine* _engine;
    QGraphicsView* _view;

    QGraphicsPolygonItem* _pi;
    b2Body* _ship_body;


};

#endif // UPDATERECEIVER_H
