#ifndef HAPTICINTERFACE_H
#define HAPTICINTERFACE_H

#include<QVector2D>
#include<QMatrix2x2>
#include<QtNetwork>
#include<QHostAddress>
#include<vector>
#include <cmath>

const double pi = 3.1415926535897;

class HapticInterface : public QThread
{
    Q_OBJECT
public:
    explicit HapticInterface(QObject *parent = 0);
    float getPosition();
    void updateTorque();
    void run();

public slots:
    bool connectToHost(QString host,int port);
    void readData();
    void setTorque(float newTorque);
    void disconnect();
    void reportConnected();
    void reportDisconnected();
    void ledState(bool state);

signals:
    void connected();
    void disconnected();

private:
    float angle, torque;
    QByteArray data;
    char dataIn[2],dataOut[2],buffer[8000],ledData[2];
    QTcpSocket *device;
    void encodeData();
    void decodeData();
    bool sendData();
    bool sendLed();
};

#endif // HAPTICINTERFACE_H
