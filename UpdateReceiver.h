#ifndef UPDATERECEIVER_H
#define UPDATERECEIVER_H
#include <iostream>
#include "box2dengine.h"
class UpdateReceiver : public QObject {
Q_OBJECT
public:
    UpdateReceiver(QtBox2DEngine* engine): _engine(engine)  {
    }


public slots:
    void update() {
        QList<b2Body*> bodyList = _engine->bodyList();
        QList<b2Body*>::iterator it;
        for (it = bodyList.begin(); it != bodyList.end(); ++it) {
            if ((*it)->GetType() == b2BodyType(2)) {
                std::cout << (*it)->GetPosition().x << " " << (*it)->GetPosition().y << std::endl;

            }
        }
    }
private:
    QtBox2DEngine* _engine;
}
;

#endif // UPDATERECEIVER_H
