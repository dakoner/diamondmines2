#include <QApplication>
#include <QtGui>
#include <QGraphicsLineItem>
#include <vector>
#include <QPolygonF>
#include <QScrollBar>
#include <set>
#include <Box2D/Box2D.h>

#include "MotionFilter.h"
#include "box2dengine.h"

std::vector<int> stalagtites{0,0,0,0,1,1,2,2,1,1,1,3,1,0,0,1,4,0,0,1,0,0,0,0,2,2,2,2,2,3,3,4,4,3,0,0,0,0,0,1};
std::vector<int> stalagmites{0,0,0,0,2,2,2,2,2,3,3,4,4,3,0,0,0,0,0,1,0,0,0,0,1,1,2,2,1,1,1,3,1,0,0,1,4,0,0,1};

void addLine(b2Body* body, QGraphicsScene *scene, QtBox2DEngine* engine, std::set<QGraphicsItem*>* set, double x1, double y1, double x2, double y2) {
    QPen p;
    p.setWidth(0);
    QGraphicsItem* item = scene->addLine(QLineF(x1, y1, x2, y2), p);
    item->setScale(72);
    set->insert(item);
    b2EdgeShape *edge = new b2EdgeShape;
    edge->Set(b2Vec2(x1,y1), b2Vec2(x2,y2));
    engine->createFixture(body, edge);

}


QGraphicsPolygonItem* addPolygon(b2Body* body, QGraphicsScene* scene, QtBox2DEngine* engine, const QPolygonF& polygon) {
    QPen p;
    p.setWidth(0);
    QGraphicsPolygonItem* pi = scene->addPolygon(polygon, p);
    pi->setScale(8);
    pi->setPos(720,360);

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
        engine.setGravity(-9);


        std::set<QGraphicsItem *> stalagtites_set;
        b2Body* stalagtites_body = engine.createBody(b2_staticBody, 0, 0, 0, false);
        for (unsigned int i = 0; i < stalagtites.size()-1; ++i)
            addLine(stalagtites_body, scene, &engine, &stalagtites_set, i,stalagtites[i],(i+1.),stalagtites[i+1]);
        addLine(stalagtites_body, scene, &engine, &stalagtites_set, stalagtites.size()-1, stalagtites[stalagtites.size()-1], stalagtites.size(), 0);
        addLine(stalagtites_body, scene, &engine, &stalagtites_set,0,0,0,10);



        std::set<QGraphicsItem *> stalagmites_set;
        b2Body* stalagmites_body = engine.createBody(b2_staticBody, 0, 0, 0, false);
        for (unsigned int i = 0; i < stalagmites.size()-1; ++i) {
            addLine(stalagmites_body, scene, &engine, &stalagmites_set, i,10-stalagmites[i],(i+1.),10-stalagmites[i+1]);
        }
        addLine(stalagmites_body, scene, &engine, &stalagmites_set, stalagmites.size()-1, 10-stalagmites[stalagmites.size()-1], stalagmites.size(), 10);
        addLine(stalagmites_body, scene, &engine, &stalagmites_set, stalagmites.size(),0,stalagmites.size(),10);

        QPolygonF polygon;
        polygon << QPointF(0,3) << QPointF(1,2) << QPointF(2,2.75)<< QPointF(3,1.5) << QPointF(2,.25) << QPointF(1, 1) << QPointF(0, 0) ;


        b2Body* ship_body = engine.createBody(b2_dynamicBody, 0, 0, 0, false);
        QGraphicsPolygonItem* pi = addPolygon(ship_body, scene, &engine, polygon);

        QGraphicsView view;
        view.resize(1280,720);
        view.show();
        view.setScene(scene);
        view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        MotionFilter* motion_filter = new MotionFilter(&view, pi, stalagtites_set, stalagmites_set);

        view.installEventFilter(motion_filter);

        view.centerOn(pi->x(), view.height()/2);

        engine.start();
        return app.exec();
}
