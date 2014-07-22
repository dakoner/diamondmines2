#include <QApplication>
#include <QtGui>
#include <QGraphicsLineItem>
#include <vector>
#include <QPolygonF>
#include <QScrollBar>
#include <set>

#include "MotionFilter.h"
std::vector<int> stalagtites{0,0,0,0,1,1,2,2,1,1,1,3,1,0,0,1,4,0,0,1,0,0,0,0,2,2,2,2,2,3,3,4,4,3,0,0,0,0,0,1};
std::vector<int> stalagmites{0,0,0,0,2,2,2,2,2,3,3,4,4,3,0,0,0,0,0,1,0,0,0,0,1,1,2,2,1,1,1,3,1,0,0,1,4,0,0,1};


int main(int argc, char** argv) {
        QApplication app(argc, argv);

        QGraphicsScene *scene = new QGraphicsScene();

        QPen p;
        p.setWidth(0);


        std::set<QGraphicsItem *> stalagtites_set;
        for (unsigned int i = 0; i < stalagtites.size()-1; ++i) {
           QGraphicsItem* item = scene->addLine(QLineF(i,stalagtites[i],(i+1.),stalagtites[i+1]), p);
           item->setScale(72);
           stalagtites_set.insert(item);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(stalagtites.size()-1, stalagtites[stalagtites.size()-1], stalagtites.size(), 0), p);
            item->setScale(72);
            stalagtites_set.insert(item);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(0,0,0,10), p);
            item->setScale(72);
            stalagtites_set.insert(item);
        }

        std::set<QGraphicsItem *> stalagmites_set;
        for (unsigned int i = 0; i < stalagmites.size()-1; ++i) {
            QGraphicsItem* item = scene->addLine(QLineF(i,10-stalagmites[i],(i+1.),10-stalagmites[i+1]), p);
            item->setScale(72);
            stalagmites_set.insert(item);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(stalagmites.size()-1, 10-stalagmites[stalagmites.size()-1], stalagmites.size(), 10), p);
            item->setScale(72);
            stalagmites_set.insert(item);
        }
        {
            QGraphicsItem* item = scene->addLine(QLineF(stalagmites.size(),0,stalagmites.size(),10), p);
            item->setScale(72);
            stalagmites_set.insert(item);
        }



        QPolygonF polygon;
        polygon << QPointF(0,3) << QPointF(1,2) << QPointF(2,2.75)<< QPointF(3,1.5) << QPointF(2,.25) << QPointF(1, 1) << QPointF(0, 0) ;
        QGraphicsPolygonItem* pi = scene->addPolygon(polygon, p);
        pi->setScale(8);
        pi->setPos(720,360);

        QGraphicsView view;
        view.resize(1280,720);
        view.show();
        view.setScene(scene);
        view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        MotionFilter* motion_filter = new MotionFilter(&view, pi, stalagtites_set, stalagmites_set);

        view.installEventFilter(motion_filter);

        view.centerOn(pi->x(), view.height()/2);

        return app.exec();
}
