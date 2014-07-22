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


int main(int argc, char** argv) {
        QApplication app(argc, argv);

        QGraphicsScene *scene = new QGraphicsScene();

        QPen p;
        p.setWidth(0);

        QtBox2DEngine engine;
        engine.setGravity(-9);


        std::set<QGraphicsItem *> stalagtites_set;
        b2Body* stalagtites_body = engine.createBody(b2_staticBody, 0, 0, 0, false);
        for (unsigned int i = 0; i < stalagtites.size()-1; ++i) {
           QGraphicsItem* item = scene->addLine(QLineF(i,stalagtites[i],(i+1.),stalagtites[i+1]), p);
           item->setScale(72);
           stalagtites_set.insert(item);
           b2EdgeShape *edge = new b2EdgeShape;
           edge->Set(b2Vec2(i,stalagtites[i]), b2Vec2((i+1.),stalagtites[i+1]));
           engine.createFixture(stalagtites_body, edge);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(stalagtites.size()-1, stalagtites[stalagtites.size()-1], stalagtites.size(), 0), p);
            item->setScale(72);
            stalagtites_set.insert(item);
            b2EdgeShape *edge = new b2EdgeShape;
            edge->Set(b2Vec2(stalagtites.size()-1, stalagtites[stalagtites.size()-1]), b2Vec2(stalagtites.size(), 0));
            engine.createFixture(stalagtites_body, edge);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(0,0,0,10), p);
            item->setScale(72);
            stalagtites_set.insert(item);
            b2EdgeShape *edge = new b2EdgeShape;
            edge->Set(b2Vec2(0,0), b2Vec2(0,10));
            engine.createFixture(stalagtites_body, edge);
        }

        std::set<QGraphicsItem *> stalagmites_set;
        b2Body* stalagmites_body = engine.createBody(b2_staticBody, 0, 0, 0, false);
        for (unsigned int i = 0; i < stalagmites.size()-1; ++i) {
            QGraphicsItem* item = scene->addLine(QLineF(i,10-stalagmites[i],(i+1.),10-stalagmites[i+1]), p);
            item->setScale(72);
            stalagmites_set.insert(item);
            b2EdgeShape *edge = new b2EdgeShape;
            edge->Set(b2Vec2(i,10-stalagmites[i]), b2Vec2((i+1.),10-stalagmites[i+1]));
            engine.createFixture(stalagmites_body, edge);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(stalagmites.size()-1, 10-stalagmites[stalagmites.size()-1], stalagmites.size(), 10), p);
            item->setScale(72);
            stalagmites_set.insert(item);
            b2EdgeShape *edge = new b2EdgeShape;
            edge->Set(b2Vec2(stalagmites.size()-1, 10-stalagmites[stalagmites.size()-1]), b2Vec2(stalagmites.size(), 10));
            engine.createFixture(stalagmites_body, edge);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(stalagmites.size(),0,stalagmites.size(),10), p);
            item->setScale(72);
            stalagmites_set.insert(item);
            b2EdgeShape *edge = new b2EdgeShape;
            edge->Set(b2Vec2(stalagmites.size(),0), b2Vec2(stalagmites.size(),10));
            engine.createFixture(stalagmites_body, edge);
        }


        QPolygonF polygon;
        polygon << QPointF(0,3) << QPointF(1,2) << QPointF(2,2.75)<< QPointF(3,1.5) << QPointF(2,.25) << QPointF(1, 1) << QPointF(0, 0) ;
        QGraphicsPolygonItem* pi = scene->addPolygon(polygon, p);
        pi->setScale(8);
        pi->setPos(720,360);
        
        B2Body* ship_body = engine.createBody(b2_dynamicBody, 0, 0, 0, false);
        b2Vec2* vertices;
                
        int count = polygon.count();
        b2Vec2* vertices = new b2Vec2[count];
                
        for (int i = 0; i < count; ++i) {
             QPointF p = polygon.at(i);
             vertices[i].Set(p.x(), -p.y());
        }

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
