#include "physics.h"
#include<iostream>

Physics::Physics()
{
gravity = b2Vec2(0.0f, 0.0f);
world = new b2World(gravity);
velocityIterations = 6;
positionIterations = 2;
stepCount = 0;
timeStep = 0.001f;
timer = new QTimer(this);
}

//void Physics::createRigidBall(b2Vec2 position, float radius, bool canCollide=true)
void Physics::createRigidBall(b2Vec2 position, float radius)
{
    Body rigidBall;
    b2BodyDef rigidBallDef; //static body by default
    rigidBallDef.position.Set(position.x,position.y);
    b2Body* rigidBallBody = world->CreateBody(&rigidBallDef);
    b2CircleShape rigidBallShape;
    rigidBallShape.m_radius = radius;
    b2FixtureDef rigidBallFixtureDef;
    rigidBallFixtureDef.shape = &rigidBallShape;
    //fulcrum only supports the bar and doesn't collide with anything
    //if(canCollide == false)
    //{
        //rigidBallFixtureDef.filter.categoryBits = 0;
    //}
    rigidBallBody->CreateFixture(&rigidBallFixtureDef);
    rigidBall.setRadius(radius);
    rigidBall.addNode(rigidBallBody);
    rigidBall.finish();
    rigidBall.setType(RIGIDBALL);
    rigidBall.setTransform(physics2graphics);
    bodyList.append(rigidBall);
    emit objectListUpdated(bodyList);
    updateBodies();
}

void Physics::createBall(b2Vec2 position,float radius, float stiffness,
                         float damping, float density, float maxSpacing)
{
    Body ball;
    ball.setType(BALL);
    float nodeRadius = maxSpacing*.45;
    ball.setNodeRadius(nodeRadius);
    float mass(density*radius*radius);
    int nodeCount = (2*pi*radius/maxSpacing); //not including center
    float nodeDensity(mass/(nodeRadius*nodeRadius*(nodeCount+1)));
    b2BodyDef ballNodeDef;
    b2CircleShape ballNodeShape;
    ballNodeShape.m_radius = nodeRadius;
    b2FixtureDef ballFixtureDef;
    ballFixtureDef.shape = &ballNodeShape;
    ballFixtureDef.density = nodeDensity;
    b2DistanceJointDef nodeLinkDef;
    nodeLinkDef.frequencyHz = stiffness;
    nodeLinkDef.dampingRatio = damping;

    //center
    ballNodeDef.position.Set(position.x,position.y);
    b2Body* ballCenter = world->CreateBody(&ballNodeDef);
    ballCenter->CreateFixture(&ballFixtureDef);
    ball.addNode(ballCenter);
    //circle
    ballNodeDef.type = b2_dynamicBody;
    b2Vec2 nodePosition;
    //b2Body* firstNode =ballCenter,lastNode = ballCenter;
    for(int i(0);i<nodeCount;i++)
    {
        nodePosition = position + radius*b2Vec2(cos(2*pi*i/nodeCount),
                                                sin(2*pi*i/nodeCount));
        ballNodeDef.position.Set(nodePosition.x,nodePosition.y);
        b2Body* ballNode = world->CreateBody(&ballNodeDef);
        ballNode->CreateFixture(&ballFixtureDef);
        nodeLinkDef.Initialize(ballCenter,ballNode,
                               ballCenter->GetWorldCenter(),
                               ballNode->GetWorldCenter());
        world->CreateJoint(&nodeLinkDef);
        if(i>0)
        {
            nodeLinkDef.Initialize(ballNode,ball.getNode(i-1),
                                   ballNode->GetWorldCenter(),
                                   ball.getNode(i-1)->GetWorldCenter());
            world->CreateJoint(&nodeLinkDef);
        }
        ball.addNode(ballNode);
    }
    nodeLinkDef.Initialize(ball.getNode(nodeCount-1),ball.getNode(1),
                           ball.getNode(nodeCount-1)->GetWorldCenter(),
                           ball.getNode(1)->GetWorldCenter());
    world->CreateJoint(&nodeLinkDef);
    ball.finish();
    ball.setTransform(physics2graphics);
    bodyList.append(ball);
    emit objectListUpdated(bodyList);
}

