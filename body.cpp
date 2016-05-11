#include "body.h"
#include<iostream>

Body::Body()
{
    id = rand()*rand();
    isSelected = false;
}

long int Body::getId() const
{
    return id;
}

void Body::setType(int type)
{
    bodyType = type;
}

bool Body::getSelected()
{
    return isSelected;
}

void Body::setSelected(bool selected)
{
    isSelected = selected;
}

void Body::setNodeRadius(float newNodeRadius)
{
    nodeRadius = newNodeRadius;
}

void Body::setTransform(sf::Transform transform)
{
    physics2graphics = transform;
}

//define drawable object once all vertices are entered
void Body::finish()
{
    int nodeCount = nodeList.size();
    switch(bodyType)
    {
    case BALL:      vertices = sf::VertexArray(sf::TrianglesFan,nodeCount+1);//(sf::Points,nodeCount+1);
                    break;
    }
}

void Body::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //states.texture = &m_texture;
    states.transform *= physics2graphics;
    if(bodyType == RIGIDBALL) target.draw(rigidBall, states);
    target.draw(vertices, states);
}

void Body::updatePosition()
{
    int nodeCount = nodeList.size();
    if(bodyType == BALL){
        sf::Vector2f p0 = convertPosition(nodeList[0]->GetPosition());
        if(isSelected) vertices[0].color = sf::Color::Green;
        else vertices[0].color = sf::Color::White;
       // vertices[0].position = physics2graphics.transformPoint(p0);
        vertices[0].position = p0;
        for(int i(1);i<nodeCount;i++)
        {
            sf::Vector2f p = convertPosition(nodeList[i]->GetPosition());
            //float norm = sqrt(p.x*p0.x + p.y*p0.y); //account for node radius
            float norm = sqrt((p.x-p0.x)*(p.x-p0.x) + (p.y-p0.y)*(p.y-p0.y)); //account for node radius
            p = (p - p0)*(norm+nodeRadius)/norm + p0;
            //vertices[i].position = physics2graphics.transformPoint(p);
            vertices[i].position = p;
            vertices[i].color = sf::Color::Blue;
        }
        if(bodyType == BALL)
        {
            vertices[nodeCount].position = vertices[1].position;
            vertices[nodeCount].color = vertices[1].color;
        }
    } else if(bodyType == RIGIDBALL)
    {
        rigidBall.setPosition(convertPosition(nodeList[0]->GetPosition()));
        if(isSelected) rigidBall.setFillColor(sf::Color::Green);
        else rigidBall.setFillColor(sf::Color::White);
    }
    //TODO : solid wall
}

void Body::addNode(b2Body* node)
{
   nodeList.append(node);
}

b2Body* Body::getNode(int index)
{
    return nodeList[index];
}

void Body::setRadius(float radius_)
{
    radius = radius_;
    rigidBall.setRadius(radius);
    rigidBall.setOrigin(radius,radius);
}

sf::Vector2f Body::convertPosition(b2Vec2 v)
{
    return sf::Vector2f(v.x,v.y);
}

bool Body::operator==(const Body& rhs)
{
    return(getId() == rhs.getId());
}

void Body::destroyNodes()
{
        size_t numberOfNodes(nodeList.size());
        std::cout << numberOfNodes << std::endl;
        for(size_t i(0); i<numberOfNodes; i++)
        {
           nodeList[i]->GetWorld()->DestroyBody(nodeList[i]);
        }
}

bool Body::contains(sf::Vector2f position)
{
    if(bodyType == RIGIDBALL)
    {
        sf::Vector2f dist = position - rigidBall.getPosition();
        return (dist.x*dist.x + dist.y*dist.y < radius*radius);
    } else
    return vertices.getBounds().contains(position);
}
