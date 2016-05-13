#ifndef EFFECTOR_H
#define EFFECTOR_H

#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<QVector>
#include<QVector2D>
#include<body.h>


class Effector : public sf::Drawable
{
public:
    Effector();
    void setTransform(sf::Transform transform);
    void setTransformHaptic(sf::Transform transform);
    b2Vec2 getPosition();
    b2Body* getPhysical();
    void setPhysical(b2Body * effectorPhysical);
    void setSize(float x,float y);
    void updateGraphic();
    float updateTorque(float angle);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    b2ContactEdge * collisions;
    void resetGraphical();
    float kp,torqueFactor;
    float torque;
    sf::Vector2f convertPosition(b2Vec2 v);
    sf::Vector2f convertPosition(QVector2D v);
    b2Vec2 convertPosition(sf::Vector2f v);
    b2Body* physical;
    sf::RectangleShape graphical;
    sf::Vector2f size;
    sf::Transform physics2graphics,device2physics;
};

#endif // EFFECTOR_H
