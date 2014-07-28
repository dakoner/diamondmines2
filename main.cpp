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

void addLine(b2Body* body, QGraphicsScene *scene, QtBox2DEngine* engine, double x1, double y1, double x2, double y2) {
    QPen p;
    p.setWidth(0);
    scene->addLine(QLineF(x1, y1, x2, y2), p);
    b2EdgeShape *edge = new b2EdgeShape;
    edge->Set(b2Vec2(x1,-y1), b2Vec2(x2,-y2));
    engine->createFixture(body, edge);

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
    engine->createFixture(body, chain);
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


    engine->createFixture(body, polyshape);
    return pi;
}



int main(int argc, char** argv) {
    QApplication app(argc, argv);

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


    b2Body* ship_body = engine.createBody(b2_dynamicBody, 0, -2, 0, false);
    QGraphicsPolygonItem* pi = addPolygon(ship_body, scene, &engine, polygon);

    QGraphicsView view;
    view.resize(1280,720);
    view.scale(scale, scale);
    view.setScene(scene);
    view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    MotionFilter* motion_filter = new MotionFilter(&view, &engine, ship_body, pi);

    view.installEventFilter(motion_filter);

    engine.setGravity(0);
    engine.start();

    engine.setInterval(100);
    UpdateReceiver update_receiver(&engine, &view,  pi, ship_body);
    update_receiver.connect(&engine, SIGNAL(step()), &update_receiver, SLOT(update()));

    view.show();
    return app.exec();
}
