#include "hapticinterface.h"
#include <iostream>

HapticInterface::HapticInterface(QObject *parent) : QThread(parent)
{
    device = new QTcpSocket(this);
    commSet = false;
    torque = 0;
    encoderOffset = 2360;
}

//void run();
    void HapticInterface::run(){
    //Connect this back later
    connect(device, &QIODevice::readyRead, this, &HapticInterface::readData);
    exec();
  }

void HapticInterface::setTorque(float newTorque)
{
    torque = newTorque;
    if(torque > 500) torque = 500;
    else if (torque < -500) torque = -500;
    if(device->bytesAvailable() == 0 && !commSet)
    {

        commSet = true;
    }
}

float HapticInterface::getPosition()
{
    return angle;
}

bool HapticInterface::connectToHost(QString host,int port)
{
    device->connectToHost(host,port);
    connect(device, SIGNAL(connected()), this, SLOT(reportConnected()));
    connect(device, SIGNAL(disconnected()), this, SLOT(reportDisconnected()));
    bool connectionSuccessful = device->waitForConnected();
    if(connectionSuccessful) ;//sendData();
    else std::cout << "connection failed" << std::endl;
    return connectionSuccessful;
}

void HapticInterface::changeOffset(int newOffset)
{
    encoderOffset = newOffset;
}

void HapticInterface::encodeData()
{
   //Encode each component of the force as a 10-bit number
   //float originalValue;
   uint16_t encodedValue;
   data.clear();
   encodedValue = (uint16_t)(torque+500);
  // std::cout <<"encoding" << std::endl;
   //std::cout <<torque << " "<<  encodedValue << std::endl;
   //encodedValue = 100;
   dataOut[0] = (uchar)((encodedValue>>8)&255);
   dataOut[1] = (uchar)((encodedValue)&255);
   //dataOut[0] = 14;
   //dataOut[1] = 5;
   //originalValue = force.x();
   //data.append((uchar)((encodedValue>>8)&255));
   //data.append((uchar)((encodedValue)&255));

}
bool HapticInterface::sendData()
{
    if(device->state() == QAbstractSocket::ConnectedState)
    {
        encodeData();
        //no need to write the size of the data because it's fixed (4 bytes)
        //so we just write the data itself
        //device->write(data,2);
        device->write(dataOut,2);
        return device->waitForBytesWritten();
    }
    else
        return false;
}

void HapticInterface::reportConnected()
{
    emit connected();
    std::cout << "device connected" << std::endl;
}
void HapticInterface::reportDisconnected()
{
    emit disconnected();
    std::cout << "device disconnected" << std::endl;
}

void HapticInterface::disconnect()
{
    device->disconnectFromHost();
}

void HapticInterface::readData()
 {
     while(device->bytesAvailable() > 0)
     {
         size_t bytesAvailable(device->bytesAvailable());
         // if more than 8 bytes are available, store them in buffer
         // and then take only the last 8 bytes to dataIn
         // to ignore old information
         if(bytesAvailable!= 2){
             device->read(buffer,device->bytesAvailable());
             for(int i(0);i<2;i++)
             {
                 dataIn[i] = buffer[bytesAvailable-2+i];
             }
         }
         else
         {
             device->read(dataIn,2);
         }

       decodeData();
       sendData();
     }
 }

void HapticInterface::ledState(bool state)
{
    if(state) ledData[0] = '1';
    else ledData[0] = '0';
    sendLed();
}

bool HapticInterface::sendLed()
{
    device->write(ledData,1);
    std::cout << ledData[0] << std::endl;
    return device->waitForBytesWritten();
}

void HapticInterface::decodeData()
 {
     uint rawValue;
     qreal decodedValue;
     rawValue = (uchar)dataIn[0];
     rawValue = rawValue << 8 | (uchar)dataIn[1];
     rawValue &= 4095;
     rawValue = (rawValue + encoderOffset) % 4096;
     decodedValue = ((qreal)rawValue)*2*pi/4096;
     if(decodedValue > pi) decodedValue -= 2*pi;
     angle = decodedValue;
     //std::cout << (angle.x()*180/pi) << " " << (angle.y()*180/pi) << std::endl;
     //std::cout << position.x() << " " << position.y()<< std::endl;
 }
/*
HapticInterface::~HapticInterface()
{
    exit(0);
    return 0;
}*/
