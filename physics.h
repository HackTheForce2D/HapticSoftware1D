#ifndef PHYSICS_H
#define PHYSICS_H

#include <Box2D/Box2D.h>
#include <QVector>
#include <QVector2D>
#include <QTimer>
#include <body.h>
#include <QObject>
#include <effector.h>
#include <hapticinterface.h>

//const double pi = 3.1415926535897;

class Physics : public QThread
{
    Q_OBJECT
public:
    Physics();
    void setTimeStep(double timeStep);
    void createEntities();
    int getBodyCount();
    Body getBody(int index);
    QList<Body> * getObjects();
    Effector getEffector();
    void setTransform(sf::Transform t);
    void setHapticInterface(HapticInterface *i);

    void updateBodies();
    void run();

public slots:
    void step();
    void startSim();
    void stopSim();
    void reset();
    void selectBody(int);
    void deleteBody(int index);
    void deleteSelected();
    void createRigidBall(b2Vec2 position, float radius);
    void createBall(b2Vec2 position, float stiffness);

signals:
    void worldCreated();
    void torqueUpdated(float torque);
    void objectListUpdated(QList<Body> list);


private:
    double timeStep;
    int stepCount;
    int32 velocityIterations;
    int32 positionIterations;
    b2Vec2 gravity;
    b2World* world;
    QTimer *timer;
    HapticInterface *hapticDevice;
    sf::Transform physics2graphics;
    QList<Body> bodyList; //Model for the ListView
    Effector effector;
    QVector2D endEffectorRealPosition;
    void createBall(b2Vec2 position,float radius, float stiffness,
                    float damping, float density, float maxSpacing);
    void createEffector(float x, float y);
    void createEffector();
};

#endif // PHYSICS_H
