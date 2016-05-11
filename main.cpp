#include "mainwindow.h"
#include <QApplication>
#include <physics.h>
#include <visual.h>
#include <hapticinterface.h>
#include <QPushButton>
#include <QListView>

//Q_DECLARE_METATYPE(QList<Body>)

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    ConnectionDialog connectionDialog;
    //c.show();
    Visual * display =  w.findChild<Visual*>(QString("display"));
    QPushButton * buttonRun = w.findChild<QPushButton*>(QString("buttonRun"));
    QPushButton * buttonStop = w.findChild<QPushButton*>(QString("buttonStop"));
    HapticInterface simpleBar;
    Physics physics;
    physics.setHapticInterface(&simpleBar);
    display->setPhysics(&physics);

    QObject::connect(&physics,SIGNAL(torqueUpdated(float)),
                     &simpleBar,SLOT(setTorque(float)));
    QObject::connect(buttonRun,SIGNAL(clicked()),
                     &physics,SLOT(startSim()));
    QObject::connect(buttonStop,SIGNAL(clicked()),
                     &physics,SLOT(stopSim()));
    QObject::connect(&w, SIGNAL(deleteSelected()),
                     &physics,SLOT(deleteSelected()));
    QObject::connect(&w, SIGNAL(deleteAll()),
                     &physics,SLOT(reset()));
    QObject::connect(&w, SIGNAL(findDevice()),
                     &connectionDialog,SLOT(show()));
    QObject::connect(&w, SIGNAL(addElasticBody()),
                     display,SLOT(addElasticBody()));
    QObject::connect(&w, SIGNAL(addRigidBody()),
                     display,SLOT(addRigidBody()));
    QObject::connect(display,SIGNAL(bodyClicked(int)),
                      &physics,SLOT(selectBody(int)));
    QObject::connect(&w, SIGNAL(newStiffness(float)),
                     display,SLOT(setStiffness(float)));
    QObject::connect(&connectionDialog,SIGNAL(connectToDevice(QString,int)),
                     &simpleBar,SLOT(connectToHost(QString ,int)));
    qRegisterMetaType<b2Vec2>("b2Vec2");
    QObject::connect(display, SIGNAL(createBall(b2Vec2,float)),
                     &physics,SLOT(createBall(b2Vec2,float)));
    QObject::connect(display, SIGNAL(createRigidBall(b2Vec2,float)),
                     &physics,SLOT(createRigidBall(b2Vec2,float)));
    QObject::connect(&w,SIGNAL(ledState(bool)),
                     &simpleBar, SLOT(ledState(bool)));


    //pantograph.connectToHost("localhost",53200);

    //Launch communication thread
    simpleBar.moveToThread(&simpleBar);
    simpleBar.start();
    //Launch simulation thread
    physics.moveToThread(&physics);
    physics.start();

    int exitValue(a.exec());
    physics.exit(); // Fix this
    simpleBar.exit();
    return exitValue;
}
