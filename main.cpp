#include <QApplication>
#include <QtGui>
#include <QGraphicsLineItem>
#include "MyGraphicsView.h"
#include <vector>
#include <QPolygonF>

std::vector<int> stalagtites{0,0,0,0,10,10,20,20,10,10,10,30,10,0,0,10,40,0,0,10};
std::vector<int> stalagmites{0,0,0,0,20,20,20,20,20,30,30,40,40,30,0,0,0,0,0,10};


int main(int argc, char** argv) {
        QApplication app(argc, argv);

        QGraphicsScene *scene = new QGraphicsScene();

        QPen p;
        p.setWidth(0);
        for (unsigned int i = 0; i < stalagtites.size()-1; ++i) {
            scene->addLine(QLineF(i,stalagtites[i],i+1.,stalagtites[i+1]), p);
        }
        scene->addLine(QLineF(stalagtites.size()-1, stalagtites[stalagtites.size()-1], stalagtites.size(), 0), p);

        // Could use transformation to place stalagmites
        for (unsigned int i = 0; i < stalagmites.size()-1; ++i) {
            scene->addLine(QLineF(i,100-stalagmites[i],i+1.,100-stalagmites[i+1]), p);
        }
        scene->addLine(QLineF(stalagmites.size()-1, 100-stalagmites[stalagmites.size()-1], stalagmites.size(), 100), p);

        QPolygonF polygon;
        polygon << QPointF(0, 0) << QPointF(1, 1) << QPointF(2,.25) << QPointF(3,1.5) << QPointF(2,2.75) << QPointF(1,2) << QPointF(0,3);
        QGraphicsPolygonItem* pi = scene->addPolygon(polygon, p);
        pi->setScale(0.1);
        pi->setPos(10,50);

        MyGraphicsView view(pi);
        view.resize(1280,720);
        view.show();
        view.setScene(scene);
        view.scale(128,7.2);
        view.horizontalScrollBar()->setRange(0,1280);
        return app.exec();
}
