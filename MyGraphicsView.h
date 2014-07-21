#ifndef MYGRAPHICSVIEW_H
#define MYGRAPHICSVIEW_H

#include <iostream>
#include <QtGui>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>

#include <QScrollBar>

class MyGraphicsView : public QGraphicsView {
    Q_OBJECT

public:
    MyGraphicsView(QGraphicsPolygonItem*pi): QGraphicsView(), _pi(pi) {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        centerOn(_pi);
    }
     ~MyGraphicsView() { }

    void moveLeft(void) {
        // ensure not off left edge
        QPointF p = _pi->pos();
        p.setX(p.x() - 1);
        _pi->setPos(p);
        centerOn(_pi);
    }
    void moveRight(void) {
        // ensure not off right edge
        QPointF p = _pi->pos();
        p.setX(p.x() + 1);
        _pi->setPos(p);
        centerOn(_pi);
    }

    void moveUp(void) {
        QPointF p = _pi->pos();
        p.setY(p.y() - 0.5);
        _pi->setPos(p);
    }
    void moveDown(void) {
        QPointF p = _pi->pos();
        p.setY(p.y() + 0.5);
        _pi->setPos(p);
    }

protected:
    void keyPressEvent(QKeyEvent *e) {
         // Move left or right
        switch (e->key()) {
               case Qt::Key_Left:
                    moveLeft();
                    return; break;
               case Qt::Key_Right:
                    moveRight();
                    return; break;
               case Qt::Key_Up:
                    moveUp();
                    return; break;
              case Qt::Key_Down:
                    moveDown();
                    return; break;
        }
        // Otherwise pass to the graphics view
        QGraphicsView::keyPressEvent(e);
    }
private:
    QGraphicsPolygonItem* _pi;
};

#endif // MYGRAPHICSVIEW_H
