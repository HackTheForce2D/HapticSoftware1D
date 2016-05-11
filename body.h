#ifndef BODY_H
#define BODY_H

#include<SFML/Graphics.hpp>
#include<Box2D/Box2D.h>
#include<QVector>
#include<QVector2D>


const int WORKSPACE(0),BALL(1),RIGIDBALL(2);

class Body : public sf::Drawable
{
public:
    Body();
    void setType(int type);
    void setSelected(bool selected);
    void setNodeRadius(float newNodeRadius);
    void addNode(b2Body* node);
    bool contains(sf::Vector2f position);
    b2Body* getNode(int index);
    void finish();
    QString getName() const;
    void setRadius(float radius_);
    void setWallPosition(float rotation);
    void setTransform(sf::Transform transform);
    void updatePosition();
    long int getId() const;
    bool getSelected();
    bool operator==(const Body& rhs);
    void destroyNodes();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    int bodyType;
    long int id;
    float nodeRadius,radius; //for ball
    float rotation; //for wall
    sf::Vector2f convertPosition(b2Vec2 v);
    sf::VertexArray vertices;
    sf::CircleShape rigidBall;
    bool isSelected;
    //sf::Texture m_texture;
    QVector<b2Body*> nodeList;
    sf::Transform physics2graphics;
    sf::Transform rotateWall; //for wall
};

#endif // BODY_H
