#include <QApplication>
#include <QtGui>
#include <QGraphicsLineItem>
#include <vector>
#include <QPolygonF>
#include <QScrollBar>
#include <set>
#include <Box2D/Box2D.h>
#include "UpdateReceiver.h"
#include "MotionFilter.h"
#include "box2dengine.h"


std::vector<int> stalagtites{0,0,0,0,1,1,2,2,1,1,1,3,1,0,0,1,4,0,0,1,0,0,0,0,2,2,2,2,2,3,3,4,4,3,0,0,0,0,0,1};
std::vector<int> stalagmites{0,0,0,0,2,2,2,2,2,3,3,4,4,3,0,0,0,0,0,1,0,0,0,0,1,1,2,2,1,1,1,3,1,0,0,1,4,0,0,1};

float scale = 72.;

class MyContactListener: public b2ContactListener {
  void BeginContact(b2Contact* contact) {
      std::cout << "Begin contact: " << contact->GetFixtureA() << " " << contact->GetFixtureB() << std::endl;
  }
  void EndContact(b2Contact* contact) {
      std::cout << "End contact: " << contact->GetFixtureA() << " " << contact->GetFixtureB() << std::endl;
  }
};

void addLine(b2Body* body, QGraphicsScene *scene, QtBox2DEngine* engine, double x1, double y1, double x2, double y2) {
    QPen p;
    p.setWidth(0);
    scene->addLine(QLineF(x1, y1, x2, y2), p);
    b2EdgeShape *edge = new b2EdgeShape;
    edge->Set(b2Vec2(x1,-y1), b2Vec2(x2,-y2));
    b2Fixture* fixture = engine->createFixture(body, edge);
    fixture->SetRestitution(1);
    fixture->SetFriction(0);
}


void addChain(b2Body* body, QGraphicsScene *scene, QtBox2DEngine* engine, const QList<QPointF>& points) {
    QPen p;
    p.setWidth(0);

    b2Vec2* vertices = new b2Vec2[points.size()-1];
    for (int i=0; i < points.size()-1; ++i) {

        scene->addLine(points[i].x(), points[i].y(), points[i+1].x(), points[i+1].y(), p);

        vertices[i] = b2Vec2(points[i].x(), -points[i].y());
    }
    b2ChainShape *chain = new b2ChainShape;
    chain->CreateChain(vertices, points.size()-1);
    b2Fixture* fixture = engine->createFixture(body, chain);\
    fixture->SetRestitution(1);
    fixture->SetFriction(0);
}

QGraphicsPolygonItem* addPolygon(b2Body* body, QGraphicsScene* scene, QtBox2DEngine* engine, const QPolygonF& polygon) {
    QPen p;
    p.setWidth(0);
    QGraphicsPolygonItem* pi = scene->addPolygon(polygon, p);

    int count = polygon.count();
    b2Vec2* vertices = new b2Vec2[count];

    for (int i = 0; i < count; ++i) {
        QPointF p = polygon.at(i);
        vertices[i].Set(p.x(), -p.y());
    }
    b2PolygonShape *polyshape = new b2PolygonShape;
    polyshape->Set(vertices, count);



    b2Fixture* fixture = engine->createFixture(body, polyshape);
    fixture->SetRestitution(1);
    fixture->SetFriction(0);

    return pi;
}



int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, true);
    QGraphicsScene *scene = new QGraphicsScene();

    QPen p;
    p.setWidth(0);

    QtBox2DEngine engine;
    engine.setGravity(0);


    b2Body* stalagtites_body = engine.createBody(b2_staticBody, 0, 0, 0, false);
    {
        QList<QPointF> points;
        for (unsigned int i = 0; i < stalagtites.size(); ++i)
            points.push_back(QPointF(i,stalagtites[i]));
        addChain(stalagtites_body, scene, &engine,  points);
    }

    b2Body* stalagmites_body = engine.createBody(b2_staticBody,0 , 0, false);
    {
        QList<QPointF> points;
        for (unsigned int i = 0; i < stalagmites.size(); ++i)
            points.push_back(QPointF(i,10-stalagmites[i]));
        addChain(stalagmites_body, scene, &engine, points);
    }


    addLine(stalagtites_body, scene, &engine,0,0,0,10);
    addLine(stalagtites_body, scene, &engine,stalagtites.size()-1,0,stalagtites.size()-1,10);


    QPolygonF polygon;
    polygon << QPointF(0, 0) << QPointF(.1, .1 )<< QPointF(0.2,0.025)  << QPointF(.3333,.15) << QPointF(.2,.275)<< QPointF(.1,.2) << QPointF(0,.3333) ;


    b2Body* ship_body = engine.createBody(b2_dynamicBody, 2, -5, 0, false);
    ship_body->SetFixedRotation(true);
    QGraphicsPolygonItem* pi = addPolygon(ship_body, scene, &engine, polygon);
    //pi->setAcceptTouchEvents(true);

    QGraphicsView view;
    view.setMouseTracking(true);
    view.resize(1280,720);
    view.scale(scale, scale);
    view.setScene(scene);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view.viewport()->setAttribute(Qt::WA_AcceptTouchEvents);
    MotionFilter* motion_filter = new MotionFilter(ship_body);
    SceneMotionFilter* scene_motion_filter = new SceneMotionFilter(ship_body);

    // scene event filter required to get mouse events
    scene->installEventFilter(scene_motion_filter);
    view.installEventFilter(motion_filter);

    engine.setGravity(0);
    engine.start();

    engine.setInterval(60);
    UpdateReceiver update_receiver(&view,  pi, ship_body);
    QObject::connect(&engine, SIGNAL(step()), &update_receiver, SLOT(update()));
    MyContactListener myContactListenerInstance;
    engine.setContactListener(&myContactListenerInstance);

    view.show();
    return app.exec();
}
