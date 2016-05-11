#include "effector.h"
#include <iostream>

Effector::Effector()
{
    graphical.setFillColor(sf::Color::Green);
    device2physics = device2physics.Identity;
    device2physics.rotate(180,sf::Vector2f(0,0));
    device2physics.translate(sf::Vector2f(0,-20));
    device2physics.scale(sf::Vector2f(-.2,.2),sf::Vector2f(0,0));
    kp = 20e3;

}

void Effector::setTransformHaptic(sf::Transform transform)
{
    device2physics = transform;
}

void Effector::setTransform(sf::Transform transform)
{
    physics2graphics = transform;
}

void Effector::setPhysical(b2Body * effectorPhysical)
{
    physical = effectorPhysical;
}

b2Vec2 Effector::getPosition()
{
    return physical->GetPosition();
}

b2Body* Effector::getPhysical()
{
    return physical;
}

void Effector::updateGraphic()
{
    float pi = 3.14159;
    sf::Vector2f p = convertPosition(physical->GetPosition());
    resetGraphical();
    graphical.setPosition(p);
    graphical.setRotation(physical->GetAngle()*180/pi);
}

void Effector::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform = physics2graphics;
    target.draw(graphical, states);
}

void Effector::setSize(float x,float y)
{
    size.x = x; size.y = y;
}

void Effector::resetGraphical()
{
    graphical.setPosition(sf::Vector2f(0,0));
    graphical.setSize(size);
    graphical.setOrigin(size/2.f);
}

float Effector::updateTorque(float angle)
{
    //std::cout << positionb2.x << " " << positionb2.y << std::endl;
    //Proportional control
    torque = kp*(angle - physical->GetAngle());
    if(torque > 1000) torque = 1000;
    else if(torque < -1000) torque = -1000;
    //physical->ApplyForce(force,physical->GetWorldCenter(),true);
    physical->ApplyTorque(torque,true);
    //return the reaction to the force as a QVector2D
    //to be used by the interface with the device
    return (-torque/2);

}

sf::Vector2f Effector::convertPosition(b2Vec2 v)
{
    return sf::Vector2f(v.x,v.y);
}

sf::Vector2f Effector::convertPosition(QVector2D v)
{
    return sf::Vector2f(v.x(),v.y());
}

b2Vec2 Effector::convertPosition(sf::Vector2f v)
{
    return b2Vec2(v.x,v.y);
}