void Physics::createEffector(float x, float y)
{
    b2BodyDef effectorDef;
    effectorDef.position.Set(0,0);
    effectorDef.type = b2_dynamicBody;
    b2Body* effectorPhysical = world->CreateBody(&effectorDef);
    b2PolygonShape effectorShape;
    effectorShape.SetAsBox(x/2,y/2);
    b2FixtureDef effectorFixtureDef;
    effectorFixtureDef.shape = &effectorShape;
    effectorFixtureDef.density = .0001;
    effectorPhysical->CreateFixture(&effectorFixtureDef);
    //effectorPhysical->SetLinearDamping(40);
    effectorPhysical->SetAngularDamping(1000);
    effector.setPhysical(effectorPhysical);
    effector.setSize(x,y);
}

 int Physics::getBodyCount()
 {
     return bodyList.size();
 }

Body Physics::getBody(int index)
{
    return bodyList[index];
}

void Physics::selectBody(int index)
{
    for(int i(1);i<getBodyCount();i++)
    {
        if(index == i) bodyList[i].setSelected(true);
        else bodyList[i].setSelected(false);
    }
}

void Physics::deleteSelected()
{
    for(int i(1);i<getBodyCount();i++)
    {
        if(bodyList[i].getSelected())
        {
            deleteBody(i);
        }
    }
}

Effector Physics::getEffector()
{
    return effector;
}

void Physics::setTransform(sf::Transform t)
{
    physics2graphics = t;
    for(int i(0);i<getBodyCount();i++)
    {
        bodyList[i].setTransform(physics2graphics);
    }
    effector.setTransform(physics2graphics);
}

void Physics::updateBodies()
{
    for(int i(0);i<getBodyCount();i++)
    {
        bodyList[i].updatePosition();
    }
    effector.updateGraphic();
}

void Physics::createBall(b2Vec2 position, float stiffness)
{
    createBall(position,2,stiffness/10,0.5f,.1,0.4);
    updateBodies();
}

void Physics::createEntities()
{
    //Create bar (effector)  and fulcrum (rigid ball)
    createEffector(1,10);
    createRigidBall(b2Vec2(0,0),1.5);
    std::cout << bodyList.size() << std::endl;
    b2RevoluteJointDef pivot;
    pivot.localAnchorA = b2Vec2(0,0);
    pivot.localAnchorB = b2Vec2(0,-5);
    pivot.collideConnected = false;
    pivot.bodyA = bodyList[0].getNode(0);
    pivot.bodyB = effector.getPhysical();
    world->CreateJoint(&pivot);
    //createBall(b2Vec2(-6,5),2,10,1,1,.45);
    //createBall(b2Vec2(6,5),2,5,1,1,.45);
}

void Physics::deleteBody(int index)
{
    stopSim();
    if(bodyList.size() > index && index > 0)
    {
        bodyList[index].destroyNodes();
        bodyList.removeAt(index);
    }
    std::cout <<  bodyList.size() << " bodies left"<< std::endl;
    emit objectListUpdated(bodyList);
    startSim();
}

void Physics::setHapticInterface(HapticInterface *i)
{
    hapticDevice = i;
}

void Physics::step()
{
    float angle(hapticDevice->getPosition());
   // std::cout << "angle : " << angle << std::endl;
    emit torqueUpdated(effector.updateTorque(angle));
    // std::cout << effector.getPosition().y << std::endl;
    //effector.getPhysical()->ApplyTorque(1e5,true);
    world->Step(timeStep,velocityIterations, positionIterations);
    stepCount++;
    //if(stepCount%1000 == 0) std::cout <<stepCount << " steps" << std::endl;
    //if(stepCount%1000 == 0) std::cout << effector.getPhysical()->GetAngle() << std::endl;
}

void Physics::run(){
connect(timer, SIGNAL(timeout()), this, SLOT(step()));
exec();
}

void Physics::startSim()
{
    timer->start((int)(timeStep*1000));
}
void Physics::stopSim()
{
    timer->stop();
}

void Physics::reset()
{
    size_t nbBodies = getBodyCount();
    std::cout << nbBodies << std::endl;
    //delete bodies from last to first
    for(size_t i(0); i <nbBodies;i++)
    {
        deleteBody(nbBodies-i-1);
    }
}
